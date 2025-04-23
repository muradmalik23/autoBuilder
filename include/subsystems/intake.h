#include "pros/motor_group.hpp"
#include "pros/optical.hpp"

using namespace pros;

enum Alliance{
	red,
	blue,
	neutral
};

enum intakeState{
    regular,
    ejecting,
    relative,
    inch
};

class Intake{
    private:
    MotorGroup intakeMotor;
    Optical intakeOptical;


    void autoEject();
    void antiJam();

    int intakeVoltage = 0;
    int redMin, redMax, blueMin, blueMax;
    int distanceThreshold;
    float relativeTarget;
    float acceptableError = .01;

    Alliance currentAlliance = neutral;
    intakeState currentState = regular;

    public:
    
    Intake(int8_t motorA, int8_t motorB, Optical optical,
    std::vector<int> redRange, std::vector<int> blueRange, int distanceThreshold,
    Alliance startingAlliance, float acceptableError);

    void move(std::uint32_t voltage);
    void move_relative(double position);
    void brake();
    void inchRing(int attempts, bool async);

    void setAlliance(Alliance alliance);
    void setState(intakeState state);

    Alliance getAlliance();
    bool ringDetected();

    void intakeTask();
    void driverControl();
};