cmake_minimum_required(VERSION 3.2)
project(cleaver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(cleaver SHARED
#  ../lib/nvwa/debug_new.cpp
  source/Cleaver.cpp
  source/Barracks.cpp
)

include(cleaver-config.cmake)

include_directories(
  ${cleaver_includes}
)

if (CMAKE_BUILD_TYPE STREQUAL Debug)
  set(ogre_lib c:/dev/ogre32/buildd/lib/libOgreMain_d.dll.a)
  set(ogre_dlls buildd/bin)
  set(dep_path Debug)
else()
  set(ogre_lib c:/dev/ogre32/build/lib/libOgreMain.dll.a)
  set(ogre_dlls build/bin)
  set(dep_path Release)
endif ()

set(cleaver_libraries
  c:/dev/ogre32/ogredeps/lib/libSDL2.dll.a
  ${ogre_lib}
)

target_link_libraries(cleaver ${cleaver_libraries})

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