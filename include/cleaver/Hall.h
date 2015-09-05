#pragma once
//#include "Ogre.h"

#include "OgreRenderWindow.h"
#include "OgreSceneManager.h"
#include "OgreRoot.h"
#include "OgreViewport.h"
#include "SDL2/SDL.h"

namespace cleaver {

  class Hall {
  public:
      SDL_Window *sdl_window;
      Ogre::RenderWindow *ogre_window;
      Ogre::Root *root;
      Ogre::SceneManager *scene_manager;
      Ogre::Viewport *viewport;
  };

  Ogre::MeshPtr createColourCube();

}