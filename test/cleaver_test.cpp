#include "cleaver/Cleaver.h"
#include <iostream>
#include "lookinglass/Glass.h"

int main(int argc, char *const argv[]) {
  cleaver::Cleaver glass = cleaver::Cleaver();
  glass.start();
  while (glass.update() == 0);
  glass.stop();

  return 0;
}
