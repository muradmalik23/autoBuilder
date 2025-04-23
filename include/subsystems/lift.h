#include "pros/motors.hpp"
#include "pros/rotation.hpp"
#include "lemlib/api.hpp"

using namespace pros;

enum LiftState{
    zero, //zero height
    load,  //loading on-hook height
    score, //outer height
    alliance,
    custom,
    none, //driver control state
};

class Lift{
    private:
    Motor liftMotor;
    Rotation liftRotation;

    LiftState globalLiftState;
    float globalTarget = 0;
    lemlib::PID liftPID;

    float loadHeight, scoreHeight, zeroOffset, allianceHeight;
    float acceptableError;

    void moveToPID(float position);

    public:
    
    Lift(Motor motor, Rotation rotation, float kP, float kI, float kD, std::vector<double> heights, float acceptableError);

    void setState(LiftState state);
    void move (std::uint32_t voltage);
    void moveTo (float position);
    double get_position();
    void brake();
    void setBrakeMode(MotorBrake mode);
    void driverControl();
    void waitUntilFinished();
    void waitUntil(float position);
    LiftState getLiftState();
    LiftState estimateState(float position);
    

    void liftTask();

};