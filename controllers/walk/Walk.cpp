/*
When running this controller in the real robot
do not interface via SSh client (i,e, PuTTY)
The result will be on a segmentation fault error.

Instead interface with the robot via remote desktop
(i.e. VNC). Open a terminal window, compile controller
and run.
*/
#include "Walk.hpp"
#include <webots/LED.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>
#include <RobotisOp2MotionManager.hpp>
#include <webots/utils/Motion.hpp>
#include <webots/Speaker.hpp>
#include <webots/Keyboard.hpp>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

//#include <cmath>

#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define SOCKET_PORT1 10026
#define SOCKET_PORT2 10020

#define SOCKET_SERVER1 "192.168.1.4"   /*  */
#define SOCKET_SERVER2 "127.0.0.1"   /* local host */



using namespace webots;
using namespace managers;
using namespace std;

static const char *motorNames[NMOTORS] = {
  "ShoulderR" /*ID1 */, "ShoulderL" /*ID2 */, "ArmUpperR" /*ID3 */, "ArmUpperL" /*ID4 */,
  "ArmLowerR" /*ID5 */, "ArmLowerL" /*ID6 */, "PelvYR"    /*ID7 */, "PelvYL"    /*ID8 */,
  "PelvR"     /*ID9 */, "PelvL"     /*ID10*/, "LegUpperR" /*ID11*/, "LegUpperL" /*ID12*/,
  "LegLowerR" /*ID13*/, "LegLowerL" /*ID14*/, "AnkleR"    /*ID15*/, "AnkleL"    /*ID16*/,
  "FootR"     /*ID17*/, "FootL"     /*ID18*/, "Neck"      /*ID19*/, "Head"      /*ID20*/
};

//Constructor
Walk::Walk():
    Robot()
{
  //Get time step
  mTimeStep = getBasicTimeStep();
 
  getLED("HeadLed")->set(0xFF0000);
  getLED("EyeLed")->set(0x00FF00);
  mSpeaker = getSpeaker("Speaker");
  mSpeaker->setLanguage("en-US");
  
  for (int i=0; i<NMOTORS; i++) {
   mMotors[i] = getMotor(motorNames[i]);
   string sensorName = motorNames[i];
   sensorName.push_back('S');
   mPositionSensors[i] = getPositionSensor(sensorName);
   mPositionSensors[i]->enable(mTimeStep);
  }
  mKeyboard = getKeyboard();
  mKeyboard->enable(mTimeStep);
  
  mMotionManager = new RobotisOp2MotionManager(this);
}

//Destructor
Walk::~Walk() {
}

//Step function
void Walk::myStep() {
  int ret = step(mTimeStep);
  if (ret == -1)
    exit(EXIT_SUCCESS);
}

//Wait function
void Walk::wait(int ms) {
  double startTime = getTime();
  double s = (double) ms / 1000.0;
  while (s + startTime >= getTime())
    myStep();
}

int Walk::communicateWithServer(int g, int currentLevel, int fd, int fd2) {
  printf("in communicateWithServer, g=%d, fd=%d\n",g,fd);
  char commandCNN[10];
  char commandKinect[10];
  
  switch(g){
    case 1:
      commandKinect[0] = '1';
      commandKinect[1] = ',';
      break;
    
    case 2:
      commandKinect[0] = '2';
      commandKinect[1] = ',';
      break;
      
    case 3:
      commandKinect[0] = '3';
      commandKinect[1] = ',';
      break;
  }
    switch(currentLevel){
      case 0:  strncpy(commandCNN, "8", sizeof(commandCNN) - 1);
               //strncpy(commandKinect, "9", sizeof(commandKinect) - 1);
               commandKinect[2] = '3';
               break;
      case 1:  strncpy(commandCNN, "14", sizeof(commandCNN) - 1);
               //strncpy(commandKinect, "15", sizeof(commandKinect) - 1);
               commandKinect[2] = '5';
               break;
      case 2:  strncpy(commandCNN, "23", sizeof(commandCNN) - 1);
               //strncpy(commandKinect, "24", sizeof(commandKinect) - 1);
               commandKinect[2] = '8';
               
               break;
    }
    commandCNN[sizeof(commandCNN) - 1] = 0;
    commandKinect[3] = 0;
    
  int n = -1;
  int n2 = -1;      
  n = strlen(commandCNN);
  n2 = strlen(commandKinect);
  printf("commandCNN:%s\n",commandCNN);
   printf("commandKinect:%s\n",commandKinect);

  n = send(fd, commandCNN, n, 0);
  n2 = send(fd2,commandKinect,n2,0);

    wait(3000);
    mSpeaker->speak("You can do it!",1.0);
    printf("in communicateWithServer while loop\n");
    mSpeaker->speak("Keep Going!",1.0);
    wait(3000);
    mSpeaker->speak("Keep Going!",1.0);
    
    n = recv(fd, commandCNN, 10, 0);
    n2 = recv(fd2, commandKinect, 10, 0);


  
  //n = recv(fd, buffer, 256, 0);
  commandCNN[n] = '\0';
  commandKinect[n] = '\0';

  printf("Answer from CNN is: %s\n", commandCNN);
  int resultFromCNN = atoi(commandCNN);
  printf("Answer from Kinect is: %s\n", commandKinect);
  int resultFromKinect = atoi(commandKinect);
  printf("resultFromCNN is: %d\n", resultFromCNN);
  printf("resultFromKinect is: %d\n", resultFromKinect);
  return resultFromCNN;
}

