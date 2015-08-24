cmake_minimum_required(VERSION 3.2)
project(cleaver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(cleaver SHARED Cleaver.cpp)

set(current_target cleaver)
include(cleaver.cmake)

#message(STATUS "hello")
#get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
#foreach(dir ${dirs})
#  message(STATUS "dir='${dir}'")
#endforeach()

#target_link_libraries(cleaver c:/dev/ogre/build/lib/libOgreMain.dll.a)
#target_link_libraries(cleaver c:/dev/ogre/ogredeps/lib/libSDL2.dll.a)


target_link_libraries(cleaver c:/dev/ogre32/buildd/lib/libOgreMain_d.dll.a)




if (CMAKE_BUILD_TYPE STREQUAL Debug)
else()
  target_link_libraries(cleaver c:/dev/ogre32/build/lib/libOgreMain.dll.a)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL Debug)
  set(ogre_dlls buildd/bin)
  set(dep_path Debug)
else()
  set(ogre_dlls build/bin)
  set(dep_path Release)
endif ()

add_custom_command(TARGET cleaver POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  c:/dev/ogre32/ogredeps/bin/${dep_path} $<TARGET_FILE_DIR:cleaver>
)

add_custom_command(TARGET cleaver POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  c:/dev/ogre32/${ogre_dlls} $<TARGET_FILE_DIR:cleaver>
)

add_subdirectory(test)

add_dependencies(cleaver_test cleaver)