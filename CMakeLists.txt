cmake_minimum_required(VERSION 3.10)
project(MiJuego)

# Usa C++17
set(CMAKE_CXX_STANDARD 17)

# Agrega la ruta de raylib instalada
set(raylib_DIR "C:/Users/aaron/Desktop/raylib/install/lib/cmake/raylib")
find_package(raylib REQUIRED)

file(GLOB SOURCES "*.cpp")
add_executable(MiJuego ${SOURCES})


target_link_libraries(MiJuego raylib winmm)
