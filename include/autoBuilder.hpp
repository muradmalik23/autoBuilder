#pragma once
#include "lemlib/chassis/chassis.hpp"
#include "pros/misc.hpp"
#include "robodash/views/console.hpp"
#include "robodash/views/image.hpp"
#include <functional>
#include <string>

struct Command {
  std::string name;
  std::function<std::string()> codeGen;
};

class autoBuilder {
public:
  autoBuilder(std::vector<Command> commands, pros::Controller controller,
              lemlib::Chassis *chassis, rd::Console* console, rd::Image* image, bool startingState = false);
  void init();
  void setFlag(bool state);
  void run();

private:
  int controllerIndex = 0;
  std::vector<Command> commands;
  pros::Controller controller;
  lemlib::Chassis *chassis;
  lemlib::Pose chassisPose = chassis->getPose();
  rd::Console *console;
  rd::Image *image;
  bool flag;
};