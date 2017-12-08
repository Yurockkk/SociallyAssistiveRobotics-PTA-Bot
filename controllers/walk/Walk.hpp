// Description:   Example showing how to use the gait manager
//                and keyboard inputs

#ifndef WALK_HPP
#define WALK_HPP

#define NMOTORS 20

#include <webots/Robot.hpp>

namespace managers {
  class RobotisOp2MotionManager;
}

namespace webots {
  class Motor;
  class PositionSensor;
  class Keyboard;
  //class Camera;
};

class Walk : public webots::Robot {
  public:
             Walk();
    virtual  ~Walk();
    void     createSocketWithServers(int &fd, int &fd2,int &n);
    void     runExerciseOne(int &g, char buffer[5], int fd);
    void     runExerciseTwo(int &g, int currentLevel, int fd);
    void     runExerciseThree(int &g, int currentLevel, int fd);
    void     textToSpeechGreeting();
    void     textToSpeechEnding();
    void     textToSpeechEncourage();
    void     getUpdatedLevel(int &currentLevel,int &resultFromCNN);
    int      communicateWithServer(int g, int currentLevel, int fd, int fd2);

  private:
    int      mTimeStep;

    void     myStep();
    void     wait(int ms);

    webots::Motor                   *mMotors[NMOTORS];
    webots::PositionSensor *mPositionSensors[NMOTORS];
    webots::Speaker                 *mSpeaker;
    webots::Keyboard                *mKeyboard;

    managers::RobotisOp2MotionManager *mMotionManager;
};

#endif
