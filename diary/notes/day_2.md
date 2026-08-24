# Дневник разработки: SDL2 → Miyoo display pipeline

## Цель этапа

Продолжить перенос SDL2 на Miyoo Mini и получить минимально рабочий графический pipeline:

```text

SDL2 приложение
      ↓
SDL2 API
      ↓
Miyoo Mini SDL2 backend
      ↓
MI_GFX
      ↓
framebuffer
      ↓
дисплей

```

На этом этапе задача была не сделать полноценный renderer, а доказать, что наш SDL2 backend действительно получает команды SDL2 и способен самостоятельно выполнять их через нативный графический API Miyoo.

1. Текущая структура проекта и сборки


Работа ведётся в WSL через VS Code.

Основная директория проекта:

```text
/home/dimakomplekt/miyoo_sdl2
```

Внутри проекта находятся отдельные части:

```text

miyoo_sdl2/
├── sdl2/
│   └── SDL2 source
│
├── build-scripts/
│   └── скрипты сборки
│
└── [проект Miyoo / application]

```

При этом важно разделять:


```text

WSL
│
├── SDL2
│   └── наша модифицированная версия SDL2
│
├── build scripts
│   └── собирают SDL2 и связанные компоненты
│
└── Miyoo-side project
    └── собираем приложение/библиотеки

```


Отдельно существует проект в Windows, который используется для разработки и тестирования приложения.

Общая идея такая:

```text

Windows project
      │
      │ использует
      ▼
собранный SDL2
      │
      │ собранный в WSL
      ▼
Miyoo SDL2 backend
      │
      ▼
бинарник для Miyoo
      │
      ▼
SD-карта Miyoo

```

То есть SDL2 теперь фактически является отдельной собираемой зависимостью проекта.

Мы не редактируем SDL2 внутри Windows-проекта напрямую. Вместо этого:

```text

меняем исходники SDL2 в WSL;
собираем SDL2;
получаем новую SDL2-библиотеку;
приложение использует именно эту собранную версию;
запускаем приложение на Miyoo;
смотрим, какие команды доходят до нашего backend.

```

Это позволяет постепенно реализовывать собственный renderer, не ломая основной код приложения.

2. Изменения в Makefile приложения

В Makefile приложения была добавлена логика, позволяющая собирать SDL2 из нашей локальной директории и затем использовать результат при сборке приложения.

Смысл изменения:

```

SDL2 source
    ↓
build SDL2
    ↓
получить libSDL2
    ↓
использовать эту libSDL2
    ↓
собрать приложение

```

Это важно, потому что системная SDL2 на Miyoo нам не подходит.

На устройстве присутствуют старые SDL-библиотеки:

```
/customer/lib/libSDL-1.2.so.0
/customer/lib/libSDL-1.2.so.0.11.4
/customer/lib/libSDL.so
```

Но наша программа использует SDL2:

```
/mnt/SDCARD/App/MIYOO_SQUARE/lib/libSDL2-2.0.so.0
```

Поэтому приложение должно быть связано именно с нашей версией SDL2.

Кроме того, приложение запускается с собственным:

```
LD_LIBRARY_PATH
```

примерно такого вида:

```
/mnt/SDCARD/App/MIYOO_SQUARE/lib:/lib:/config/lib:/mnt/SDCARD/miyoo/lib:/mnt/SDCARD/.tmp_update/lib:/mnt/SDCARD/.tmp_update/lib/parasyte
```

Это позволяет подсовывать приложению наши версии библиотек.

В результате получилась контролируемая схема:

```

Наш SDL2
    +
Наш backend
    +
Наши зависимости
    ↓
libSDL2-2.0.so.0
    ↓
MIYOO_SQUARE

```


3. Что мы нашли на Miyoo

На устройстве нет привычного desktop Linux graphics stack.

Например:

```
/dev/fb0
```

присутствует.

При этом:

```
/dev/dri/*
```

отсутствует.

Также присутствуют специфические устройства:

```
/dev/mdisp
/dev/mi_disp
/dev/sstarfb
```

В системе есть библиотеки SigmaStar/Miyoo:

```
/config/lib/libmi_gfx.so
/config/lib/libmi_disp.so
/config/lib/libmi_common.so
```

Особенно важна:

```
libmi_gfx.so
```

