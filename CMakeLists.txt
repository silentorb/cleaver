cmake_minimum_required(VERSION 3.2)
project(gauntlet)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_library(gauntlet SHARED ${SOURCE_FILES} Gauntlet.cpp Gauntlet.h)

include_directories(
    ../ogre/build/include
    ../ogre/OgreMain/include
)

target_link_libraries(lookinglass c:/dev/ogre/build/lib/libOgreMain.dll.a)