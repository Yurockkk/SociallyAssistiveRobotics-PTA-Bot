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
    void     run(int &fd, int &fd2,int &n);
    int      runExerciseOne(int n, char buffer[5], int fd);
    int      runExerciseTwo(int n, int currentLevel, int fd);
    int      runExerciseThree(int n, int currentLevel, int fd);
    void     textToSpeechGreeting();
    void     textToSpeechSecondExercise();
    void     textToSpeechThirdExercise();
    void     textToSpeechSad();
    void     textToSpeechHappy();
    void     textToSpeechSadEncouragement();
    void     textToSpeechHappyEncouragement();
    void     textToSpeechEnding();
    void     getUpdatedLevel(int &currentLevel,int &resultFromCNN);
    void     textToSpeechEncourage();

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
