# CMAKE MINIMAL VERSION
cmake_minimum_required (VERSION 3.11)

# PROJECT PROPERTIES
project ("Integration" LANGUAGES C CXX)

# THE 17TH STANDART
set(CMAKE_CXX_STANDARD 17)

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

# SRC FILES
set(SRC_MAIN src/main.c)
set(SRC_LONG_TIME_INTEGRATION src/long_time_integrate.c)
set(SRC_GNUPLOT_CALL src/gnuplot_call.c)
set(SRC_TEST tests/test.cpp)
set(SRC_LIB src/lib.c)

# EXECUTABLES
add_executable(${MAIN} ${SRC_MAIN} ${SRC_LIB})
add_executable(${GNUPLOT_CALL} ${SRC_GNUPLOT_CALL})
add_executable(${LONG_TIME_INTEGRATION} ${SRC_LONG_TIME_INTEGRATION} ${SRC_LIB})

add_executable(${TEST} ${SRC_TEST})

# TARGETS LINK LIBRARIES
target_link_libraries(${MAIN} "dl" "pthread" "m" ${CMAKE_DL_LIBS})
target_link_libraries(${GNUPLOT_CALL} "dl" ${CMAKE_DL_LIBS})
target_link_libraries(${LONG_TIME_INTEGRATION} "dl" ${CMAKE_DL_LIBS})

# TARGETS INCLUDE DERICTORIES
target_include_directories(${MAIN}                  PRIVATE include src)
target_include_directories(${TEST}                  PRIVATE include tests)
target_include_directories(${LONG_TIME_INTEGRATION} PRIVATE include src)

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