cmake_minimum_required(VERSION 3.10)
project(EricHpackFuzz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files for the library
set(HPACK_SOURCES hpack.cpp hpack.h)

add_library(hpack_lib ${HPACK_SOURCES})

# Fuzzer executable
add_executable(fuzz_hpack fuzz_hpack.cpp)
target_link_libraries(fuzz_hpack PRIVATE hpack_lib)

# Compiler flags for fuzzing
# We attempt to detect Clang to apply libFuzzer flags automatically.
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message(STATUS "Clang detected, adding -fsanitize=fuzzer,address")
    target_compile_options(hpack_lib PRIVATE -fsanitize=fuzzer-no-link,address -g -O1)
    target_compile_options(fuzz_hpack PRIVATE -fsanitize=fuzzer,address -g -O1)
    target_link_options(fuzz_hpack PRIVATE -fsanitize=fuzzer,address)
else()
    message(WARNING "Compiler is not Clang. You may need to manually specify fuzzing flags or link a fuzzing engine.")
    # For GCC or others, one might need -fsanitize=fuzzer if supported (newer GCC) or link against libFuzzer manually.
endif()
