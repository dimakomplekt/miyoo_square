#!/bin/bash
set -e

SCRIPT_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
cd "$SCRIPT_DIR"

TOOLCHAIN_ROOT="${MIYOO_TOOLCHAIN_ROOT:-/home/dimakomplekt/miyoo_toolchain}"
BUILD_DIR="${MIYOO_BUILD_DIR:-$SCRIPT_DIR/build_miyoo}"
DEPLOY_DIR="$SCRIPT_DIR/build_lin"


CMAKE="$TOOLCHAIN_ROOT/mini/bin/cmake"
GCC="$TOOLCHAIN_ROOT/mini/bin/arm-linux-gnueabihf-gcc"
GXX="$TOOLCHAIN_ROOT/mini/bin/arm-linux-gnueabihf-g++"
NINJA="$TOOLCHAIN_ROOT/mini/bin/ninja"

SYSROOT_LIB="$TOOLCHAIN_ROOT/mini/arm-buildroot-linux-gnueabihf/sysroot/usr/lib"

SDL2_ROOT="${MIYOO_SDL2_ROOT:-$HOME/miyoo_sdl2/sdl2}"
MINI_LIB_DIR="${MIYOO_MINI_LIB_DIR:-$HOME/miyoo_sdl2/mini/lib}"
MIYOO_PREBUILT_DIR="${MIYOO_PREBUILT_DIR:-$HOME/miyoo_sdl2/prebuilt/640x480}"

SDL2_LIB="$SDL2_ROOT/build/.libs/libSDL2-2.0.so.0.18.2"
SDL2_IMAGE_LIB="$SYSROOT_LIB/libSDL2_image-2.0.so.0"
SDL2_TTF_LIB="$SYSROOT_LIB/libSDL2_ttf-2.0.so.0"

# For 640 x 480
MIYOO_EGL="$MIYOO_PREBUILT_DIR/libEGL.so"
MIYOO_GLES="$MIYOO_PREBUILT_DIR/libGLESv2.so"

require_file()
{
    if [ ! -f "$1" ]; then
        echo "ERROR: required file is missing: $1"
        exit 1
    fi
}

fingerprint()
{
    file="$1"
    if command -v sha256sum >/dev/null 2>&1; then
        printf '%s: ' "$file"
        sha256sum "$file" | awk '{print $1}'
    else
        printf '%s: sha256sum unavailable\n' "$file"
    fi
    ls -l --time-style=full-iso "$file"
}

require_file "$SDL2_LIB"
require_file "$SDL2_IMAGE_LIB"
require_file "$SDL2_TTF_LIB"
require_file "$MIYOO_EGL"
require_file "$MIYOO_GLES"

echo "SDL2 source artifact:"
fingerprint "$SDL2_LIB"

for required in "$CMAKE" "$NINJA" "$SDL2_LIB" "$SDL2_IMAGE_LIB" \
    "$SDL2_TTF_LIB" "$MIYOO_EGL" "$MIYOO_GLES"; do
    if [ ! -f "$required" ]; then
        echo "ERROR: required Miyoo build input not found: $required" >&2
        exit 1
    fi
done

if [ ! -x "$CMAKE" ] || [ ! -x "$NINJA" ]; then
    echo "ERROR: CMake and Ninja must be executable." >&2
    exit 1
fi



echo "========================================"
echo "Configuring Miyoo build..."
echo "Toolchain: $TOOLCHAIN_ROOT"
echo "CMake:     $CMAKE"
echo "GCC:       $GCC"
echo "G++:       $GXX"
echo "Ninja:     $NINJA"
echo "Build:     $BUILD_DIR"
echo "========================================"


# ============================================================
# BUILD
# ============================================================

rm -rf "$BUILD_DIR"

"$CMAKE" \
    -S . \
    -B "$BUILD_DIR" \
    -G Ninja \
    -DCMAKE_MAKE_PROGRAM="$NINJA" \
    -DCMAKE_TOOLCHAIN_FILE="$PWD/cmake/miyoo_toolchain.cmake" \
    -DMIYOO_TOOLCHAIN_ROOT="$TOOLCHAIN_ROOT" \
    -DMIYOO_SDL2_ROOT="$SDL2_ROOT" \
    -DMIYOO_MINI_LIB_DIR="$MINI_LIB_DIR" \
    -DMIYOO_EGL_LIB="$MIYOO_EGL" \
    -DMIYOO_GLES_LIB="$MIYOO_GLES" \
    -DPROJECT_PLATFORM=MIYOO

