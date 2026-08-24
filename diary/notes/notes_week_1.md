# Дневник разработки: SDL2 → Miyoo display pipeline

## Цель этапа

Получить стабильный графический pipeline:

```text
Приложение
    ↓
SDL2 API
    ↓
Mini video driver
    ↓
Mini renderer
    ↓
Miyoo framebuffer / MI_GFX
    ↓
Дисплей
```

При этом код приложения должен работать через обычный SDL2 API и не зависеть напрямую от железа Miyoo.

---

## Что было сделано

### 1. SDL2 успешно запускается на Miyoo

Собрана собственная версия SDL2 и размещена в:

```text
/mnt/SDCARD/App/MIYOO_SQUARE/lib/libSDL2-2.0.so.0
```

Приложение использует её через:

```text
LD_LIBRARY_PATH=/mnt/SDCARD/App/MIYOO_SQUARE/lib:...
```

При этом наличие старого SDL 1.2 в системных библиотеках не мешает работе собственного SDL2.

---

### 2. Mini video driver успешно загружается

При запуске:

```text
INFO: TTF_Init succeeded!
SDL compiled video drivers:
  [0] Mini
```

Это подтверждает, что SDL2 успешно видит и использует наш `Mini` video driver.

Приложение доходит до:

```text
Entering MAIN_MENU
```

То есть SDL2 video subsystem и создание renderer уже работают.

---

### 3. Подтверждена работа SDL RenderCommandQueue

В `Mini_RunCommandQueue()` была добавлена подробная диагностика команд SDL:

```text
MINI: RunCommandQueue ENTER
MINI: CMD 1
MINI: CMD SETVIEWPORT
MINI: CMD 4
MINI: CMD CLEAR
```

Таким образом подтверждено, что вызовы SDL2 действительно доходят до нашего Mini renderer.

Цепочка:

```text
SDL_RenderClear()
    ↓
SDL RenderCommand
    ↓
Mini_RunCommandQueue()
    ↓
SDL_RENDERCMD_CLEAR
```

работает.

---

### 4. Подтверждён вызов RenderPresent

В логах регулярно появляется:

```text
MINI: RenderPresent ENTER
```

Следовательно, после выполнения render command queue SDL2 вызывает наш `Mini_RenderPresent()`.

Текущая цепочка уже работает:

```text
SDL_RenderClear()
    ↓
SDL command queue
    ↓
Mini_RunCommandQueue()
    ↓
SDL_RENDERCMD_CLEAR
    ↓
Mini_RenderPresent()
```

---

### 5. Проверена инфраструктура Miyoo

На устройстве обнаружены необходимые графические библиотеки:

```text
/config/lib/libmi_gfx.so
/config/lib/libmi_disp.so
/config/lib/libmi_sys.so
...
```

Также присутствует framebuffer:

```text
/dev/fb0
```

и другие связанные с дисплеем устройства:

```text
/dev/mdisp
/dev/mi_disp
/dev/sstarfb
```

В Mini backend уже используется Miyoo graphics API через:

```c
GFX_Copy(...)
```

Это даёт готовую точку интеграции с аппаратным выводом.

---

## Текущее состояние

На данный момент подтверждено:

| Компонент                            | Состояние            |
| ------------------------------------ | -------------------- |
| SDL2 под Miyoo                       | ✅ работает           |
| Собственная `libSDL2`                | ✅ загружается        |
| Mini video driver                    | ✅ работает           |
| Создание renderer                    | ✅ работает           |
| RenderCommandQueue                   | ✅ работает           |
| `SDL_RENDERCMD_CLEAR` обнаруживается | ✅                    |
| `Mini_RenderPresent()` вызывается    | ✅                    |
| Miyoo graphics libraries             | ✅ доступны           |
| `/dev/fb0`                           | ✅ доступен           |
| Фактическая запись пикселей          | ❌ ещё не реализована |
| Стабильный вывод изображения         | ❌ ещё не получен     |
| `SDL_RenderCopy()`                   | ⏳ следующий этап     |
| Полноценный renderer                 | ⏳ позже              |

---

## Важный вывод

На текущем этапе проблема уже **не выглядит как проблема SDL2 или регистрации renderer**.

