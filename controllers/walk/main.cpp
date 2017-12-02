// Description:   Manage the entree point function

#include "Walk.hpp"
#include <cstdlib>

using namespace webots;

int main()
{ 
  Walk *controller = new Walk();
  controller->run();
  controller->textToSpeechGreeting();
  controller->runExerciseOne();
  controller->runExerciseTwo();
  controller->runExerciseThree();
  controller->textToSpeechEnding();
  delete controller;
  
  return EXIT_FAILURE;

}


