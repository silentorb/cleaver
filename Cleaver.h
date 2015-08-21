#pragma once

#include <memory>
#include "Ogre.h"
#include "lookinglass/Glass.h"
#include "SDL2/SDL.h"

namespace cleaver {

  class Cleaver : public lookinglass::Glass {

    std::unique_ptr<Ogre::Root> root;
    SDL_Window *sdl_window;
    Ogre::RenderWindow* ogre_window;

    virtual void initialize();
  };

}

