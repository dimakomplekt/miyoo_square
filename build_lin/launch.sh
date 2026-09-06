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
# если readelf есть в системе
readelf -d "$APP_DIR/lib/libSDL2-2.0.so.0" 2>/dev/null |
    grep NEEDED >> "$LOG_FILE" 2>&1

echo >> "$LOG_FILE"
echo "=== START ===" >> "$LOG_FILE"


"$APP_DIR/MIYOO_SQUARE" >> "$LOG_FILE" 2>&1

EXIT_CODE=$?

echo >> "$LOG_FILE"
echo "EXIT_CODE=$EXIT_CODE" >> "$LOG_FILE"
echo "========================================" >> "$LOG_FILE"

exit "$EXIT_CODE"
