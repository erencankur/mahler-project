cmake_minimum_required(VERSION 3.20)
project(MahlerProject VERSION 0.1.0 LANGUAGES CXX)

option(MAHLER_ENABLE_SANITIZERS "Enable address and undefined-behavior sanitizers" OFF)

add_library(mahler_core src/sequence.cpp src/early.cpp src/reconstruct.cpp)
target_include_directories(mahler_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_features(mahler_core PUBLIC cxx_std_20)
set_target_properties(mahler_core PROPERTIES CXX_EXTENSIONS OFF)

add_executable(mahler app/main.cpp app/batch.cpp app/analysis.cpp)
target_link_libraries(mahler PRIVATE mahler_core)
target_compile_definitions(mahler PRIVATE
    MAHLER_VERSION="${PROJECT_VERSION}-dev"
    MAHLER_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}"
    MAHLER_COMPILER_VERSION="${CMAKE_CXX_COMPILER_VERSION}"
    MAHLER_BUILD_TYPE="$<CONFIG>"
    MAHLER_SANITIZERS_ENABLED=$<BOOL:${MAHLER_ENABLE_SANITIZERS}>)
set_target_properties(mahler PROPERTIES CXX_EXTENSIONS OFF)

function(mahler_configure_target target)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
        if(MAHLER_ENABLE_SANITIZERS)
            target_compile_options(${target} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
            target_link_options(${target} PRIVATE -fsanitize=address,undefined)
        endif()
    elseif(MAHLER_ENABLE_SANITIZERS)
        message(FATAL_ERROR "Sanitizers currently require Clang or GCC")
    endif()
endfunction()

mahler_configure_target(mahler_core)
mahler_configure_target(mahler)

include(CTest)
if(BUILD_TESTING)
    add_executable(mahler_core_tests tests/sequence_tests.cpp)
    target_link_libraries(mahler_core_tests PRIVATE mahler_core)
    set_target_properties(mahler_core_tests PROPERTIES CXX_EXTENSIONS OFF)
    mahler_configure_target(mahler_core_tests)
    add_test(NAME core COMMAND mahler_core_tests)
    add_executable(mahler_early_tests tests/early_tests.cpp)
    target_link_libraries(mahler_early_tests PRIVATE mahler_core)
    set_target_properties(mahler_early_tests PROPERTIES CXX_EXTENSIONS OFF)
    mahler_configure_target(mahler_early_tests)
    add_test(NAME early COMMAND mahler_early_tests)
    add_test(NAME cli COMMAND "${CMAKE_COMMAND}"
        "-DMAHLER=$<TARGET_FILE:mahler>"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/cli_tests.cmake")
    add_test(NAME batch COMMAND "${CMAKE_COMMAND}"
        "-DMAHLER=$<TARGET_FILE:mahler>"
        "-DTEST_OUTPUT=${CMAKE_CURRENT_BINARY_DIR}/batch-test-output"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/tests/batch_tests.cmake")
endif()
