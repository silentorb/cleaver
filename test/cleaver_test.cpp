//#define CATCH_CONFIG_MAIN
//#define CATCH_CONFIG_RUNNER
//#include "catch.hpp"
#include "cleaver/Cleaver.h"
#include <iostream>
#include <lookinglass/glass.h>

int main( int argc, char* const argv[] )
{
//  int result = Catch::Session().run( argc, argv );
  try {
//    throw std::runtime_error("!!!!!!!!!!!");
    cleaver::Cleaver glass = cleaver::Cleaver();
    glass.start();
    glass.stop();

//    throw "Memory allocation failure!";
  }

  catch (std::exception &e) {
    std::cout << e.what();
  }

  std::cin.get();

  return 0;
//  return result;
}

//TEST_CASE("Test test", "[not sure what to put here]") {
//  cleaver::Cleaver glass = cleaver::Cleaver();
//  glass.start();
//  REQUIRE(1 == 1);
//  glass.stop();
//}