#include "subsystems/devices.hpp"
#include "util.hpp"

Intake::Intake(int8_t motorA, int8_t motorB, Optical optical,
  std::vector<int> redRange, std::vector<int> blueRange, int distanceThreshold,
  Alliance startingAlliance, float acceptableError):
  intakeMotor({motorA, motorB}), intakeOptical(optical),
  redMin(redRange[0]), redMax(redRange[1]), blueMin(blueRange[0]),
  blueMax(blueRange[1]), distanceThreshold(distanceThreshold),
  currentAlliance(startingAlliance) {
  intakeOptical.set_led_pwm(23*3); 
  intakeOptical.set_integration_time(3);
  intakeMotor.set_encoder_units(E_MOTOR_ENCODER_ROTATIONS);
}

void Intake::move(std::uint32_t voltage) {
  currentState = regular;
  intakeVoltage = voltage;
}

void Intake::move_relative(double position) {
  currentState = relative;
  relativeTarget = position;
}

void Intake::brake() { intakeVoltage = 0; }

void Intake::antiJam() {
  bool condition = ((intakeMotor.get_efficiency() == 0) &&
      intakeMotor.get_current_draw() > 1800 &&
      lift.getLiftState() == zero);
  if (condition) {
    delay(100);
    if(condition){
          intakeMotor.move(-80);
    delay(200);
    }
  }
}


void Intake::autoEject(){

  if (!intakeOptical.is_installed()){
    return;
  }

  double hue = intakeOptical.get_hue();
  double distance = intakeOptical.get_proximity();

  switch (currentAlliance){
    case red:
      if (inRange(hue, blueMin, blueMax) && distance > distanceThreshold){
      setState(ejecting);
      }
      break;
    case blue:
      if (inRange(hue, redMin, redMax) && distance > distanceThreshold){
      setState(ejecting);
      }
      break;
    case neutral:
      break;
  }
}


void Intake::setAlliance(Alliance alliance) { currentAlliance = alliance; }

void Intake::setState(intakeState state) { currentState = state; }

void Intake::intakeTask() {
  autoEject();
  antiJam();

  switch (currentState) {
  case regular:
    intakeMotor.move(intakeVoltage);
    break;

  case ejecting:
    while(intakeOptical.get_proximity() > distanceThreshold){
      intakeMotor.move(127);
    }
    // delay(3);
    intakeMotor.move(-127);
    delay(80);
    setState(regular);
    break;

  case relative:
    intakeMotor.tare_position_all();
    delay(23);
    intakeMotor.move_relative(relativeTarget, 127);
    delay(500);
    currentState = regular;
    break;
  case inch:
    break;
  }

  delay(20);
}

void Intake::driverControl() {
  if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R2)) {
    move(127);
  } else if (controller.get_digital(pros::E_CONTROLLER_DIGITAL_R1)) {
    move(-127);
  } else if (currentState == regular) {
    brake();
  }
  // if(currentAlliance == neutral){
  //   controller.set_text(2, 0, "Color Sort: Off");
  // } else{
	// 	controller.set_text(2, 0, "Color Sort: On ");
  // }
}

Alliance Intake::getAlliance(){
  return currentAlliance;
}

bool Intake::ringDetected(){
  return(intakeOptical.get_proximity() > distanceThreshold);
}

void Intake::inchRing(int attempts, bool async){
  if(async){
    Task inchTempTask([attempts, this]{inchRing(attempts, false);});
  }
  currentState = inch;
  int fwdDelay = 200;
  int stopDelay = 100;
  for (int i = 0; i<= attempts; i++){
    intakeMotor.move(127);
    delay(fwdDelay);
    intakeMotor.move(-35);
    delay(stopDelay);
  }
  currentState = regular;
}