#include "intake.h"
#include "lemlib/chassis/chassis.hpp"
#include "lift.h"
#include "pros/adi.hpp"
#include "robodash/views/console.hpp"
#include "robodash/views/image.hpp"

extern Controller controller;

extern Lift lift;
extern Intake intake;

extern lemlib::Chassis chassis;

extern pros::adi::Pneumatics mogoMech, sweeper;

extern rd::Console console;
extern rd::Image logoScreen;