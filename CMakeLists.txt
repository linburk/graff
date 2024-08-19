cmake_minimum_required(VERSION 3.22)

project(graff LANGUAGES C)
set(C_STANDARD 11)

find_package(SDL2 REQUIRED)
include_directories(graff ${SDL2_INCLUDE_DIRS})

add_executable(graff 
    src/main.c
    src/parser.c
    src/evaluate_rpn.c
    src/interface/tui.c
    src/interface/gui.c 
)
target_link_libraries(graff ${SDL2_LIBRARIES})
set_target_properties(graff PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin")