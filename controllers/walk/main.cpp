// Description:   Manage the entree point function

#include "Walk.hpp"
#include <cstdlib>

using namespace webots;

int main()
{
  
  
  Walk *controller = new Walk();
  controller->run();
  delete controller;
  
  return EXIT_FAILURE;
  
  //MotionPlayer *controller = new MotionPlayer();
  //controller->textToSpeech();
  //controller->run();
  //delete controller;
  //return EXIT_FAILURE;
}


