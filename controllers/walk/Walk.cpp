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
#include <webots/Accelerometer.hpp>
#include <webots/Gyro.hpp>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Keyboard.hpp>
#include <RobotisOp2MotionManager.hpp>
#include <RobotisOp2GaitManager.hpp>

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <fstream>

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

#define SOCKET_SERVER1 "192.168.1.8"   /*  */
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

Walk::Walk():
    Robot()
{
  mTimeStep = getBasicTimeStep();

  getLED("HeadLed")->set(0xFF0000);
  getLED("EyeLed")->set(0x00FF00);
  mAccelerometer = getAccelerometer("Accelerometer");
  mAccelerometer->enable(mTimeStep);

  getGyro("Gyro")->enable(mTimeStep);

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
  mGaitManager   = new RobotisOp2GaitManager(this, "config.ini");

}

Walk::~Walk() {
}

void Walk::myStep() {
  int ret = step(mTimeStep);
  if (ret == -1)
    exit(EXIT_SUCCESS);
}

void Walk::wait(int ms) {
  double startTime = getTime();
  double s = (double) ms / 1000.0;
  while (s + startTime >= getTime())
    myStep();
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
  
  int fd = 0;
  int fd2 = 0;
  int n;
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
  cout << "This example illustrates Gait Manager" << endl;
  cout << "Press the space bar to start/stop walking" << endl;
  cout << "Use the arrow keys to move the robot while walking" << endl;

  // First step to update sensors values
  myStep();

  // stand up from initial movement
  mMotionManager->playPage(9); // init position
  wait(200);

  // main loop
  int key = 0;
  bool isWalking = false;

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
          
        case 87 :
            cout << "User clicked W" << endl;
            mMotionManager->playPage(6, false);
            
            if(isSocket2Established){
              for(int i = 0 ; i < 256; i++){
                buffer[i] = command[i];
              }
              n = strlen(buffer);
              //buffer[n++] = '\n';     /* append carriage return */
              buffer[n] = '\0';
              n = send(fd2, buffer, n, 0);

              if (strncmp(buffer, "exit", 4) == 0) {
                  //break;
              }

              n = recv(fd2, buffer, 256, 0);
              buffer[n] = '\0';
              printf("Answer is: %s\n", buffer);
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
              for(int i = 0 ; i < 256; i++){
                buffer[i] = command[i];
              }
            
              n = strlen(buffer);
              //buffer[n++] = '\n';     /* append carriage return */
              buffer[n] = '\0';
              n = send(fd, buffer, n, 0);

              if (strncmp(buffer, "exit", 4) == 0) {
                  //break;
              }

              n = recv(fd, buffer, 256, 0);
              buffer[n] = '\0';
              printf("Answer is: %s\n", buffer);
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

    mGaitManager->step(mTimeStep);

    // step
    myStep();
  }
}

void Walk::checkIfFallen() {
  static int fup = 0;
  static int fdown = 0;
  static const double acc_tolerance = 80.0;
  static const double acc_step = 100;

  // count how many steps the accelerometer
  // says that the robot is down
  const double *acc = mAccelerometer->getValues();
  if (acc[1] < 512.0 - acc_tolerance)
    fup++;
  else
    fup = 0;

  if (acc[1] > 512.0 + acc_tolerance)
    fdown++;
  else
    fdown = 0;

  // the robot face is down
  if (fup > acc_step) {
    mMotionManager->playPage(10); // f_up
    mMotionManager->playPage(9); // init position
    fup = 0;
  }
  // the back face is down
  else if (fdown > acc_step) {
    mMotionManager->playPage(11); // b_up
    mMotionManager->playPage(9); // init position
    fdown = 0;
  }
}
