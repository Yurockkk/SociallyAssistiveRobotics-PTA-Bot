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
    void     run();
    void     runExerciseOne();
    void     runExerciseTwo();
    void     runExerciseThree();
    void     textToSpeechGreeting();
    void     textToSpeechSecondExercise();
    void     textToSpeechThirdExercise();
    void     textToSpeechSad();
    void     textToSpeechHappy();
    void     textToSpeechSadEncouragement();
    void     textToSpeechHappyEncouragement();
    void     textToSpeechEnding();

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
