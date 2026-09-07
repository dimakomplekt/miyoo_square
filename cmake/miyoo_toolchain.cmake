set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# ============================================================
# MIYOO TOOLCHAIN
# ============================================================

if(NOT DEFINED MIYOO_TOOLCHAIN_ROOT)
    set(MIYOO_TOOLCHAIN_ROOT
        "/home/dimakomplekt/miyoo_toolchain"
    )
endif()

# ============================================================
# COMPILERS
# ============================================================

set(CMAKE_C_COMPILER
    "${MIYOO_TOOLCHAIN_ROOT}/mini/bin/arm-linux-gnueabihf-gcc"
)

set(CMAKE_CXX_COMPILER
    "${MIYOO_TOOLCHAIN_ROOT}/mini/bin/arm-linux-gnueabihf-g++"
)

set(CMAKE_AR
    "${MIYOO_TOOLCHAIN_ROOT}/mini/bin/arm-linux-gnueabihf-ar"
)

set(CMAKE_RANLIB
    "${MIYOO_TOOLCHAIN_ROOT}/mini/bin/arm-linux-gnueabihf-ranlib"
)

set(CMAKE_STRIP
    "${MIYOO_TOOLCHAIN_ROOT}/mini/bin/arm-linux-gnueabihf-strip"
)

# ============================================================
# SYSROOT
# ============================================================

set(MIYOO_SYSROOT
    "${MIYOO_TOOLCHAIN_ROOT}/mini/arm-buildroot-linux-gnueabihf/sysroot"
)

set(CMAKE_SYSROOT
    "${MIYOO_SYSROOT}"
)

set(CMAKE_FIND_ROOT_PATH
    "${MIYOO_SYSROOT}"
)

# ============================================================
# SEARCH MODES
# ============================================================

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)