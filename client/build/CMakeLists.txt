
cmake_minimum_required(VERSION 2.6)
project(bomba)

set(PROJECT_ROOT_DIR ${PROJECT_SOURCE_DIR}/..)
set(CMAKE_MODULE_PATH ${PROJECT_ROOT_DIR}/cmake)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_ROOT_DIR}/bin)

find_package(Irrlicht REQUIRED)
find_package(Protobuf REQUIRED)
find_package(GTest)

include(Utils)

configure_file(config.h.in ${PROJECT_ROOT_DIR}/src/config.h)

add_custom_target(clean-gen
    rm -rf CMakeFiles CMakeCache.txt cmake_install.cmake Makefile 
    ${PROJECT_ROOT_DIR}/bin ${PROJECT_ROOT_DIR}/src/config.h
  )

add_custom_target(unittest)
add_custom_command(TARGET unittest POST_BUILD
    COMMAND ${PROJECT_ROOT_DIR}/util/unittest.sh ${PROJECT_ROOT_DIR}/bin
  )

add_subdirectory(${PROJECT_ROOT_DIR}/src CMakeFiles/src)

