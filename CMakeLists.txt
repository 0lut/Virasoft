cmake_minimum_required(VERSION 2.8)
project(Virasoft)
find_package( OpenCV REQUIRED )
find_package(Armadillo REQUIRED) # <<<
add_executable(${PROJECT_NAME} "main.cpp")
include_directories(${ARMADILLO_INCLUDE_DIRS}) # <<<
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
target_link_libraries( ${PROJECT_NAME} ${ARMADILLO_LIBRARIES}) #<<<Armadillo
file(GLOB HEADERS src/*.hpp src/*.h)
file(GLOB SOURCES src/*.cpp)
add_library( Libraries SHARED ${HEADERS} ${SOURCES})

set(CMAKE_C_FLAGS_RELEASE "-O3")
