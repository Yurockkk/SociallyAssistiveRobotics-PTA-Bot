// Description:   Manage the entree point function

#include "Walk.hpp"
#include <cstdlib>

using namespace webots;

int main()
{ 
  int fd = -1;
  int fd2 = -1;
  int g = -1;
  int resultFromCNN = -1;
  char command[] = "10";
  int currentLevel = 1;   //0: easy, 1: medium 2: hard
  
  Walk *controller = new Walk();
  controller->run(fd,fd2,g);
  controller->textToSpeechGreeting();
  controller->runExerciseOne(g,command,fd);
  resultFromCNN = controller->communicateWithServer(g,currentLevel,fd,fd2);
  std::cout << "after movement1, Result from CNN " << resultFromCNN << std::endl;
  controller->textToSpeechEncourage();
  controller->getUpdatedLevel(currentLevel,resultFromCNN);
  controller->runExerciseTwo(g,currentLevel,fd);
  resultFromCNN = controller->communicateWithServer(g,currentLevel,fd,fd2);
  std::cout << "after movement2, Result from CNN " << resultFromCNN << std::endl;
  controller->textToSpeechEncourage();
  controller->getUpdatedLevel(currentLevel,resultFromCNN);
  controller->runExerciseThree(g,currentLevel,fd);
  resultFromCNN = controller->communicateWithServer(g,currentLevel,fd,fd2);
  std::cout << "after movement3, Result from CNN " << resultFromCNN << std::endl;
  controller->textToSpeechEnding();
  delete controller;
  
  return EXIT_FAILURE;

}


