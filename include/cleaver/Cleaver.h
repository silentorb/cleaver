#pragma once

#define _SDL_main_h

#include <memory>
#include "lookinglass/Glass.h"
#include <vector>
#include <map>

namespace cleaver {

  class Cleaver;
  using namespace lookinglass;

  typedef void(*Element_Delegate)(Cleaver *cleaver, const Element &element);

  struct Hall;

  class Cleaver : public Glass {

      std::vector<Element_Pointer> elements;

  public:
      Cleaver();
      ~Cleaver();

      void start();
      void stop();
      int update();
      void add_element(const Element_Pointer &element);
      void delete_element(const Element_Pointer &element);

      std::map<const Element*, std::string> element_map;
      std::map<Element_Type, Element_Delegate> barracks_functions;
      Hall *hall;

      unsigned long camera_index;
  };

}

