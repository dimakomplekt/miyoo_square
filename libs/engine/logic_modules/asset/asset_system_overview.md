# Asset and instance system overview

## Purpose

The module separates a source resource (`Asset`) from a runtime, usually
object-specific representation (`Instance`):

```text
file on disk -> Asset_manager -> Asset -> Instance_manager -> Instance -> game object
```

An image asset owns the original `SDL_Surface`. An image instance owns a
derived surface containing its crop and scale. The game object borrows that
surface through `Image_instance::get_surface()` and never frees it.

## Intended lifetime pipeline

1. Create an `Asset_manager`.
2. Load a source with `add_asset(IMAGE_AT, path)` and retain its
   `handle_ctx`.
3. Create one or more image instances with
   `add_instance(IMAGE_AT, asset_handle)`. The creating caller is registered as
   the first subscriber automatically.
4. Additional game objects call `sub()` when they start using the instance.
   Each owner calls `unsub()` when its lifetime ends; the creator then calls
   `delete_instance_request()` after releasing its subscription.
5. The renderer reads the borrowed surface from
   `get_image_instance(instance_handle)->get_surface()`.
6. Delete the asset only after all its instances have been deleted.

Handles are slot index plus generation. A generation change invalidates stale
handles after deletion, while stable slots avoid pointer invalidation when a
different resource is added.

## Current implementation

Implemented:

- image asset loading through SDL_image;
- image instance crop, independent X/Y scale, dimensions, anchors and derived
  surface regeneration;
- generation-checked asset and instance handles;
- reference counters preventing deletion of an asset while instances exist;
- borrowed surface access suitable for a renderer or game object;
- explicit instance subscription counting with `sub()` / `unsub()`.

Audio, video and font enum values are reserved, but their concrete loaders and
instances intentionally fail with an explicit diagnostic until implemented.

Runtime paths are resolved relative to the executable directory. Engine-owned
resources are staged under `content/`, while application-owned resources from
`libs/app/global_data/app_content/` are staged under `app_content/`.

## SWOT analysis

### Strengths

- Clear source-versus-runtime separation.
- Runtime representations can be different per object without duplicating the
  source image.
- Generation handles make stale references detectable.
- SDL-specific details are currently isolated to the image implementation.

### Weaknesses

- Ownership is manual and requires a strict unsubscribe/delete protocol.
- Managers expose raw borrowed pointers, so lifetime rules must be documented
  and followed by callers.
- Asset lookup is currently linear only when finding a free slot; paths are not
  deduplicated.
- Image instances eagerly allocate a new surface for every crop/scale change.

### Opportunities

- Add typed asset/instance factories and a common loader interface for audio,
  video and fonts.
- Replace manual subscription calls with small RAII handle wrappers.
- Add path normalization, deduplication and an explicit loading state.
- Add renderer-side texture caching without changing the asset/instance model.
- Add asynchronous loading and a main-thread upload step for SDL textures.

### Threats

- Destroying managers while handles or borrowed surfaces are still in use is
  invalid.
- Large sprites or frequent animation changes can cause allocation and blit
  pressure.
- SDL surfaces are not generally safe to mutate or use across arbitrary
  threads.
- A future asset type must not be added only to the enum; it also needs a
  loader, typed instance, and manager validation.

## Suitability for a medium-sized game

For a platformer in the scope of *Dead Cells* or *Animal Well*, this is a
reasonable first resource layer for static and moderately changing 2D image
data. It is not yet a complete production asset pipeline: it lacks streaming,
hot reload, dependency tracking, memory budgets, texture-atlas support and
tooling. Those are extensions around the current separation rather than a
reason to discard it.

The main modernization priority should be making ownership harder to misuse:
introduce RAII wrappers (or an object/component subscription layer), keep
manager destruction at the end of the world/render lifecycle, and add
automated tests for stale handles, deletion ordering and surface
recalculation. Audio/video/font support should be added behind type-specific
loader and instance implementations, not by expanding image conditionals.
