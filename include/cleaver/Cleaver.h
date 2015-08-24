#pragma once

#define _SDL_main_h

#include <memory>
#include "Ogre.h"
#include "lookinglass/glass.h"
#include "SDL2/SDL.h"

namespace cleaver {

  class Cleaver : public lookinglass::Glass {

      Ogre::Root* root;
      SDL_Window *sdl_window;
      Ogre::RenderWindow *ogre_window;

  public:
      Cleaver();
      ~Cleaver();

      virtual void stop();
      virtual void start();
  };

}

