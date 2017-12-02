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

#define SOCKET_PORT1 10020
#define SOCKET_PORT2 10021

#define SOCKET_SERVER1 "127.0.0.1"   /*  */
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

void communicateWithServer(int n, char buffer[5], int fd) {
         
  n = strlen(buffer);
  //buffer[n++] = '\n';     /* append carriage return */
  //buffer[n] = '\0';
  n = send(fd, buffer, n, 0);

  if (strncmp(buffer, "exit", 4) == 0) {
    //break;
  }

  n = recv(fd, buffer, 256, 0);
  buffer[n] = '\0';
  printf("Answer is: %s\n", buffer);
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
void Walk::run() {

  // First step to update sensors values
  myStep();
  int fd = -1;
  int fd2 = -1;
  int n = -1;
  char buffer[256];
  char command[256] = {'y','e','s'};
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


void Walk::runExerciseOne() {

  cout << "-------MotionPlayer first exercise of ROBOTIS OP2-------" << endl;
  cout << "This exercise plays a Webots hand_extend.motion file" << endl;

  // step
  myStep();
  
   /*
    bool motionOneFlag = false;
    bool motionTwoFlag = false;
    bool motionThreeFlag = false;
    */
    Motion motion_1("hand_extend.motion");
    int time1 = motion_1.getDuration();
    for (int i=0; i<2; i++){
    	motion_1.play();
    	wait(time1);
    }
    
    mSpeaker->speak("Please start now.",1.0);
    wait(2000); 
    
   // motion_1.setLoop(true);
   // motion_2.setLoop(true);
    
    /*while(true){
    
        switch(mKeyboard->getKey()) {
        case 81 ://Q key
          cout<<"q key press"<<endl;
          motion_1.stop();
          motion_2.stop();
          motionThreeFlag = true;
          break;
        case 87 ://W key
          cout<<"w key press"<<endl;
          motionThreeFlag = false;
          break;
        case Keyboard::UP :
          //cout<<"pressed up key"<<endl;
          motion_2.stop();
          motionTwoFlag = false;
          motionThreeFlag = false;
          motion_1.play();
          motionOneFlag = true;
          break;
        case Keyboard::DOWN :
          //cout<<"pressed down key"<<endl;
          motion_1.stop();
          motionOneFlag = false;
          break;
        case Keyboard::RIGHT :
          //cout<<"pressed right key"<<endl;
          motion_2.stop();
          motionTwoFlag = false;
          break;
        case Keyboard::LEFT :
          //cout<<"pressed left key"<<endl;
          motion_1.stop();
          motionOneFlag = false;
          motionThreeFlag = false;
          motion_2.play();
          motionTwoFlag = true;
          break;
      }
     
    if (motionThreeFlag){
      mMotionManager->playPage(1);
      wait(1000);
      mMotionManager->playPage(15);
      motionThreeFlag = false;
      cout<<"finsh"<<endl;
    }
      myStep();
    }
    
  while (true){
      // play the hello motion
    mMotionManager->playPage(1); // init position
    wait(1000);
    mMotionManager->playPage(15);
  }*/
}

void Walk::runExerciseTwo() {

  cout << "-------MotionPlayer second exercise of ROBOTIS OP2-------" << endl;
  cout << "This exercise plays a Webots hand_high.motion file" << endl;

  // step
  myStep();
  

    Motion motion_2("hand_high.motion");
    int time2 = motion_2.getDuration();
    for (int i=0; i<2; i++){
    	motion_2.play();
    	wait(time2);
    }
    
    mSpeaker->speak("Please start now.",1.0);
    wait(2000); 
}


void Walk::runExerciseThree() {

  cout << "-------MotionPlayer last exercise of ROBOTIS OP2-------" << endl;
  cout << "This exercise plays blend knee motion" << endl;

  // step
  myStep();
  
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

void Walk::textToSpeechSecondExercise() {

  myStep();
  
    cout<<mSpeaker->getLanguage()<<endl;

    mSpeaker->speak("Now let’s move on to do the next exercise. Please follow the demonstration",1.0);

}

void Walk::textToSpeechThirdExercise() {

  myStep();
       
    mSpeaker->speak("This is your last exercise. Please follow the demonstration",1.0);

}

void Walk::textToSpeechSad() {

  myStep();
  
    mSpeaker->speak("And repeat three times",1.0);

}

void Walk::textToSpeechHappy() {

  myStep();
    
    mSpeaker->speak("And repeat eight times",1.0);

}

void Walk::textToSpeechSadEncouragement() {

  myStep();
    
    mSpeaker->speak("You can do it!",1.0);
    wait(5000);
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