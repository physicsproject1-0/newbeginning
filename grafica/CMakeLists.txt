cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

project(Progettoinfografica)

add_executable(grafica.out main.cpp finestra.cpp mondo.cpp gui.cpp automa.cpp animazione.cpp pennello.cpp riquadro_informazioni.cpp bordi.cpp)
set (EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(SFML 2.5 COMPONENTS graphics system window REQUIRED)

target_compile_options(grafica.out PRIVATE -Wall -Wextra -fsanitize=address)

# se metto -fsanitize=address fa una marea di errori

target_link_libraries(grafica.out PRIVATE sfml-graphics sfml-system sfml-window)
target_link_options(grafica.out PRIVATE -fsanitize=address)
