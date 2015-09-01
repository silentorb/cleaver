#pragma once

#define _SDL_main_h

#include <memory>
#include "Ogre.h"
#include "lookinglass/Glass.h"
#include "SDL2/SDL.h"

namespace cleaver {

  class Cleaver : public lookinglass::Glass {

      Ogre::Root* root;
      SDL_Window *sdl_window;
      Ogre::RenderWindow *ogre_window;
      Ogre::Viewport* viewport;

  public:
      Cleaver();
      ~Cleaver();

      virtual void stop();
      virtual void start();
      int update ();
      void add_element(lookinglass::Element element);
      void delete_element(lookinglass::Element element);

      Ogre::MeshPtr createColourCube();
  };

}

