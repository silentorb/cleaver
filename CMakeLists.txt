cmake_minimum_required(VERSION 3.2)
project(cleaver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(cleaver SHARED Cleaver.cpp Cleaver.h)

include_directories(
    ../lookinglass
    ../../ogre/build/include
    ../../ogre/OgreMain/include
    ../../ogre/ogredeps/include
)

target_link_libraries(cleaver c:/dev/ogre/build/lib/libOgreMain.dll.a)
target_link_libraries(cleaver c:/dev/ogre/ogredeps/lib/libSDL2.dll.a)

#set(output_dir ${PROJECT_SOURCE_DIR}/bin)
#
#set_target_properties(cleaver PROPERTIES
#    ARCHIVE_OUTPUT_DIRECTORY "${output_dir}"
#    LIBRARY_OUTPUT_DIRECTORY "${output_dir}"
#    RUNTIME_OUTPUT_DIRECTORY "${output_dir}"
#)

add_custom_command(TARGET cleaver POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy_directory
   ../../ogre/ogredeps/bin/Debug $<TARGET_FILE_DIR:cleaver>
)