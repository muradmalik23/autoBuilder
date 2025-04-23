#pragma once

#include "fmt/format.h"
#include "lemlib/chassis/chassis.hpp"
#include "pros/misc.hpp"
#include <functional>
#include <string>

struct Command {
  std::string name;
  std::function<std::string()> codeGen;
};

class autoBuilder {
public:
  autoBuilder(std::vector<Command> commands, pros::Controller controller,
              lemlib::Chassis *chassis, bool startingState = false);
  void init();
  void setFlag(bool state);
  void run();

private:
  int controllerIndex = 0;
  const std::vector<Command> commands;
  pros::Controller controller;
  lemlib::Chassis *chassis;
  lemlib::Pose chassisPose = chassis->getPose();
  bool flag;
};