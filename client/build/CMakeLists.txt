
cmake_minimum_required(VERSION 2.6)
project(bomba)

set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR}/..)
set(CMAKE_MODULE_PATH ${PROJECT_ROOT_DIR}/cmake)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)

find_package(Irrlicht)

add_subdirectory(${PROJECT_ROOT_DIR}/src CMakeFiles/src)

add_custom_target(clean-gen
    rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake Makefile
  )

