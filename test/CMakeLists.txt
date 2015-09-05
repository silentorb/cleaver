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

include(../cleaver-config.cmake)

target_link_libraries(cleaver_test
  mingw32
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/libcleaver.dll.a
)
#${CMAKE_CURRENT_SOURCE_DIR}/../../bin/Debug/libcleaver.dll.a

message("LIBRARY_OUTPUT_PATH ${LIBRARY_OUTPUT_PATH}")