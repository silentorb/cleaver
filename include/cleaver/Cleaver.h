#pragma once

#define _SDL_main_h

#include <memory>
#include "Ogre.h"
#include "lookinglass/Glass.h"
#include "SDL2/SDL.h"

namespace cleaver {

  class Cleaver;
  using namespace lookinglass;

  typedef void(*Element_Delegate)(Cleaver *cleaver, const Element &element);

  class Cleaver : public Glass {

      SDL_Window *sdl_window;
      Ogre::RenderWindow *ogre_window;
      std::vector<Element_Pointer> elements;

  public:
      Cleaver();
      ~Cleaver();

      void start();
      void stop();
      int update();
      void add_element(const Element_Pointer &element);
      void delete_element(const Element_Pointer &element);

      Ogre::MeshPtr createColourCube();
      Ogre::Root *root;
      Ogre::SceneManager *scene_manager;
      Ogre::Viewport *viewport;
      std::map<const Element*, std::string> element_map;
      std::map<Element_Type, Element_Delegate> barracks_functions;

      unsigned long camera_index;
  };

}