SDL2 успешно проходит большую часть пути:

```text
SDL2
 ↓
Mini driver
 ↓
Renderer
 ↓
RenderCommandQueue
 ↓
CLEAR
 ↓
RenderPresent
```

Теперь основной вопрос находится ниже:

```text
Mini renderer
    ↓
framebuffer / pixel buffer
    ↓
MI_GFX
    ↓
дисплей
```

---

# Следующий этап

Главная ближайшая цель:

> Получить стабильный однотонный экран через обычный SDL2 API.

Тест:

```c
SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
SDL_RenderClear(renderer);
SDL_RenderPresent(renderer);
```

Ожидаемый результат:

```text
┌─────────────────────┐
│                     │
│      RED SCREEN     │
│                     │
└─────────────────────┘
```

---

## План дальнейшей реализации

### Этап 1. Разобраться с физическим framebuffer

Определить:

* формат framebuffer;
* разрешение;
* pitch;
* способ записи;
* используется ли напрямую `/dev/fb0`;
* либо правильный путь идёт через `MI_GFX`;
* как именно `GFX_Copy()` передаёт изображение на дисплей.

Желательно сначала сделать отдельный минимальный hardware-тест без SDL.

---

### Этап 2. Реализовать `SDL_RENDERCMD_CLEAR`

Сейчас команда фактически только диагностируется:

```c
case SDL_RENDERCMD_CLEAR:
    printf("MINI: CMD CLEAR\n");
    break;
```

Следующая задача:

```text
SDL_RENDERCMD_CLEAR
        ↓
заливка framebuffer
        ↓
RenderPresent
        ↓
дисплей
```

После этого проверить:

```text
RED
GREEN
BLUE
BLACK
WHITE
```

---

### Этап 3. Добиться стабильного `RenderPresent`

После реализации `CLEAR` нужно убедиться, что:

```text
SDL_RenderClear()
SDL_RenderPresent()
```

могут выполняться постоянно без:

* падений;
* мерцания;
* зависаний;
* артефактов;
* деградации после длительной работы.

Это будет первый настоящий стабильный SDL2 → Miyoo display pipeline.

---

### Этап 4. Реализовать `SDL_RenderFillRect`

После стабильного `CLEAR`:

```c
SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
SDL_RenderClear(renderer);

SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

SDL_Rect rect = {
    50, 50,
    100, 100
};

SDL_RenderFillRect(renderer, &rect);

SDL_RenderPresent(renderer);
```

Цель:

```text
чёрный экран
+
красный прямоугольник
```

Это позволит проверить не только framebuffer, но и работу координат и примитивов.

---

### Этап 5. Реализовать `SDL_RenderCopy`

После примитивов перейти к текстурам:

```text
SDL_CreateTexture()
        ↓
SDL_UpdateTexture()
        ↓
SDL_RenderCopy()
        ↓
SDL_RENDERCMD_COPY
        ↓
MI_GFX / framebuffer
        ↓
display
```

Именно этот этап даст основу для нормальной графики приложения.

---

## Приоритет

Не пытаться сейчас реализовать весь SDL renderer.

Минимальный порядок:

```text
1. Framebuffer / MI_GFX
        ↓
2. CLEAR
        ↓
3. RenderPresent
        ↓
4. FILL_RECTS
        ↓
5. TEXTURE
        ↓
6. COPY
        ↓
7. Scaling / rotation
        ↓
8. Clipping / blending
        ↓
9. Остальные SDL renderer features
```

---

## Главный milestone

Первый значимый результат следующего этапа:

```c
SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
SDL_RenderClear(renderer);
SDL_RenderPresent(renderer);
```

→ **стабильный красный экран на Miyoo.**

После этого уже можно считать базовый pipeline:

```text
SDL2 → Mini → Miyoo → Display
```

доказанным и переходить к выводу реальных текстур.







___


# Изменения исходников SDL2 для Mini backend

## Контекст

В рамках отладки вывода SDL2 на Miyoo был временно изменён исходный код SDL2 `Mini` video renderer.

