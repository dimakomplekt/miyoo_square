# Miyoo build dependencies

Эта папка содержит зависимости для ARM-сборки `MIYOO_SQUARE` под Miyoo Mini.

Мои исходники можно скачать отсюда - https://drive.google.com/drive/folders/1H0RXBUeVgYKCPh1hy41QANdy_Hkx8eEo?usp=drive_link

## Содержимое

- `src_miyoo/miyoo_toolchain.tar.gz` — полный Linux toolchain: компиляторы ARM, sysroot, CMake/Ninja и служебные файлы.
- `src_miyoo/miyoo_sdl2_inputs.tar.gz` — заголовки SDL2, собранный SDL2, библиотеки `libmi_*`/`libshmvar` и EGL/GLES для профиля `640x480`.
- `libs_miyoo/` — готовые ARM runtime-библиотеки, которые можно скопировать в каталог приложения на SD-карте.

Архивы содержат Linux-файлы и предназначены для распаковки внутри WSL/Linux. Не распаковывайте их обычными Windows-инструментами, если нужно сохранить символические ссылки и права доступа.

## Установка toolchain

Из корня репозитория:

```bash
BASE_MODULES="libs/engine/base_modules/Miyoo_toolchain"
mkdir -p "$HOME/miyoo_toolchain" "$HOME/miyoo_sdl2"
tar -xzf "$BASE_MODULES/src_miyoo/miyoo_toolchain.tar.gz" -C "$HOME/miyoo_toolchain"
tar -xzf "$BASE_MODULES/src_miyoo/miyoo_sdl2_inputs.tar.gz" -C "$HOME"
```

После распаковки ожидаются пути:

```text
$HOME/miyoo_toolchain/mini/bin/arm-linux-gnueabihf-gcc
$HOME/miyoo_toolchain/mini/arm-buildroot-linux-gnueabihf/sysroot
$HOME/miyoo_sdl2/sdl2/include
$HOME/miyoo_sdl2/mini/lib
$HOME/miyoo_sdl2/prebuilt/640x480/libEGL.so
$HOME/miyoo_sdl2/prebuilt/640x480/libGLESv2.so
```

Проверьте, что `miyoo_toolchain/mini/bin/arm-linux-gnueabihf-gcc` исполняемый. Если используется другое расположение, задайте его перед сборкой:

```bash
export MIYOO_TOOLCHAIN_ROOT="$HOME/miyoo_toolchain"
export MIYOO_SDL2_ROOT="$HOME/miyoo_sdl2/sdl2"
export MIYOO_MINI_LIB_DIR="$HOME/miyoo_sdl2/mini/lib"
```

## Сборка

Скрипт сборки проекта должен использовать:

```text
cmake/miyoo_toolchain.cmake
PROJECT_PLATFORM=MIYOO
```

Toolchain-файл использует `MIYOO_TOOLCHAIN_ROOT`, а CMake-проект дополнительно берет SDL2 и runtime-зависимости из `MIYOO_SDL2_ROOT`, `MIYOO_MINI_LIB_DIR` и sysroot. Внутренний каталог сборки можно оставить вне репозитория, например `/tmp/miyoo_square_build`.

## Runtime-библиотеки

Файлы из `libs_miyoo/` собраны для ARM и не предназначены для запуска на x86 Linux или Windows. Скопируйте их в каталог `lib` рядом с исполняемым файлом на Miyoo:

```bash
cp libs/engine/base_modules/Miyoo_toolchain/libs_miyoo/* \
   /путь/к/SDCARD/App/MIYOO_SQUARE/lib/
```

В список включены:

```text
libbz2.so.1.0
libEGL.so
libfreetype.so.6
libGLESv2.so
libGLESv2.so.2
libjson-c.so.5
libpng16.so.16
libSDL2-2.0.so.0
libSDL2_image-2.0.so.0
libSDL2_ttf-2.0.so.0
libz.so.1
```

`libEGL.so` и `libGLESv2.so` взяты из профиля `640x480`. Версия `libGLESv2.so.2` сохранена отдельно из Miyoo runtime, потому что это другой ABI-совместимый файл с другим SONAME.

## Проверка после копирования

В WSL/Linux можно проверить архитектуру файлов так:

```bash
file libs_miyoo/*
```

Для библиотек должна быть указана `ELF 32-bit ... ARM, EABI5`. Не заменяйте эти файлы хостовыми библиотеками из `/usr/lib/x86_64-linux-gnu`.
