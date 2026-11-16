cmake_minimum_required(VERSION 3.20)

project(ToolchainCheck LANGUAGES CXX)

add_executable(
    toolchain_check
    labs/w01/toolchain.cpp
)

target_compile_features(
    toolchain_check
    PRIVATE
    cxx_std_17
)

enable_testing()

add_test(
    NAME toolchain_check
    COMMAND toolchain_check
)

add_executable(
    lifetime
    labs/w01/lifetime.cpp
)

target_compile_features(
   lifetime
   PRIVATE
   cxx_std_17
)

add_test(
    NAME lifetime
    COMMAND lifetime
)

add_executable(
    pointers
    labs/w01/pointers.cpp
)

target_compile_features(
    pointers
    PRIVATE
    cxx_std_17
)

add_test(
    NAME pointers
    COMMAND pointers
)
