cmake_minimum_required(VERSION 3.16)

# Top CMake script for building TVILIB.
# Author: Igor Lesik 2022

# Must have a pointer to directory where Toolchain is installed.
if(NOT DEFINED TACHY_TOOLCHAIN_HOME)
    message(FATAL_ERROR "TACHY_TOOLCHAIN_HOME not defined, do 'cmake -DTACHY_TOOLCHAIN_HOME=your_toolchain/install/set0'")
endif()

set(CMAKE_CROSSCOMPILING TRUE)
set(CMAKE_TOOLCHAIN_FILE tachyum-toolchain.cmake)

project(TVILIB
    VERSION 0.1.0
    DESCRIPTION "Internal CPU Performance Benchmarks"
    LANGUAGES ASM C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(.)

set(CMAKE_C_FLAGS "-O2 -Werror -Wall -Wextra -static")
set(CMAKE_CXX_FLAGS "-O2 -Werror -Wall -Wextra -static")
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--gc-sections")

set(FSIM ${TACHY_TOOLCHAIN_HOME}/bin/tachy-cyclerun)
set(PSIM ${TACHY_TOOLCHAIN_HOME}/bin/psim)
set(QEMU ${TACHY_TOOLCHAIN_HOME}/bin/qemu-system-tachy)

# Install inside the build directory.
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

#set(REPORTS_DIR ${CMAKE_BINARY_DIR}/reports)
#file(MAKE_DIRECTORY ${REPORTS_DIR} ${REPORTS_DIR}/fsim ${REPORTS_DIR}/psim ${REPORTS_DIR}/rtlsim)

enable_testing()

add_subdirectory(tachy)
add_subdirectory(test)