Цель изменений на текущем этапе не полноценная реализация рендера, а **диагностика цепочки SDL2 RenderCommand → Mini renderer → Miyoo graphics backend**.

Все изменения являются отладочными и должны быть сохранены отдельно, чтобы при необходимости можно было легко вернуть исходное состояние.

---

# 1. Изменённый файл

Основной изменённый файл:

```text
src/render/mini/SDL_render_mini.c
```

Изменялась функция:

```c
Mini_RunCommandQueue(...)
```

---

# 2. Что было сделано с `Mini_RunCommandQueue`

Исходная функция `Mini_RunCommandQueue()` была временно заменена на диагностическую реализацию.

Главная идея:

```text
SDL2 формирует RenderCommand
        ↓
Mini_RunCommandQueue()
        ↓
выводим тип команды в консоль
        ↓
при необходимости выполняем команду
```

Таким образом стало видно, какие SDL renderer-команды реально доходят до Mini backend.

---

# 3. Добавлена диагностика входа в RenderCommandQueue

В начало функции добавлено:

```c
printf("MINI: RunCommandQueue ENTER\n");
```

Теперь каждый вызов:

```c
Mini_RunCommandQueue(...)
```

фиксируется в логе:

```text
MINI: RunCommandQueue ENTER
```

Это позволяет отличить ситуацию:

```text
SDL не вызывает renderer
```

от:

```text
SDL вызывает renderer, но команда не обрабатывается
```

---

# 4. Добавлен вывод номера каждой RenderCommand

В цикле обработки команд добавлено:

```c
printf("MINI: CMD %d\n", (int)cmd->command);
```

Перед обработкой каждой команды теперь выводится её числовой идентификатор.

Например:

```text
MINI: CMD 4
```

Это позволило определить, какие именно команды SDL2 создаёт при конкретном вызове API.

---

# 5. Добавлена диагностика отдельных команд

В `switch (cmd->command)` были добавлены диагностические ветки.

### `SDL_RENDERCMD_NO_OP`

```c
case SDL_RENDERCMD_NO_OP:
    printf("MINI: CMD NO_OP\n");
    break;
```

### `SDL_RENDERCMD_SETVIEWPORT`

```c
case SDL_RENDERCMD_SETVIEWPORT:
    printf("MINI: CMD SETVIEWPORT\n");
    break;
```

### `SDL_RENDERCMD_SETCLIPRECT`

```c
case SDL_RENDERCMD_SETCLIPRECT:
    printf("MINI: CMD SETCLIPRECT\n");
    break;
```

### `SDL_RENDERCMD_SETDRAWCOLOR`

```c
case SDL_RENDERCMD_SETDRAWCOLOR:
    printf("MINI: CMD SETDRAWCOLOR\n");
    break;
```

### `SDL_RENDERCMD_CLEAR`

```c
case SDL_RENDERCMD_CLEAR:
    printf("MINI: CMD CLEAR\n");
    break;
```

### `SDL_RENDERCMD_DRAW_POINTS`

```c
case SDL_RENDERCMD_DRAW_POINTS:
    printf("MINI: CMD DRAW_POINTS\n");
    break;
```

### `SDL_RENDERCMD_DRAW_LINES`

```c
case SDL_RENDERCMD_DRAW_LINES:
    printf("MINI: CMD DRAW_LINES\n");
    break;
```

### `SDL_RENDERCMD_FILL_RECTS`

```c
case SDL_RENDERCMD_FILL_RECTS:
    printf("MINI: CMD FILL_RECTS\n");
    break;
```

### `SDL_RENDERCMD_COPY_EX`

```c
case SDL_RENDERCMD_COPY_EX:
    printf("MINI: CMD COPY_EX\n");
    break;
```

### `SDL_RENDERCMD_GEOMETRY`

```c
case SDL_RENDERCMD_GEOMETRY:
    printf("MINI: CMD GEOMETRY\n");
    break;
```

Также оставлена диагностика неизвестных команд:

```c
default:
    printf("MINI: CMD UNKNOWN = %d\n", (int)cmd->command);
    break;
```

---

# 6. Особое изменение: `SDL_RENDERCMD_COPY`

Для `SDL_RENDERCMD_COPY` была добавлена не только диагностика, но и фактический вызов существующего Miyoo graphics API.

