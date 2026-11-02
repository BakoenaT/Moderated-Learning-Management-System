cmake_minimum_required(VERSION 3.20)
project(ModeratedLMS VERSION 1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(BUILD_TESTING "Build Catch2 unit and integration tests" ON)
option(ENABLE_COVERAGE "Instrument the business-logic layer for GCC coverage" OFF)

find_package(SQLite3 REQUIRED)
find_package(OpenSSL REQUIRED)

add_library(lms_business_rules src/business_rules.cpp)
target_include_directories(lms_business_rules PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(lms_business_rules PUBLIC SQLite::SQLite3)

add_executable(moderated_lms src/main.cpp)
target_link_libraries(moderated_lms PRIVATE lms_business_rules SQLite::SQLite3 OpenSSL::Crypto)

if(MSVC)
    target_compile_options(lms_business_rules PRIVATE /W4)
    target_compile_options(moderated_lms PRIVATE /W4)
else()
    target_compile_options(lms_business_rules PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(moderated_lms PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(lms_business_rules PRIVATE --coverage -O0 -g)
    target_link_options(moderated_lms PRIVATE --coverage)
endif()

include(CTest)
if(BUILD_TESTING)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.7.1
        GIT_SHALLOW TRUE
    )
    FetchContent_MakeAvailable(Catch2)
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

    add_executable(lms_tests tests/test_business_rules.cpp)
    target_link_libraries(lms_tests PRIVATE lms_business_rules Catch2::Catch2WithMain SQLite::SQLite3)
    if(MSVC)
        target_compile_options(lms_tests PRIVATE /W4)
    else()
        target_compile_options(lms_tests PRIVATE -Wall -Wextra -Wpedantic)
    endif()
    if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        target_link_options(lms_tests PRIVATE --coverage)
    endif()
    include(Catch)
    catch_discover_tests(lms_tests)
endif()
