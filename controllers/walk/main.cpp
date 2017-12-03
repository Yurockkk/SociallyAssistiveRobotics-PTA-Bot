// Description:   Manage the entree point function

#include "Walk.hpp"
#include <cstdlib>

using namespace webots;

int main()
{ 
  int fd = -1;
  int fd2 = -1;
  int n = -1;
  int resultFromCNN = -1;
  char command[] = "CNN";
  
  Walk *controller = new Walk();
  controller->run(fd,fd2,n);
  controller->textToSpeechGreeting();
  resultFromCNN = controller->runExerciseOne(n,command,fd);
  std::cout << "Result from CNN " << resultFromCNN << std::endl;

  //controller->runExerciseTwo();
  //controller->runExerciseThree();
  //controller->textToSpeechEnding();
  delete controller;
  
  return EXIT_FAILURE;

}