Именно через MI_GFX мы сейчас можем выполнять графические операции непосредственно с framebuffer.

Также в приложении присутствуют:

```
libEGL.so
libGLESv2.so.2
```

Но на текущем этапе мы не используем OpenGL ES renderer.

Мы пошли по более прямому пути:

```

SDL2 renderer
      ↓
MI_GFX
      ↓
framebuffer

```


4. Что мы поняли про SDL2 backend

В исходниках SDL2 уже существовал backend:

```
src/render/mini/
```

То есть SDL2 уже знает о платформе Miyoo Mini.

Это оказалось очень важным открытием.

Мы ожидали, что backend придётся создавать практически с нуля.

На самом деле нам достался существующий backend, но он оказался незавершённым.

То есть это не готовый renderer.

Структура backend уже существует:

```
Mini_CreateRenderer()
Mini_CreateTexture()
Mini_UpdateTexture()
Mini_LockTexture()
Mini_UnlockTexture()

Mini_QueueSetViewport()
Mini_QueueSetDrawColor()
Mini_QueueDrawPoints()
Mini_QueueGeometry()
Mini_QueueFillRects()
Mini_QueueCopy()
Mini_QueueCopyEx()

Mini_RunCommandQueue()
Mini_RenderPresent()
```

Но значительная часть этих функций либо ничего не делает, либо только логирует вызов.

Например:

```cpp
static int Mini_QueueDrawPoints(...)
{
    debug("%s\n", __func__);
    return 0;
}
```

Или:

```cpp
static int Mini_QueueGeometry(...)
{
    debug("%s\n", __func__);
    return 0;
}
```


Поэтому SDL2 может успешно выбрать renderer:

Renderer name: Miyoo Mini

но это ещё не означает, что renderer умеет реально рисовать.

5. Как работает SDL2 renderer

Сегодня окончательно разобрались с важной частью архитектуры SDL2.

Когда приложение делает:

```cpp
SDL_SetRenderDrawColor(renderer, ...);
SDL_RenderFillRect(renderer, &rect);
SDL_RenderPresent(renderer);
```

SDL2 не обязательно сразу рисует прямоугольник на framebuffer.

Вместо этого backend получает команды.

Получается примерно:

```
SDL_SetRenderDrawColor()
        ↓
QueueSetDrawColor
        ↓
создаётся команда

затем:

SDL_RenderFillRect()
        ↓
QueueFillRects
        ↓
создаётся команда FILL_RECTS
        ↓
данные прямоугольника помещаются
в render vertex buffer

```

а затем:

```
SDL_RenderPresent()
        ↓
SDL запускает RunCommandQueue()
        ↓
backend проходит по цепочке SDL_RenderCommand
        ↓
каждая команда выполняется
```

Именно поэтому:

```
Mini_RunCommandQueue(...)
```

получает:

```
SDL_RenderCommand *cmd
```

и делает:

```
while (cmd) {
    switch (cmd->command) {
        ...
    }

    cmd = cmd->next;
}
```

То есть cmd здесь не является одним большим массивом команд.

Это начало цепочки команд:

```
cmd
 ↓
command
 ↓
next
 ↓
command
 ↓
next
 ↓
command
 ↓
NULL
```

А данные конкретной команды, например координаты прямоугольников, находятся отдельно в render vertex buffer.

Поэтому используется:

```
vertices + cmd->data.draw.first
```

Получается два уровня:

```
SDL_RenderCommand
    │
    ├── command type
    ├── metadata
    └── first/count
             │
             ▼
       vertex buffer
             │
             └── SDL_FRect[]

```


6. Первый тест SDL2 renderer

Был создан минимальный тест:

```cpp

SDL_Init(SDL_INIT_VIDEO);

SDL_CreateWindow(...);

SDL_CreateRenderer(
    window,
    -1,
    SDL_RENDERER_ACCELERATED
);

SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
SDL_RenderClear(renderer);

SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

SDL_Rect rect = {
    100,
    100,
    200,
    100
};

SDL_RenderFillRect(renderer, &rect);

SDL_RenderPresent(renderer);

```


Первоначальный результат:

красный экран

но зелёного прямоугольника не было.

Это было ожидаемо после анализа backend.

Команда:

```
FILL_RECTS
```

до backend доходила, но её обработчик только печатал координаты:

