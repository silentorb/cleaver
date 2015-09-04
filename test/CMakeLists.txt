cmake_minimum_required(VERSION 3.2)
project(cleaver_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_executable(cleaver_test cleaver_test.cpp
#  ../../lib/nvwa/debug_new.cpp
)

#set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static")

include_directories(
  ../../lib/Catch/include
)

set(current_target cleaver_test)
include(../cleaver-config.cmake)

target_link_libraries(cleaver_test
  mingw32
  ${CMAKE_CURRENT_SOURCE_DIR}/../build/libcleaver.dll.a
)