Используются:

```c
SDL_Rect *verts;
SDL_Rect src;
SDL_Rect dst;
SDL_Texture *texture;
const void *pixels;
int pitch;
int c0;
int c1;
int scale;
```

---

## Получение координат команды

Из vertex buffer извлекаются два прямоугольника:

```c
verts =
    (SDL_Rect *)(((Uint8 *)vertices) +
                 cmd->data.draw.first);

src = verts[0];
dst = verts[1];
```

Таким образом:

```text
verts[0] → source rectangle
verts[1] → destination rectangle
```

---

# 7. Получение SDL texture

Из команды извлекается текстура:

```c
texture = cmd->data.draw.texture;
```

После чего через существующие функции Mini backend получаются её данные:

```c
pixels = get_pixels(texture);
pitch = get_pitch(texture);
```

---

# 8. Добавлена проверка валидности texture data

Перед попыткой передачи данных добавлено:

```c
if((pitch == 0) || (pixels == NULL)) {
    printf("MINI: COPY INVALID TEXTURE DATA\n");
    return -1;
}
```

То есть если SDL texture не содержит корректного pixel buffer:

```text
pixels == NULL
```

или:

```text
pitch == 0
```

функция не пытается выполнять `GFX_Copy()`.

---

# 9. Добавлена диагностика COPY

Перед масштабированием выводятся исходные параметры:

```c
printf(
    "MINI: COPY "
    "src=%d,%d,%d,%d "
    "dst=%d,%d,%d,%d "
    "pitch=%d "
    "pixels=%p\n",
    src.x, src.y, src.w, src.h,
    dst.x, dst.y, dst.w, dst.h,
    pitch,
    pixels
);
```

В логе можно увидеть:

```text
src
dst
pitch
pixels
```

Это необходимо для проверки того, действительно ли SDL texture доходит до Mini renderer.

---

# 10. Добавлено масштабирование под экран Miyoo

Используется:

```c
c0 = FB_W / vid_win->w;
c1 = FB_H / vid_win->h;
scale = c0 > c1 ? c1 : c0;
```

То есть выбирается минимальный коэффициент масштабирования:

```text
scale = min(
    FB_W / virtual_width,
    FB_H / virtual_height
)
```

Это позволяет вписать виртуальное SDL-окно в физический framebuffer.

---

# 11. Масштабируется destination rectangle

После вычисления `scale`:

```c
dst.w *= scale;
dst.h *= scale;
```

Таким образом размеры destination увеличиваются до физических размеров framebuffer.

---

# 12. Изменена координата X

Используется:

```c
dst.x =
    (vid_win->w - (verts[1].x + verts[1].w))
    * scale;
```

После этого добавляется горизонтальное центрирование:

```c
dst.x +=
    (FB_W - (vid_win->w * scale)) / 2;
```

То есть изображение не просто растягивается, но и центрируется по горизонтали.

---

# 13. Изменена координата Y

Используется:

```c
dst.y =
    verts[1].y * scale;
```

После этого также добавляется вертикальное центрирование:

```c
dst.y +=
    (FB_H - (vid_win->h * scale)) / 2;
```

Итоговая модель:

```text
SDL virtual coordinates
        ↓
scale
        ↓
physical framebuffer coordinates
        ↓
centered on physical display
```

---

# 14. Добавлена диагностика итогового destination

Перед передачей изображения в Miyoo graphics API добавлено:

```c
printf(
    "MINI: GFX_Copy "
    "dst=%d,%d,%d,%d "
    "scale=%d\n",
    dst.x, dst.y,
    dst.w, dst.h,
    scale
);
```

Теперь можно отдельно видеть:

```text
SDL destination
        ↓
scaled destination
        ↓
final GFX destination
```

---

# 15. Вызов Miyoo `GFX_Copy`

Для реального копирования текстуры используется:

```c
GFX_Copy(
    pixels,
    src,
    dst,
    pitch,
    0,
    E_MI_GFX_ROTATE_180
);
```

То есть Mini backend уже имеет участок, который пытается передавать SDL texture непосредственно в Miyoo graphics stack.

