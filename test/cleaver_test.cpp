#include "cleaver/Cleaver.h"
#include <iostream>
#include "lookinglass/Glass.h"

using namespace lookinglass;

int main(int argc, char *const argv[]) {
  cleaver::Cleaver glass = cleaver::Cleaver();
  glass.start();
//  glass.ad();
//  cleaver::wow();
  std::shared_ptr<Element> camera (new Element(Element_Type::camera));
  glass.add_element(camera);

  std::shared_ptr<Element> model (new Element(Element_Type::model));
  glass.add_element(model);

  while (!glass.update());

  glass.stop();

  return 0;
}
