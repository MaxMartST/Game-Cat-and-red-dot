cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(sfml-lab-3)

set(SFML_STATIC_LIBRARIES TRUE)

find_package(Freetype REQUIRED)
find_package(JPEG REQUIRED)
find_package(SFML 2 COMPONENTS window graphics system REQUIRED)

add_subdirectory(03)