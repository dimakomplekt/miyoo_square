#!/bin/bash

TOOLCHAIN_ROOT="/home/dimakomplekt/miyoo_toolchain"

READELF="$TOOLCHAIN_ROOT/mini/bin/arm-linux-gnueabihf-readelf"

ELF="build_lin/MIYOO_SQUARE"
LIB_DIR="build_lin/lib"

echo "========================================"
echo " Miyoo ELF dependency check"
echo "========================================"

if [ ! -f "$ELF" ]; then
    echo "ERROR: ELF not found:"
    echo "  $ELF"
    exit 1
fi

if [ ! -x "$READELF" ]; then
    echo "ERROR: readelf not found:"
    echo "  $READELF"
    exit 1
fi

echo
echo "Executable:"
echo "  $ELF"

echo
echo "Architecture:"
"$READELF" -h "$ELF" | grep -E "Class:|Machine:"

echo
echo "Direct dependencies:"
"$READELF" -d "$ELF" | grep NEEDED

echo
echo "========================================"
echo " Dependency tree"
echo "========================================"

declare -A VISITED

check_dependencies()
{
    local FILE="$1"
    local PREFIX="$2"

    local BASENAME
    BASENAME="$(basename "$FILE")"

    if [[ "${VISITED[$BASENAME]}" == "1" ]]; then
        echo "${PREFIX}${BASENAME} [already checked]"
        return
    fi

    VISITED[$BASENAME]=1

    echo "${PREFIX}${BASENAME}"

    while read -r LIB
    do
        # Try application lib directory first
        if [ -f "$LIB_DIR/$LIB" ]; then
            echo "${PREFIX}├── $LIB [LOCAL]"
            check_dependencies "$LIB_DIR/$LIB" "${PREFIX}│   "
            continue
        fi

        # Library is not inside our package
        echo "${PREFIX}├── $LIB [SYSTEM]"
    done < <(
        "$READELF" -d "$FILE" 2>/dev/null |
        sed -n 's/.*Shared library: \[\(.*\)\].*/\1/p'
    )
}

check_dependencies "$ELF" ""

echo
echo "========================================"
echo " Local libraries"
echo "========================================"

if [ -d "$LIB_DIR" ]; then
    find "$LIB_DIR" -maxdepth 1 \
        \( -type f -o -type l \) \
        -printf "  %f\n"
else
    echo "  ERROR: $LIB_DIR does not exist"
fi

echo
echo "========================================"
echo " Done"
echo "========================================"
