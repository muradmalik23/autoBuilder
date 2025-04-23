#include "main.h"

lemlib::Pose chassisPose = chassis.getPose();

std::vector<Command> commands = {
    {"Point Fwd", []() {
        return std::format("chassis.moveToPoint(%f, %f, 1500, {.maxSpeed = 100, .minSpeed = 20, .earlyExitRange = .05}, false);", chassisPose.x, chassisPose.y);
    }},
    {"Point Rev", []() {
        return std::format("chassis.moveToPoint(%f, %f, 1500, {.forwards = false, .minSpeed = 20, .earlyExitRange = .05}, false);", chassisPose.x, chassisPose.y);
    }},
    {"Point Goal", []() {
        return std::format("chassis.moveToPoint(%f, %f, 1500, {.forwards = false, .minSpeed = 50, .earlyExitRange = .05}), false;", chassisPose.x, chassisPose.y);
    }},
    {"Pose Fwd", []() {
        return std::format("chassis.moveToPose(%f, %f, %f, 1500, {.minSpeed = 20, .earlyExitRange = .05}, false);", chassisPose.x, chassisPose.y, chassisPose.theta);
    }},
    {"Pose Goal", []() {
        return std::format("chassis.moveToPose(%f, %f, %f, 1500, {.forwards = false, .minSpeed = 50, .earlyExitRange = .05}, false);", chassisPose.x, chassisPose.y, chassisPose.theta);
    }},
    {"Point Turn Fwd", []() {
        return std::format("chassis.turnToPoint(%f, %f, 1500, {.minSpeed = 20, .earlyExitRange = .05}, false);", chassisPose.x, chassisPose.y);
    }},
    {"Point Turn Rev", []() {
        return std::format("chassis.turnToPoint(%f, %f, 1500, {.forwards = false, .minSpeed = 20, .earlyExitRange = .05}, false);", chassisPose.x, chassisPose.y);
    }},
    {"Heading Turn", []() {
        return std::format("chassis.turnToHeading(%f, 500, {}, false);", chassisPose.theta);
    }},
    {"Intake", []() {
        return "intake.move(127);";
    }},
    {"Outtake", []() {
        return "intake.move(-127);";
    }},
    {"Stop Intake", []() {
        return "intake.move(0);";
    }},
    {"Clamp", []() {
        return "clamp.set_value(true);";
    }},
    {"Unclamp", []() {
        return "clamp.set_value(false);";
    }},
    {"Sweeper Down", []() {
        return "sweeper.set_value(true);";
    }},
    {"Sweeper Up", []() {
        return "sweeper.set_value(false);";
    }}
};

autoBuilder autoBuilder(commands, controller, &chassis, true);


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
		pros::delay(20);                               // Run for 20 ms then update
	}
}