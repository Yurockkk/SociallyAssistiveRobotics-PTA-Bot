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
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include <cstdio>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

<<<<<<< HEAD
#define SOCKET_PORT1 10020
#define SOCKET_PORT2 10023

#define SOCKET_SERVER1 "127.0.0.1"   /*  */
#define SOCKET_SERVER2 "192.168.1.4"   /* local host */
=======
#define SOCKET_PORT1 10026
#define SOCKET_PORT2 10020

#define SOCKET_SERVER1 "192.168.1.4"   /*  */
#define SOCKET_SERVER2 "127.0.0.1"   /* local host */
>>>>>>> refactoringCode

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

<<<<<<< HEAD
void communicateWithServer(int n, char buffer[5], int fd) {
  printf("in communicateWithServer, n=%d, fd=%d",n,fd);       
  n = strlen(buffer);
  //buffer[n++] = '\n';     /* append carriage return */
  //buffer[n] = '\0';
  n = send(fd, buffer, n, 0);

  if (strncmp(buffer, "exit", 4) == 0) {
    //break;
=======
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
>>>>>>> refactoringCode
  }
  
  switch(currentLevel){
    case 0:  strncpy(commandCNN, "8", sizeof(commandCNN) - 1);
             commandKinect[2] = '3';
             break;
    case 1:  strncpy(commandCNN, "14", sizeof(commandCNN) - 1);
             commandKinect[2] = '5';
             break;
    case 2:  strncpy(commandCNN, "23", sizeof(commandCNN) - 1);
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

  wait(4000);
  mSpeaker->speak("You can do it!",1.0);
  wait(3000);
  mSpeaker->speak("Keep Going!",1.0);
  wait(1000);

  n = recv(fd, commandCNN, 10, 0);
  n2 = recv(fd2, commandKinect, 10, 0);

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
  
  return 1;
}


// function containing the main feedback loop
void Walk::createSocketWithServers(int &fd, int &fd2,int &n) {

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

    // step
    myStep();
  }


<<<<<<< HEAD
  while (true) {
    checkIfFallen();

    mGaitManager->setXAmplitude(0.0);
    mGaitManager->setAAmplitude(0.0);

    // get keyboard key
    while((key = mKeyboard->getKey()) >= 0) {
      switch(key) {
        case ' ' : // Space bar
          if(isWalking) {
            mGaitManager->stop();
            isWalking = false;
            wait(200);
          }
          else {
            mGaitManager->start();
            isWalking = true;
            wait(200);
          }
          break;
        case Keyboard::UP :
          mGaitManager->setXAmplitude(1.0);
          cout << "User clicked up" << endl;

          break;
        case Keyboard::DOWN :
          mGaitManager->setXAmplitude(-1.0);
          cout << "User clicked down" << endl;

          break;
        case Keyboard::RIGHT :
          mGaitManager->setAAmplitude(-0.5);
          cout << "User clicked right" << endl;

          break;
        case Keyboard::LEFT :
          mGaitManager->setAAmplitude(0.5);
          cout << "User clicked left" << endl;

          break;
        case Keyboard::KEY :
          cout << "User clicked KEY" << endl;
          break;
          
        case 49 ://1 key
          cout<<"1 key press"<<endl;
          mSpeaker->speak("Let’s start with our first exercise",1.0);

          for(int i = 0 ; i < 3; i++){
            motion_1.play();
            wait(motion_1.getDuration());
          }
          
          break; 
          
        case 50 ://2 key
          cout<<"2 key press"<<endl;
          for(int i = 0 ; i < 3; i++){
            motion_2.play();
            wait(motion_2.getDuration());
          }
          
          break; 
          
        case 51 ://3 key
          cout<<"3 key press"<<endl;
          count = 0;
          while(count < 3){
            mMotionManager->playPage(15);
            wait(1000);
            mMotionManager->playPage(1);
            count++;
          }
          
          
          break; 
          
        case 81 ://Q key
          cout<<"q key press"<<endl;
          
          motion_1.play();
          motionThreeFlag = true;
          break;
          
        case 87 :
            cout << "User clicked W" << endl;
            mMotionManager->playPage(6, false);
            
            if(isSocket2Established){
              for(int i = 0 ; i < 256; i++){
                buffer[i] = command[i];
              }
              char command[] = "5";
              communicateWithServer(n, command, fd2);
            }
            
            while (mMotionManager->isMotionPlaying()) {
              mMotionManager->step(mTimeStep);
              /*
                * Do something,
                * like image processing for example
              */
              myStep();
            }
            break;
            
        case 69 :
            cout << "User clicked E" << endl;
            mMotionManager->playPage(10, false);
            
            if(isSocket1Established){
            //get the command
              for(int i = 0 ; i < 256; i++){
                buffer[i] = command[i];
              }
              char command[] = "BBC";
              communicateWithServer(n, command, fd);
            }
            while (mMotionManager->isMotionPlaying()) {
              mMotionManager->step(mTimeStep);
              /*
                * Do something,
                * like image processing for example
              */
              myStep();
            }
            break;
        default:
        cout << "default: User clicked " << key << endl;
          break;
      }
    }
=======
void Walk::runExerciseOne(int &g, char buffer[5], int fd) {
  g = 1;
  printf("in runExerciseOne, g=%d, fd=%d",g,fd);
  cout << "-------MotionPlayer first exercise of ROBOTIS OP2-------" << endl;
  cout << "This exercise plays a Webots hand_extend.motion file" << endl;
>>>>>>> refactoringCode

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
    
  for (int i=0; i<2; i++){
    mMotionManager->playPage(15);
    wait(500);
    mMotionManager->playPage(1);
  }
    
  mSpeaker->speak("Please start now.",1.0);
  wait(2000); 
    
}
   
void Walk::textToSpeechGreeting() {

  myStep();
  
  cout<<mSpeaker->getLanguage()<<endl;
  mMotionManager->playPage(1);
  mSpeaker->speak("Hello, my name is PTABOT. I am here to help you with your cardiac rehab therapy today.",1.0);
  wait(6000);
  mSpeaker->speak("Let’s start with our first exercise. Please follow the demonstration and repeat five times.",1.0);
  wait(6000);    
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