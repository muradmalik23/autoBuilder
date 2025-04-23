#include "util.hpp"
#include "subsystems/devices.hpp"

using namespace pros;

Lift::Lift(Motor motor, Rotation rotation, float kP, float kI, float kD, std::vector<double> heights, float acceptableError)
    : liftMotor(motor), liftPID(kP, kI, kD, 0, true), liftRotation(rotation), globalLiftState(zero), 
    zeroOffset(heights[0]), loadHeight(heights[1]), scoreHeight(heights[2]), allianceHeight(heights[3]),acceptableError(acceptableError){
  liftMotor.set_gearing(MotorGears::green);
  liftMotor.set_brake_mode(MotorBrake::hold);

}

void Lift::moveToPID(float position) {
  double rotationData = static_cast<double>(liftRotation.get_angle())/36000.0;
  if (rotationData>.875){rotationData = 0;}
  float error = position - rotationData;
  float output = liftPID.update(error);
  // printf("Rotations: %f, Global: %f, Error: %f, Output: %f \n", rotationData, globalTarget, error, output);
  liftMotor.move(output);
  // delay(100);
}

void Lift::setState(LiftState state) { globalLiftState = state; }

void Lift::move(std::uint32_t voltage) {
  globalLiftState = none;
  liftMotor.move(voltage);
}

void Lift::brake() { liftMotor.brake(); }

void Lift::setBrakeMode(MotorBrake mode) { liftMotor.set_brake_mode(mode); }

void Lift::moveTo(float position) {
  globalLiftState = custom;
  globalTarget = position;
}

double Lift::get_position() { return liftMotor.get_position(); }

LiftState Lift::getLiftState() { return globalLiftState; }

void Lift::driverControl() {
  if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN)) {
    globalLiftState = none;
    move(50);
  } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
    globalLiftState = none;
    move(-50);
  } else if (globalLiftState == none) {
    brake();
  }
}

void Lift::waitUntilFinished() {
  // add timeout
  while (fabs(globalTarget - get_position()) > acceptableError) {
    delay(20);
  }
}

void Lift::waitUntil(float position) {
  // add timeout
  position = fabs(position);
  while (inRange(fabs(get_position()), position - acceptableError,
    position + acceptableError)) {
    delay(20);
  }
}

void Lift::liftTask() {
  switch (globalLiftState) {
  case zero:
    globalTarget = zeroOffset;
    break;
  case load:
    globalTarget = loadHeight;
    break;
  case score:
    globalTarget = scoreHeight;
    break;
  case alliance:
    globalTarget = allianceHeight;
  case custom:
    break;
  case none:
    break;
  }
  if (globalLiftState != none) {
    moveToPID(globalTarget);
  }
  delay(20);
}

LiftState Lift::estimateState(float position){
  if (inRange(position, zeroOffset, zeroOffset+.02)){
    return zero;
  } else if (inRange(position, loadHeight-.02, loadHeight+.02)){
    return load;
  } else if (inRange(position, scoreHeight-.1, scoreHeight+.1)){
    return score;
  }
}