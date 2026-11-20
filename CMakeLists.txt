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

# W2: STL word counter
add_executable(
    w2_word_count
    labs/w02/word_count.cpp
)

target_compile_features(
    w2_word_count
    PRIVATE
    cxx_std_17
)
# W2: normal word count test
add_test(
    NAME w2_word_count_normal
    COMMAND w2_word_count
            "${CMAKE_SOURCE_DIR}/data/words.txt"
)

set_tests_properties(
    w2_word_count_normal
    PROPERTIES
    TIMEOUT 15
)

# W2: exact output verification
add_test(
    NAME w2_word_count_normal_exact
    COMMAND bash
        "${CMAKE_SOURCE_DIR}/tests/w02/check_normal.sh"
        "$<TARGET_FILE:w2_word_count>"
        "${CMAKE_SOURCE_DIR}/data/words.txt"
        "${CMAKE_SOURCE_DIR}/tests/w02/normal.expected"
)

set_tests_properties(
    w2_word_count_normal_exact
    PROPERTIES
    TIMEOUT 15
)

# W2: empty file exact output test
add_test(
    NAME w2_word_count_empty_exact
    COMMAND bash
        "${CMAKE_SOURCE_DIR}/tests/w02/check_normal.sh"
        "$<TARGET_FILE:w2_word_count>"
        "${CMAKE_SOURCE_DIR}/data/w2-cpp-1-empty.txt"
        "${CMAKE_SOURCE_DIR}/tests/w02/empty.expected"
)

set_tests_properties(
    w2_word_count_empty_exact
    PROPERTIES
    TIMEOUT 15
)

# W2: missing file error test
add_test(
    NAME w2_word_count_missing
    COMMAND bash
        "${CMAKE_SOURCE_DIR}/tests/w02/check_missing.sh"
        "$<TARGET_FILE:w2_word_count>"
        "${CMAKE_SOURCE_DIR}/data/w2-cpp-1-nonexistent.txt"
)

set_tests_properties(
    w2_word_count_missing
    PROPERTIES
    TIMEOUT 20
)

# W2: case normalization exact output test
add_test(
    NAME w2_word_count_case_exact
    COMMAND bash
        "${CMAKE_SOURCE_DIR}/tests/w02/check_normal.sh"
        "$<TARGET_FILE:w2_word_count>"
        "${CMAKE_SOURCE_DIR}/data/w2-cpp-1-case.txt"
        "${CMAKE_SOURCE_DIR}/tests/w02/case.expected"
)

set_tests_properties(
    w2_word_count_case_exact
    PROPERTIES
    TIMEOUT 15
)
