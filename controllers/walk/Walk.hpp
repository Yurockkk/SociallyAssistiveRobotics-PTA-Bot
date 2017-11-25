// Description:   Example showing how to use the gait manager
//                and keyboard inputs

#ifndef WALK_HPP
#define WALK_HPP

#define NMOTORS 20

#include <webots/Robot.hpp>

namespace managers {
  class RobotisOp2MotionManager;
  class RobotisOp2GaitManager;
}

namespace webots {
  class Motor;
  class PositionSensor;
  class LED;
  class Camera;
  class Accelerometer;
  class Gyro;
  class Keyboard;
  class Speaker;
};

class Walk : public webots::Robot {
  public:
                                     Walk();
    virtual                         ~Walk();
    void                             run();
    void                             checkIfFallen();

  private:
    int                              mTimeStep;

    void                             myStep();
    void                             wait(int ms);

    webots::Motor                   *mMotors[NMOTORS];
    webots::PositionSensor *mPositionSensors[NMOTORS];
    webots::Accelerometer           *mAccelerometer;
    webots::Keyboard                *mKeyboard;

    managers::RobotisOp2MotionManager *mMotionManager;
    managers::RobotisOp2GaitManager   *mGaitManager;
};

#endif
