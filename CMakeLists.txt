# CMAKE MINIMAL VERSION
cmake_minimum_required (VERSION 3.11)

# PROJECT PROPERTIES
project ("Integration" LANGUAGES CXX)

# EXECUTABLE FILE PROPERTIES
set(MAIN                  "main")
set(GNUPLOT_CALL          "gnuplot_call")
set(TEST                  "test")
set(LONG_TIME_INTEGRATION "long_time_integration")

# непонятно, зачем?
set(HEADERS
    include/lib.h
    include/catch.hpp
)

# src files
set(SRC_MAIN src/main.c)
set(SRC_LONG_TIME_INTEGRATION src/long_time_integration.c)
set(SRC_GNUPLOT_CALL src/gnuplot_call.c)
set(SRC_LIB src/lib.c)
set(SRC_TEST tests/test.cpp)

# add_executable(${MAIN} ${SRC_MAIN} ${SRC_LIB})
# add_executable(${GNUPLOT_CALL} ${SRC_GNUPLOT_CALL})
# add_executable(${SRC_LONG_TIME_INTEGRATION} ${SRC_MAIN} ${SRC_LIB})

add_executable(${TEST} ${SRC_TEST})

# target_include_directories(${MAIN}                  PRIVATE include src)
target_include_directories(${TEST}                  PRIVATE include tests)
# target_include_directories(${LONG_TIME_INTEGRATION} PRIVATE include src)

add_compile_options(
    -Werror

    -Wall
    -Wextra
    -Wpedantic

    -Wnarrowing 
    -Wwrite-strings 
    -Wcast-qual 
    -Wundef 
    -Wstrict-prototypes 
    -Wbad-function-cast
)