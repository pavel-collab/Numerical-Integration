# CMAKE MINIMAL VERSION
cmake_minimum_required (VERSION 3.11)

# PROJECT PROPERTIES
project ("Integration" LANGUAGES C CXX)

# THE 17TH STANDART
set(CMAKE_CXX_STANDARD 17)

# EXECUTABLE FILE PROPERTIES
set(MAIN                  "main")
set(GNUPLOT_CALL          "gnuplot_call")
set(LONG_TIME_INTEGRATION "long_time_integration")

set(HEADERS
    include/lib.h
)

# SRC FILES
set(SRC_MAIN src/main.c)
set(SRC_LONG_TIME_INTEGRATION src/long_time_integrate.c)
set(SRC_GNUPLOT_CALL src/gnuplot_call.c)
set(SRC_GTEST tests/gtest.cpp)

# EXECUTABLES
add_executable(${MAIN}                  ${SRC_MAIN})
add_executable(${GNUPLOT_CALL}          ${SRC_GNUPLOT_CALL})
add_executable(${LONG_TIME_INTEGRATION} ${SRC_LONG_TIME_INTEGRATION})

# TARGETS LINK LIBRARIES
target_link_libraries(${MAIN} "dl" "pthread" "m"    ${CMAKE_DL_LIBS})
target_link_libraries(${GNUPLOT_CALL} "dl"          ${CMAKE_DL_LIBS})
target_link_libraries(${LONG_TIME_INTEGRATION} "dl" ${CMAKE_DL_LIBS})

################################
# Unit Tests
################################
# Add test cpp file
add_executable(runUnitTests ${SRC_GTEST} ${SRC_LIB})
# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main pthread)
add_test(runUnitTests runUnitTests)

# TARGETS INCLUDE DERICTORIES
target_include_directories(${MAIN}                  PRIVATE include src)
target_include_directories(${LONG_TIME_INTEGRATION} PRIVATE include src)
target_include_directories(runUnitTests             PRIVATE include src)

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