int initClient(int &fd, int SOCKET_PORT, bool isSocket1){
  struct sockaddr_in address;
  struct hostent *server;
  int rc;
  //int n;
  //char buffer[256];
  /* create the socket */
  printf("initClient\n");
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        printf("cannot create socket\n");

        return -1;
    }
    
    /* fill in the socket address */
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons(SOCKET_PORT);
    char SOCKET_SERVER[16];
    if(isSocket1){
      server = gethostbyname(SOCKET_SERVER1);
      strcpy(SOCKET_SERVER,SOCKET_SERVER1);
    }else{
      server = gethostbyname(SOCKET_SERVER2);
      strcpy(SOCKET_SERVER,SOCKET_SERVER2);
    }

    if (server) {
        memcpy((char *) &address.sin_addr.s_addr, (char *) server->h_addr,
               server->h_length);
    } else {
        printf("cannot resolve server name: %s\n", SOCKET_SERVER);

        return -1;
    }

    /* connect to the server */
    rc = connect(fd, (struct sockaddr *) &address, sizeof(struct sockaddr));
    if (rc == -1) {
        printf("cannot connect to the server\n");

        return -1;
    }
    //TODO when to trigger
    return 1;
}



// function containing the main feedback loop
void Walk::run(int &fd, int &fd2,int &n) {

  // First step to update sensors values
  myStep();
  bool isSocket1Established = false;
  bool isSocket2Established = false;
  if(initClient(fd,SOCKET_PORT1, true) == -1){
    printf("socket1 fail\n");
  }else{
    printf("socket1 success\n");
    isSocket1Established = true;
  }
  if(initClient(fd2,SOCKET_PORT2, false) == -1){
    printf("socket2 fail\n");
  }else{
    printf("socket2 success\n");
    isSocket2Established = true;
  }

    //mGaitManager->step(mTimeStep);

    // step
    myStep();
  }


void Walk::runExerciseOne(int &g, char buffer[5], int fd) {
  g = 1;
  printf("in runExerciseOne, g=%d, fd=%d",g,fd);
  cout << "-------MotionPlayer first exercise of ROBOTIS OP2-------" << endl;
  cout << "This exercise plays a Webots hand_extend.motion file" << endl;

  // step
  myStep();

    Motion motion_1("hand_extend.motion");
    int time1 = motion_1.getDuration();
    for (int i=0; i<2; i++){
    	motion_1.play();
    	wait(time1);
    }
    
    mSpeaker->speak("Please start now.",1.0);
    wait(1000);
    char command[] = "10";
    //return communicateWithServer(n,command,fd);
   
}

void Walk::runExerciseTwo(int &g, int currentLevel, int fd) {
  g = 2;
  printf("in runExerciseTwo, currentLevel=%d",currentLevel);
  cout << "-------MotionPlayer second exercise of ROBOTIS OP2-------" << endl;
  cout << "This exercise plays a Webots hand_high.motion file" << endl;

  // step
  myStep();
  
    char command[10];
    switch(currentLevel){
      case 0:  strncpy(command, "5", sizeof(command) - 1);
               mSpeaker->speak("Let’s start with our second exercise. Please follow the demonstration and repeat three times.",1.0);
               wait(6000); 
               break;
      case 1:  strncpy(command, "10", sizeof(command) - 1);
               mSpeaker->speak("Let’s start with our second exercise. Please follow the demonstration and repeat five times.",1.0);
               wait(6000); 
               break;
      case 2:  strncpy(command, "15", sizeof(command) - 1);
               mSpeaker->speak("Let’s start with our second exercise. Please follow the demonstration and repeat eight times.",1.0);
               wait(6000); 
               break;
    }
    command[sizeof(command) - 1] = 0;
  
    
    Motion motion_2("hand_high.motion");
    int time2 = motion_2.getDuration();
    for (int i=0; i<2; i++){
    	motion_2.play();
    	wait(time2);
    }
    
    mSpeaker->speak("Please start now.",1.0);
    wait(2000); 
    
    //return communicateWithServer(n,command,fd);
}