"$CMAKE" --build "$BUILD_DIR"


# ============================================================
# DEPLOYMENT
# ============================================================

echo
echo "========================================"
echo "Preparing Miyoo deployment..."
echo "========================================"

rm -rf "$DEPLOY_DIR"

mkdir -p \
    "$DEPLOY_DIR/lib" \
    "$DEPLOY_DIR/content"


# ============================================================
# EXECUTABLE
# ============================================================

cp \
    "$BUILD_DIR/package/MIYOO_SQUARE" \
    "$DEPLOY_DIR/MIYOO_SQUARE"


# ============================================================
# CONTENT
# ============================================================

cp -r \
    "libs/engine/logic_modules/program_gui/basic_content/." \
    "$DEPLOY_DIR/content/"


# ============================================================
# SDL2
# ============================================================

cp -L \
    "$SDL2_LIB" \
    "$DEPLOY_DIR/lib/libSDL2-2.0.so.0"


# ============================================================
# SDL2 IMAGE
# ============================================================

cp -L \
    "$SDL2_IMAGE_LIB" \
    "$DEPLOY_DIR/lib/libSDL2_image-2.0.so.0"


# ============================================================
# SDL2 TTF
# ============================================================

cp -L \
    "$SDL2_TTF_LIB" \
    "$DEPLOY_DIR/lib/libSDL2_ttf-2.0.so.0"


# ============================================================
# MIYOO EGL / GLES
# ============================================================

echo "Copying Miyoo EGL/GLES..."

if [ ! -f "$MIYOO_EGL" ]; then
    echo "ERROR: Miyoo 640x480 libEGL.so not found:"
    echo "       $MIYOO_EGL"
    exit 1
fi

if [ ! -f "$MIYOO_GLES" ]; then
    echo "ERROR: Miyoo 640x480 libGLESv2.so not found:"
    echo "       $MIYOO_GLES"
    exit 1
fi

cp \
    "$MIYOO_EGL" \
    "$DEPLOY_DIR/lib/libEGL.so"

cp \
    "$MIYOO_GLES" \
    "$DEPLOY_DIR/lib/libGLESv2.so.2"

cp \
    "$MIYOO_GLES" \
    "$DEPLOY_DIR/lib/libGLESv2.so"


# ============================================================
# ADDITIONAL LIBS
# ============================================================

cp "$SYSROOT_LIB/libfreetype.so.6" \
   "$DEPLOY_DIR/lib/"

cp "$SYSROOT_LIB/libbz2.so.1.0" \
   "$DEPLOY_DIR/lib/"

cp "$SYSROOT_LIB/libpng16.so.16" \
   "$DEPLOY_DIR/lib/"

cp "$SYSROOT_LIB/libz.so.1" \
   "$DEPLOY_DIR/lib/"

cp "$SYSROOT_LIB/libjson-c.so.5" \
   "$DEPLOY_DIR/lib/"



# ============================================================
# LAUNCH SCRIPT
# ============================================================

cat > "$DEPLOY_DIR/launch.sh" << 'EOF'
#!/bin/sh

APP_DIR="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
LOG_FILE="$APP_DIR/launch.log"

export LD_LIBRARY_PATH="$APP_DIR/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}"

cd "$APP_DIR"

echo "========================================" > "$LOG_FILE"
echo "MIYOO_SQUARE LAUNCH" >> "$LOG_FILE"
echo "========================================" >> "$LOG_FILE"


echo "=== MIYOO SDL2 BACKEND LIBRARIES ===" >> "$LOG_FILE"


find /lib /config /customer /mnt/SDCARD \
    \( -name 'libEGL.so*' \
    -o -name 'libGLESv2.so*' \
    -o -name 'libjson-c.so*' \
    -o -name 'libmi_*.so*' \
    -o -name 'libshmvar.so*' \) \
    >> "$LOG_FILE" 2>&1


ulimit -c unlimited
echo "CORE_PATTERN=$(cat /proc/sys/kernel/core_pattern)" >> "$LOG_FILE"
echo "PWD=$(pwd)" >> "$LOG_FILE"


echo "APP_DIR=$APP_DIR" >> "$LOG_FILE"
echo "LD_LIBRARY_PATH=$LD_LIBRARY_PATH" >> "$LOG_FILE"
echo "LD_PRELOAD=${LD_PRELOAD-}" >> "$LOG_FILE"
echo "PID=$$" >> "$LOG_FILE"
echo "DATE=$(date)" >> "$LOG_FILE"

