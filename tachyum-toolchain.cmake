# Tell CMake how to use Tachyum Toolchain.
# Author: Igor Lesik 2022
#
# See https://cmake.org/cmake/help/book/mastering-cmake/chapter/Cross%20Compiling%20With%20CMake.html

# Use "Generic" because we assume no OS support.
set(CMAKE_SYSTEM_NAME Generic)

set(CMAKE_ASM_COMPILER  ${TACHY_TOOLCHAIN_HOME}/bin/tachy-as)
set(CMAKE_C_COMPILER    ${TACHY_TOOLCHAIN_HOME}/bin/tachy-gcc)
set(CMAKE_CXX_COMPILER  ${TACHY_TOOLCHAIN_HOME}/bin/tachy-g++)
set(CMAKE_LINKER        ${TACHY_TOOLCHAIN_HOME}/bin/tachy-ld)
set(CMAKE_AR            ${TACHY_TOOLCHAIN_HOME}/bin/tachy-ar)
set(CMAKE_RANLIB        ${TACHY_TOOLCHAIN_HOME}/bin/tachy-ranlib)
set(CMAKE_OBJCOPY       ${TACHY_TOOLCHAIN_HOME}/bin/tachy-objcopy)

set(CMAKE_FIND_ROOT_PATH ${TACHY_TOOLCHAIN_HOME})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)