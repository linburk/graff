cmake_minimum_required(VERSION 3.22)

project(math_analyzer LANGUAGES C)
set(C_STANDARD 11)

add_executable(graph src/main.c src/table.c src/parser.c src/evaluate_rpn.c)

set_target_properties(graph PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                       "${CMAKE_BINARY_DIR}/../bin")