echo >> "$LOG_FILE"
echo "=== ENVIRONMENT ===" >> "$LOG_FILE"

echo "DISPLAY=$DISPLAY" >> "$LOG_FILE"
echo "SDL_VIDEODRIVER=$SDL_VIDEODRIVER" >> "$LOG_FILE"
echo "TERM=$TERM" >> "$LOG_FILE"
echo "PATH=$PATH" >> "$LOG_FILE"

echo >> "$LOG_FILE"
echo "=== FULL ENV ===" >> "$LOG_FILE"

env >> "$LOG_FILE" 2>&1


echo >> "$LOG_FILE"
echo "=== SDL LIBRARIES ===" >> "$LOG_FILE"

find /lib /usr/lib /config /customer \
    \( -name 'libSDL2*' -o -name 'libSDL*' \) \
    >> "$LOG_FILE" 2>&1


echo >> "$LOG_FILE"
echo "=== VIDEO DEVICES ===" >> "$LOG_FILE"

ls -l /dev/fb* >> "$LOG_FILE" 2>&1
ls -l /dev/dri/* >> "$LOG_FILE" 2>&1


echo >> "$LOG_FILE"
echo "=== /dev VIDEO-RELATED ===" >> "$LOG_FILE"

ls -l /dev | grep -Ei 'fb|gpu|drm|disp|lcd|video' \
    >> "$LOG_FILE" 2>&1


echo >> "$LOG_FILE"


echo >> "$LOG_FILE"
echo "=== APP LIB DIRECTORY ===" >> "$LOG_FILE"
ls -lah "$APP_DIR/lib" >> "$LOG_FILE" 2>&1

echo >> "$LOG_FILE"
echo "=== EGL FILE ===" >> "$LOG_FILE"
ls -lah "$APP_DIR/lib/libEGL.so"* >> "$LOG_FILE" 2>&1

echo >> "$LOG_FILE"
echo "=== EGL REALPATH ===" >> "$LOG_FILE"
readlink -f "$APP_DIR/lib/libEGL.so" >> "$LOG_FILE" 2>&1

echo >> "$LOG_FILE"
echo "=== SDL2 NEEDED ===" >> "$LOG_FILE"
echo "--- $APP_DIR/MIYOO_SQUARE ---" >> "$LOG_FILE"
readelf -d "$APP_DIR/MIYOO_SQUARE" 2>/dev/null | grep NEEDED >> "$LOG_FILE" 2>&1
for lib in "$APP_DIR"/lib/libSDL2*.so*; do
    echo "--- $lib ---" >> "$LOG_FILE"
    ls -l "$lib" >> "$LOG_FILE" 2>&1
    sha256sum "$lib" >> "$LOG_FILE" 2>&1
    readelf -d "$lib" 2>/dev/null | grep NEEDED >> "$LOG_FILE" 2>&1
done

echo >> "$LOG_FILE"
echo "=== START ===" >> "$LOG_FILE"


"$APP_DIR/MIYOO_SQUARE" >> "$LOG_FILE" 2>&1

EXIT_CODE=$?

echo >> "$LOG_FILE"
echo "EXIT_CODE=$EXIT_CODE" >> "$LOG_FILE"
echo "========================================" >> "$LOG_FILE"

exit "$EXIT_CODE"
EOF

chmod +x "$DEPLOY_DIR/launch.sh"



echo
echo "========================================"
echo "DEPLOYMENT CHECK"
echo "========================================"

ls -lah "$DEPLOY_DIR/lib"

echo
echo "Checking required libraries..."

for lib in \
    libSDL2-2.0.so.0 \
    libSDL2_image-2.0.so.0 \
    libSDL2_ttf-2.0.so.0 \
    libEGL.so \
    libGLESv2.so \
    libGLESv2.so.2
do
    if [ -f "$DEPLOY_DIR/lib/$lib" ]; then
        echo "OK: $lib"
    else
        echo "ERROR: missing $lib"
        exit 1
    fi
done

echo
echo "Deployment fingerprints:"
for lib in \
    "$DEPLOY_DIR/lib/libSDL2-2.0.so.0" \
    "$DEPLOY_DIR/lib/libSDL2_image-2.0.so.0" \
    "$DEPLOY_DIR/lib/libSDL2_ttf-2.0.so.0"
do
    fingerprint "$lib"
done


# ============================================================
# RESULT
# ============================================================

echo
echo "========================================"
echo "Miyoo deployment:"
echo "========================================"

find "$DEPLOY_DIR" -maxdepth 2 -type f -print