```
MINI: CMD FILL_RECTS
MINI: count=1
MINI: RECT[0] x=100.000000 y=100.000000 w=200.000000 h=100.000000
```


и ничего не рисовал.

7. Реализация QueueFillRects

Сначала мы реализовали помещение данных прямоугольников в render command buffer:

```cpp
static int Mini_QueueFillRects(
    SDL_Renderer *renderer,
    SDL_RenderCommand *cmd,
    const SDL_FRect *rects,
    int count
)
{
    SDL_FRect *verts;

    verts = (SDL_FRect *)SDL_AllocateRenderVertices(
        renderer,
        count * sizeof(SDL_FRect),
        0,
        &cmd->data.draw.first
    );

    if (!verts) {
        return -1;
    }

    SDL_memcpy(
        verts,
        rects,
        count * sizeof(SDL_FRect)
    );

    cmd->data.draw.count = count;

    return 0;
}
```

Теперь backend действительно сохраняет данные:

```
SDL_FRect[]
```

для последующего выполнения команды.

После этого RunCommandQueue() смог получить:

```cpp
x = 100
y = 100
w = 200
h = 100

```


8. Реализация состояния цвета

Следующая проблема была с цветом.

SDL_SetRenderDrawColor() создаёт команду:

```
SDL_RENDERCMD_SETDRAWCOLOR
```

Поэтому в Mini_RunCommandQueue() мы добавили состояние:

```cpp
Uint8 draw_r = 0;
Uint8 draw_g = 0;
Uint8 draw_b = 0;
Uint8 draw_a = 255;
```

При обработке:

```
case SDL_RENDERCMD_SETDRAWCOLOR:
```

сохраняем:

```
draw_r = cmd->data.color.r;
draw_g = cmd->data.color.g;
draw_b = cmd->data.color.b;
draw_a = cmd->data.color.a;
```


После этого команда:

```
SETDRAWCOLOR
```

влияет на последующую:

```
FILL_RECTS
```

То есть теперь backend понимает последовательность:

```
SETDRAWCOLOR
      ↓
сохранить текущий цвет

FILL_RECTS
      ↓
использовать сохранённый цвет
```

Это важный момент: цвет не обязан находиться непосредственно внутри команды FILL_RECTS.

Renderer имеет состояние.

9. Реализация FILL_RECTS через MI_GFX

Для реального рисования прямоугольника мы связали SDL-команду:

```
SDL_RENDERCMD_FILL_RECTS
```

с:

```
MI_GFX_QuickFill()
```

В итоге pipeline стал:

```
SDL_RenderFillRect()
        ↓
Mini_QueueFillRects()
        ↓
SDL_RenderCommand
        ↓
Mini_RunCommandQueue()
        ↓
SDL_RENDERCMD_FILL_RECTS
        ↓
получаем SDL_FRect
        ↓
преобразуем в MI_GFX_Rect_t
        ↓
MI_GFX_QuickFill()
        ↓
framebuffer

```


Для framebuffer формируется:

```
MI_GFX_Surface_t dst;
```

с:

```
dst.phyAddr =
    gfx.fb.phyAddr +
    (FB_W * gfx.vinfo.yoffset * FB_BPP);

dst.eColorFmt = E_MI_GFX_FMT_ARGB8888;
dst.u32Width = FB_W;
dst.u32Height = FB_H;
dst.u32Stride = FB_W * FB_BPP;
```

А SDL-прямоугольник:

```
SDL_FRect
```

преобразуется в:

```
MI_GFX_Rect_t
```

примерно:

```
rect.s32Xpos = (MI_S32)rects[i].x;
rect.s32Ypos = (MI_S32)rects[i].y;
rect.u32Width = (MI_U32)rects[i].w;
rect.u32Height = (MI_U32)rects[i].h;
```

После чего:

```
MI_GFX_QuickFill(
    &dst,
    &rect,
    color,
    &fence
);
```


и:

```
MI_GFX_WaitAllDone(TRUE, fence);
```

10. Главный результат сегодняшнего дня

Мы получили первый реально работающий SDL2 drawing primitive на Miyoo.

Тест:

```cpp

SDL_SetRenderDrawColor(
    renderer,
    0,
    255,
    0,
    255
);

SDL_RenderFillRect(
    renderer,
    &rect
);

```

привёл к:

