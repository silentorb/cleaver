include_directories(
  ${PROJECT_SOURCE_DIR}/../lookinglass/include
  ${PROJECT_SOURCE_DIR}/../../ogre32/buildd/include
  ${PROJECT_SOURCE_DIR}/../../ogre32/OgreMain/include
  ${PROJECT_SOURCE_DIR}/../../ogre32/ogredeps/include
  ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${current_target} c:/dev/ogre32/ogredeps/lib/libSDL2.dll.a)



message("current_target ${current_target}")

#message("CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")
#message("CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR}")
#message("PROJECT_SOURCE_DIR  ${PROJECT_SOURCE_DIR}")

#message("CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")