void Walk::runExerciseThree(int &g, int currentLevel, int fd) {
  g = 3;
  printf("in runExerciseThree, currentLevel=%d",currentLevel);
  cout << "-------MotionPlayer last exercise of ROBOTIS OP2-------" << endl;
  cout << "This exercise plays blend knee motion" << endl;

  // step
  myStep();
  
  char command[10];
    switch(currentLevel){
      case 0:  strncpy(command, "5", sizeof(command) - 1);
               mSpeaker->speak("Let’s start with our third exercise. Please follow the demonstration and repeat three times.",1.0);
               wait(6000); 
               break;
      case 1:  strncpy(command, "10", sizeof(command) - 1);
               mSpeaker->speak("Let’s start with our third exercise. Please follow the demonstration and repeat five times.",1.0);
               wait(6000); 
               break;
      case 2:  strncpy(command, "15", sizeof(command) - 1);
               mSpeaker->speak("Let’s start with our third exercise. Please follow the demonstration and repeat eight times.",1.0);
               wait(6000); 
               break;
    }
    command[sizeof(command) - 1] = 0;
  
   /*
    bool motionOneFlag = false;
    bool motionTwoFlag = false;
    bool motionThreeFlag = false;
    */
    
    
    for (int i=0; i<2; i++){
    mMotionManager->playPage(15);
    wait(500);
    mMotionManager->playPage(1);
    }
    
    mSpeaker->speak("Please start now.",1.0);
    wait(2000); 
    
    //return communicateWithServer(n,command,fd);

}
   
void Walk::textToSpeechGreeting() {

  myStep();
  
    cout<<mSpeaker->getLanguage()<<endl;
    mMotionManager->playPage(1);
    //mSpeaker->playSound(mSpeaker,mSpeaker,"eric.wav",1.0,1.0,0,true);
    mSpeaker->speak("Hello, my name is PTABOT. I am here to help you with your cardiac rehab therapy today.",1.0);
    wait(6000);
    mSpeaker->speak("Let’s start with our first exercise. Please follow the demonstration and repeat five times.",1.0);
    wait(6000);    
    //mSpeaker->speak("Thank you for coming. See you next time.",1.0);
//    while(true){
//     myStep();
//    }
/*   
    string text = "how are you";
    string mLanguage = "en";
    fprintf(stderr, "Speaker: Saying \"%s\" ...\n", text.c_str());
    char *buffer = (char *)malloc(strlen(text.c_str())+3);
    sprintf(buffer, "\"%s\"", text.c_str());
    execl("/Users/hannah/Downloads/espeak-1.45.04-OSX/espeak-1.45.04/speak", "espeak", buffer, "-v", mLanguage.c_str(), (char *)NULL);
    //free(buffer);
*/  

}


void Walk::textToSpeechSad() {

  myStep();
  
    mSpeaker->speak("And repeat three times",1.0);

}

void Walk::textToSpeechSoso() {

  myStep();
    
    mSpeaker->speak("And repeat five times",1.0);

}

void Walk::textToSpeechHappy() {

  myStep();
    
    mSpeaker->speak("And repeat eight times",1.0);

}

void Walk::textToSpeechSadEncouragement() {

    myStep();
    wait(5000);
    mSpeaker->speak("You can do it!",1.0);
    mSpeaker->speak("Push through.",1.0);
    wait(5000);
    mSpeaker->speak("This will soon be over.",1.0);
    wait(5000);
    mSpeaker->speak("You’re almost done.",1.0);
    wait(5000);

}

void Walk::textToSpeechHappyEncouragement() {

  myStep();

    mSpeaker->speak("Good Job!",1.0);
    wait(5000);
    mSpeaker->speak("Keep Going!",1.0);
    wait(5000);
    mSpeaker->speak("You’re doing well.",1.0);
    wait(5000);

}


void Walk::textToSpeechEnding() {

  myStep();
 
    mSpeaker->speak("Thank you for coming. You did a great job today. See you next time.",1.0);
}

void Walk::getUpdatedLevel(int &currentLevel,int &resultFromCNN){
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
}

void Walk::textToSpeechEncourage(){
      mSpeaker->speak("Good Job!",1.0);
      wait(1000);
}