```
MINI: QueueSetDrawColor r=0 g=255 b=0 a=255

MINI: CMD SETDRAWCOLOR

MINI: CMD FILL_RECTS

MINI: FILL color=FF00FF00
r=0 g=255 b=0 a=255

MINI: GFX RECT
x=100 y=100 w=200 h=100

```


И самое главное:

на физическом дисплее появился зелёный прямоугольник.

То есть мы впервые прошли весь путь:

```

SDL API
  ↓
SDL renderer command
  ↓
Mini backend
  ↓
SDL command queue
  ↓
SDL_FRect
  ↓
MI_GFX_Rect_t
  ↓
MI_GFX_QuickFill
  ↓
Miyoo framebuffer
  ↓
дисплей

```


Это уже не просто диагностика backend.

Это первый реально работающий primitive нашего renderer.

11. Важное наблюдение: RenderPresent не рисует прямоугольник

В процессе работы разобрались с ролью:

```
Mini_RenderPresent()
```

Он не обязан выполнять:

```
SDL_RenderFillRect
```

самостоятельно.

Основная логика такая:

```
SDL_RenderFillRect()
        ↓
формируется команда
        ↓
RunCommandQueue()
        ↓
команда реально выполняется
        ↓
MI_GFX рисует в framebuffer
        ↓
RenderPresent()
        ↓
GFX_Flip()
        ↓
готовый framebuffer показывается

```



То есть:

```

RunCommandQueue = выполнить команды кадра

RenderPresent = предъявить/переключить готовый кадр

```


Упрощённо:

```

RenderCommand
      ↓
"что нужно нарисовать?"

RunCommandQueue
      ↓
"нарисовать это сейчас"

RenderPresent
      ↓
"показать результат"
```

12. Почему прямоугольник оказался не там, где ожидалось

Прямоугольник:

```
SDL:
x = 100
y = 100
w = 200
h = 100
```


появился на дисплее ближе к нижней правой четверти.


Это означает, что базовый вызов:

```
MI_GFX_QuickFill()

```


работает.

Но система координат SDL и система координат framebuffer сейчас ещё не согласованы полностью.

Кроме того, в backend уже присутствуют преобразования координат для текстур:

```
scale
dst.x
dst.y
E_MI_GFX_ROTATE_180
```

что показывает наличие специфики ориентации/масштабирования Miyoo.

Поэтому координатный pipeline ещё предстоит привести к единой модели.

Пока важно другое:

координаты SDL доходят до MI_GFX
и MI_GFX действительно рисует объект.


13. Что сейчас уже работает

На текущий момент доказано:

```
SDL2 успешно запускается на Miyoo;
используется наш SDL2;
выбирается renderer:
Miyoo Mini
SDL_Init() работает;
SDL_CreateWindow() работает;
SDL_CreateRenderer() работает;
SDL_RenderClear() работает;
SDL_SetRenderDrawColor() доходит до backend;
SDL_RenderFillRect() доходит до backend;
SDL_FRect корректно передаётся через render vertex buffer;
Mini_RunCommandQueue() корректно проходит цепочку команд;
состояние draw color сохраняется;
MI_GFX_QuickFill() реально рисует;
GFX_Flip() показывает framebuffer;
первый зелёный SDL-прямоугольник реально появился на дисплее.
```

14. Что пока НЕ реализовано

```
Backend пока далеко не является полноценным SDL2 renderer.
```

Необходимо допилить как минимум:

* 14.1. Координатную систему

Нужно разобраться с:

```
SDL coordinates
        ↓
window coordinates
        ↓
Miyoo framebuffer coordinates
```

Особенно:

```
положение (0,0);
ориентация дисплея;
ROTATE_180;
масштабирование;
центрирование;
соответствие размеров окна и framebuffer.
```

14.2. Draw Points

Сейчас:

```
Mini_QueueDrawPoints()
```

ничего не делает.

Нужно реализовать:

```
SDL_RenderDrawPoint
SDL_RenderDrawPoints
```

через MI_GFX.

14.3. Draw Lines

Сейчас:

```cpp
renderer->QueueDrawLines = Mini_QueueDrawPoints;
```

Это временная заглушка и фактически неправильная реализация.

Нужно отдельное:

```
Mini_QueueDrawLines()
```

и соответствующее выполнение команды:

```
SDL_RENDERCMD_DRAW_LINES
```

14.4. Geometry

