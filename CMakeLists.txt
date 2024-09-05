cmake_minimum_required(VERSION 3.22)

project(graff LANGUAGES C)
set(C_STANDARD 11)

add_executable(graff 
    src/main.c
    src/parser.c
    src/evaluate_rpn.c
    src/interface/tui.c
    src/interface/gui.c 
)
find_package(PkgConfig REQUIRED)

pkg_check_modules(SDL2 REQUIRED SDL2)
target_include_directories(graff PUBLIC ${SDL2_INCLUDE_DIRS})
target_link_libraries(graff ${SDL2_LIBRARY})

pkg_check_modules(SDL2_ttf REQUIRED SDL2_TTF)
target_include_directories(graff PRIVATE ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(graff ${SDL2_TTF_LIBRARIES})

set_target_properties(graff PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../bin")