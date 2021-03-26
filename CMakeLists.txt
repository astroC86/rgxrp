cmake_minimum_required(VERSION 3.17)
project(rgxrp)

set(CMAKE_CXX_STANDARD 11)

SET(INCLUDE_DEBUG_SYMBOLS "-fprofile-arcs -ftest-coverage")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${INCLUDE_DEBUG_SYMBOLS}")


add_executable(rgxrp main.cpp src/arg_hdlr.cpp src/utils.cpp src/Hndlr_fns.cpp)

add_custom_command(
        TARGET rgxrp
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:rgxrp> ${CMAKE_CURRENT_BINARY_DIR}/../../../tools/)