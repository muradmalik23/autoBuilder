#include "liblvgl/draw/lv_img_buf.h"
#include "main.h"
#include "robodash/views/console.hpp"
#include "robodash/views/image.hpp"

Controller controller(E_CONTROLLER_MASTER);

rd::Console console ("Terminal");
extern const lv_img_dsc_t logo;
rd::Image logoScreen(&logo, "Logo" );

Intake intake(7, 8, 14, {350, 10}, {215, 230}, 150, neutral, .01);
Lift lift(-9, 15, 750, 0, 500, {0, .08, .44, .50}, .01);

IMU imu(13);
Rotation xRotation(-11);
Rotation yRotation(-12);

lemlib::TrackingWheel xTracker(&xRotation, lemlib::Omniwheel::NEW_2, -3.5);
lemlib::TrackingWheel yTracker(&yRotation, lemlib::Omniwheel::NEW_2, .75);

pros::adi::Pneumatics mogoMech('A', true, true);
pros::adi::Pneumatics sweeper('B', false, false);

MotorGroup left_motors({-1, -2, 3}, v5::MotorGears::blue); // left motors on ports 1, 2, 3
MotorGroup right_motors({4, 5, -6}, v5::MotorGears::blue); // right motors on ports 4, 5, 6

lemlib::Drivetrain drivetrain(&left_motors, // left motor group
                              &right_motors, // right motor group
                              12, // 12 inch track width
                              lemlib::Omniwheel::NEW_325, // using new 4" omnis
                              450, // drivetrain rpm is 450
                            4 // horizontal drift is 2 (for now)
);
lemlib::OdomSensors sensors(&yTracker, // vertical tracking wheel 1, set to null
                            nullptr, // vertical tracking wheel 2, set to nullptr as we are using IMEs
                            &xTracker, // horizontal tracking wheel 1
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);
// lateral PID controller
//13, 0, 37
lemlib::ControllerSettings lateral_controller(10, // proportional gain (kP)
                                              0.05, // integral gain (kI)
                                              60, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);

// angular PID controller
//8, 0.005, 90
lemlib::ControllerSettings angular_controller(7.3, // proportional gain (kP)
                                              0.0075, // integral gain (kI)
                                              78.6, // derivative gain (kD)
                                              3, // anti windup
                                              1, // small error range, in inches
                                              100, // small error range timeout, in milliseconds
                                              3, // large error range, in inches
                                              500, // large error range timeout, in milliseconds
                                              0 // maximum acceleration (slew)
);
lemlib::Chassis chassis(drivetrain, // drivetrain settings
                        lateral_controller, // lateral PID settings
                        angular_controller, // angular PID settings
                        sensors // odometry sensors
);

