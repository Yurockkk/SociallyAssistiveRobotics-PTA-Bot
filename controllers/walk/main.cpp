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
  int currentLevel = 1;   //0 : easy, 1: medium 2: hard
  
  Walk *controller = new Walk();
  controller->run(fd,fd2,n);
  controller->textToSpeechGreeting();
  resultFromCNN = controller->runExerciseOne(n,command,fd);
  std::cout << "Result from CNN " << resultFromCNN << std::endl;
  if(resultFromCNN == 0){
    std::cout << "Result from CNN == 0, let's decrease level if posibble"<< std::endl;
    currentLevel--;
    if(currentLevel < 0){
      currentLevel = 0;
    }
  }else if(resultFromCNN == 1){
    std::cout << "Result from CNN == 1, let's increase level if posibble"<< std::endl;
    currentLevel++;
    if(currentLevel > 2){
      currentLevel = 2;
    }
  }
  

  controller->runExerciseTwo();
  //controller->runExerciseThree();
  //controller->textToSpeechEnding();
  delete controller;
  
  return EXIT_FAILURE;

}


