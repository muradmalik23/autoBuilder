#include "main.h"
#include "autoBuilder.hpp"
#include <format>
#include <vector>

lemlib::MoveToPointParams regularpoint = {.maxSpeed = 100, .minSpeed = 20, .earlyExitRange = .05};
lemlib::MoveToPoseParams regularpose = {.minSpeed = 20, .earlyExitRange = .05};
lemlib::MoveToPointParams reverseRegular = {.forwards = false, .minSpeed = 20, .earlyExitRange = .05};
lemlib::MoveToPointParams goal = {.forwards = false, .minSpeed = 50, .earlyExitRange = .05};
lemlib::MoveToPoseParams goalPose = {.forwards = false, .minSpeed = 50, .earlyExitRange = .05};
lemlib::TurnToPointParams regularTurn = {.minSpeed = 20, .earlyExitRange = .05};
lemlib::TurnToPointParams reverseTurn = {.forwards = false, .minSpeed = 20, .earlyExitRange = .05};
lemlib::TurnToHeadingParams headingTurn = {.minSpeed = 20, .earlyExitRange = .05};

std::vector<Command> commands = {
    {"Point Fwd",
     []() {
       return std::format(
           "chassis.moveToPoint({}, {}, 1500, regularPoint, false);",
           chassis.getPose().x, chassis.getPose().y);
     }},
    {"Point Rev",
     []() {
       return std::format(
           "chassis.moveToPoint({}, {}, 1500, reverseRegular, false);",
           chassis.getPose().x, chassis.getPose().y);
     }},
    {"Point Goal",
     []() {
       return std::format("chassis.moveToPoint({}, {}, 1500, goal), false;",
                          chassis.getPose().x, chassis.getPose().y);
     }},
    {"Pose Fwd",
     []() {
       return std::format(
           "chassis.moveToPose({}, {}, {}, 1500, regularPose, false);",
           chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
     }},
    {"Pose Goal",
     []() {
       return std::format(
           "chassis.moveToPose({}, {}, {}, 1500, goalPose, false);",
           chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
     }},
    {"Point Turn Fwd",
     []() {
       return std::format(
           "chassis.turnToPoint({}, {}, 1500, regularTurn, false);",
           chassis.getPose().x, chassis.getPose().y);
     }},
    {"Point Turn Rev",
     []() {
       return std::format(
           "chassis.turnToPoint({}, {}, 1500, reverseTurn, false);",
           chassis.getPose().x, chassis.getPose().y);
     }},
    {"Heading Turn",
     []() {
       return std::format("chassis.turnToHeading({}, 500, headingTurn, false);",
                          chassis.getPose().theta);
     }},
    {"Intake", []() { return "intake.move(127);"; }},
    {"Outtake", []() { return "intake.move(-127);"; }},
    {"Stop Intake", []() { return "intake.move(0);"; }},
    {"Clamp", []() { return "clamp.set_value(true);"; }},
    {"Unclamp", []() { return "clamp.set_value(false);"; }},
    {"Sweeper Down", []() { return "sweeper.set_value(true);"; }},
    {"Sweeper Up", []() { return "sweeper.set_value(false);"; }}};

autoBuilder autoBuilder(commands, controller, &chassis, &console, &logoScreen, true);


/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    autoBuilder.init();
    chassis.calibrate();
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	autoBuilder.run();
	while (true) {
        // printf("%f, %f, %f \n", chassis.getPose().x, chassis.getPose().y, chassis.getPose().theta);
        // c::delay(500);

        pros::delay(20);                               // Run for 20 ms then update
	}
}