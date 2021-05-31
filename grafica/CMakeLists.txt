cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

project(Progettoinfografica)

add_executable(grafica mainfake.cpp finestra.cpp classe.cpp)
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(SFML 2.5 COMPONENTS graphics system window REQUIRED)

target_compile_options(grafica PRIVATE -Wall -Wextra -fsanitize=address)

# se metto -fsanitize=address fa una marea di errori

target_link_libraries(grafica PRIVATE sfml-graphics sfml-system sfml-window)
target_link_options(grafica PRIVATE -fsanitize=address)