Используется:

```c
E_MI_GFX_ROTATE_180
```

Это связано с ориентацией изображения на конкретном Miyoo hardware setup.

---

# 16. Обработка команд пока НЕ реализована

Важно помнить, что текущая версия `Mini_RunCommandQueue()` является в значительной степени диагностической.

Например:

```c
case SDL_RENDERCMD_CLEAR:
    printf("MINI: CMD CLEAR\n");
    break;
```

пока **не очищает framebuffer**.

То же самое относится к:

```text
SETVIEWPORT
SETCLIPRECT
SETDRAWCOLOR
DRAW_POINTS
DRAW_LINES
FILL_RECTS
COPY_EX
GEOMETRY
```

Они сейчас в основном только фиксируются в логе.

---

# 17. Что реально работает в текущем варианте

По результатам теста подтверждено:

```text
SDL_RenderClear()
        ↓
SDL_RENDERCMD_CLEAR
        ↓
Mini_RunCommandQueue()
        ↓
лог
```

и:

```text
SDL_RenderPresent()
        ↓
Mini_RenderPresent()
        ↓
лог
```

То есть текущие изменения успешно превратили Mini renderer в удобный диагностический слой.

---

# 18. Важное предупреждение при возврате исходников

При дальнейшем переходе к реализации настоящего renderer нужно помнить, что текущая функция была заменена целиком.

Поэтому нельзя просто удалить отдельные `printf` и считать backend восстановленным.

Нужно либо:

1. восстановить оригинальную реализацию `Mini_RunCommandQueue()`;

либо:

2. взять текущую реализацию и постепенно вернуть обработку SDL-команд, сохранив необходимые диагностические сообщения.

---

# 19. Что желательно сохранить перед следующими изменениями

Текущую версию файла:

```text
src/render/mini/SDL_render_mini.c
```

желательно сохранить как отдельный debug snapshot.

Например:

```text
SDL_render_mini.c.debug_command_queue
```

или сделать git commit:

```text
debug: instrument Mini render command queue
```

Это позволит безболезненно вернуться к состоянию сегодняшнего дня.

---

# 20. Следующая правка

Следующая логичная модификация должна быть **не `COPY`**, а:

```c
case SDL_RENDERCMD_CLEAR:
```

Нужно сделать минимальную реализацию:

```text
SDL_RenderClear()
        ↓
получить текущий draw color
        ↓
заполнить framebuffer
        ↓
SDL_RenderPresent()
        ↓
Miyoo display
```

Цель первого теста:

```c
SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
SDL_RenderClear(renderer);
SDL_RenderPresent(renderer);
```

и получить стабильный красный экран.

После этого можно переходить к:

```text
CLEAR
  ↓
FILL_RECTS
  ↓
TEXTURE
  ↓
COPY
  ↓
полноценный pipeline
```

---

# Итог

На сегодняшний момент исходники SDL2 были изменены прежде всего для **рентгенографии Mini renderer**.

Главное изменение:

```text
Mini_RunCommandQueue()
```

теперь позволяет видеть в реальном времени:

```text
какая команда пришла
        ↓
какие параметры она содержит
        ↓
какая texture используется
        ↓
какой destination рассчитан
        ↓
вызывается ли Miyoo GFX
```

Это изменение является **отладочным**, а не финальной реализацией renderer.

Главная точка, которую нужно будет помнить при дальнейшем рефакторинге:

```text
src/render/mini/SDL_render_mini.c
        │
        └── Mini_RunCommandQueue()
                │
                ├── diagnostic logging
                ├── command dispatch
                └── temporary COPY → GFX_Copy()
```

Именно здесь сейчас находится основная рабочая область для перехода от подтверждённого SDL2 command pipeline к реальному выводу пикселей на Miyoo.



Пример автора не работает

И это, кстати, довольно ценный результат. Мы фактически доказали, что сам репозиторий steward-fu/sdl2 в нашей конфигурации не является готовым drop-in SDL2 для вывода на экран Miyoo. Авторский пример показывает предполагаемый API-пайплайн, но не гарантирует, что конкретная реализация Mini backend-а на твоей прошивке действительно завершает вывод.