Сейчас:

```
Mini_QueueGeometry()
```

ничего не делает.

Нужно будет разобраться с:

```
vertices
indices
colors
UV
textures
```

и понять, каким способом оптимальнее реализовать geometry через MI_GFX.

14.5. Texture COPY

Частично уже существует:

```
SDL_RENDERCMD_COPY
```

и используется:

```
GFX_Copy()
```
Но этот участок требует дополнительной проверки.

Особенно:

```
координаты;
масштаб;
pitch;
формат пикселей;
ориентация;
clipping;
source/destination rectangles.
```

14.6. COPY_EX

Сейчас:

```
Mini_QueueCopyEx()
```

ничего не делает.

Нужно реализовать:

```
rotation
center
flip
```

или определить, какие возможности можно передать непосредственно MI_GFX.

14.7. Viewport

Сейчас:

```
Mini_QueueSetViewport()
```

только логирует вызов.

Нужно учитывать:

```
SDL_RenderSetViewport()
```

при преобразовании координат.

14.8. Clip Rect

Сейчас:

```
SDL_RENDERCMD_SETCLIPRECT
```

фактически игнорируется.

Нужно реализовать clipping.

14.9. Render Target / текстуры

Есть заготовки:

```
Mini_SetRenderTarget()
Mini_CreateTexture()
Mini_UpdateTexture()
Mini_LockTexture()
Mini_UnlockTexture()
```

но полноценной поддержки render target пока нет.

Нужно определить архитектуру:


```
SDL_Texture
    ↓
MI_GFX surface / buffer

и отдельно:

SDL_RenderTarget
    ↓
offscreen buffer

```

15. Важная архитектурная граница

Сейчас особенно важно не начать смешивать уровни.

Нужно сохранить структуру:

```
SDL API
   ↓
SDL renderer abstraction
   ↓
Mini backend
   ↓
MI_GFX abstraction
   ↓
framebuffer

То есть Mini_RunCommandQueue() не должен превращаться в огромный монолит, который знает вообще всё.

Лучше постепенно выделять функции уровня:

Mini_FillRect(...)
Mini_Copy(...)
Mini_DrawPoint(...)
Mini_DrawLine(...)

а затем:

SDL command
      ↓
Mini_* operation
      ↓
MI_GFX
```


Это позволит дальше развивать backend без превращения RunCommandQueue() в гигантский switch на несколько тысяч строк.

16. Следующий этап

Следующим рабочим этапом логично сделать не весь renderer сразу, а привести в порядок минимальный pipeline.

Приоритет:

1. Исправить координаты
        ↓
2. Проверить несколько прямоугольников
        ↓
3. Проверить разные цвета
        ↓
4. Проверить границы экрана
        ↓
5. Проверить масштабирование
        ↓
6. Реализовать DrawPoint
        ↓
7. Реализовать DrawLine
        ↓
8. Проверить SDL_RenderCopy
        ↓
9. Проверить текстуры

После этого уже переходить к более сложным операциям.

17. Текущее состояние проекта

На конец сегодняшней работы состояние можно описать так:
```
                    SDL2
                     │
                     ▼
             ┌───────────────┐
             │ Mini Renderer │
             └───────┬───────┘
                     │
          ┌──────────┴──────────┐
          │                     │
          ▼                     ▼
     SDL commands          textures
          │                     │
          ▼                     ▼
   RunCommandQueue         GFX_Copy
          │
          ▼
    FILL_RECTS
          │
          ▼
    MI_GFX_QuickFill
          │
          ▼
      framebuffer
          │
          ▼
       GFX_Flip
          │
          ▼
       DISPLAY
```

На данный момент первый кусок этой схемы работает физически:

```
SDL_RenderFillRect()
        ↓
MI_GFX_QuickFill()
        ↓
зелёный прямоугольник на Miyoo
```


Главный вывод сегодняшнего дня:

    Нам не нужно создавать SDL2 backend с нуля. В SDL2 уже есть каркас mini renderer, но он был незавершённым. Теперь мы начали превращать этот каркас в настоящий backend Miyoo, постепенно связывая SDL2 render commands с нативным MI_GFX.

Следующая задача:

    Не писать всё сразу, а системно довести SDL2 → Mini backend → MI_GFX pipeline до полноценной базовой реализации, начиная с исправления координатной системы.
    