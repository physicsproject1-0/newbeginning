cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

project(Progettoinfo)

add_executable(grafico_sfml main.cpp plot.cpp newSIR.cpp)

find_package(Boost 1.71 COMPONENTS system filesystem iostreams REQUIRED)

target_compile_options(grafico_sfml PRIVATE -Wall -Wextra -fsanitize=address)

# se metto -fsanitize=address fa una marea di errori

target_link_libraries(grafico_sfml PRIVATE boost_system boost_filesystem boost_iostreams)
target_link_options(grafico_sfml PRIVATE -fsanitize=address)
