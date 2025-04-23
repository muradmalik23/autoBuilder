#include "main.h"
#include "fmt/format.h"
#include "pros/misc.h"
#include <cstdio>
#include <format>
#include <functional>
#include <vector>
//aliases for cleanliness
lemlib::Pose chassisPose = chassis.getPose();

struct Command{
	std::string name;
	std::function<std::string()> codeGen;
};

int abIndex = 0;
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


void autoBuilder(){
	controller.clear();
	controller.print(1, 1, "AutoBuilder Active.");
	controller.print(2, 1, "Action:");

	while (true) {
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
			abIndex = (abIndex - 1 + commands.size()) % commands.size(); // Wrap around
		}
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
			abIndex = (abIndex + 1) % commands.size();
		}
	
		controller.print(3, 1, "%s", commands[abIndex].name);
	
		if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
		  controller.rumble(".");
		  printf("%s", fmt::to_string(commands[abIndex].codeGen).c_str());
		  printf("\n");
		}
	
		pros::delay(100);
	  }
}

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
	printf(R"(
		d8888b. d88  dP   dP d88888P M""""""""`M                        dP               dP                oo dP       dP                   
			`88  88  88   88     d8' Mmmmmm   .M                        88               88                   88       88                   
		.aaadP'  88  88aaa88    d8'  MMMMP  .MMM    .d8888b. dP    dP d8888P .d8888b.    88d888b. dP    dP dP 88 .d888b88 .d8888b. 88d888b. 
		88'      88       88   d8'   MMP  .MMMMM    88'  `88 88    88   88   88'  `88    88'  `88 88    88 88 88 88'  `88 88ooood8 88'  `88 
		88.      88       88  d8'    M' .MMMMMMM    88.  .88 88.  .88   88   88.  .88    88.  .88 88.  .88 88 88 88.  .88 88.  ... 88       
		Y88888P d88P      dP d8'     M         M    `88888P8 `88888P'   dP   `88888P'    88Y8888' `88888P' dP dP `88888P8 `88888P' dP     
		
		)");
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
	autoBuilder();
	while (true) {
		pros::delay(20);                               // Run for 20 ms then update
	}
}