#include "autoBuilder.hpp"
#include "robodash/views/console.hpp"
#include "robodash/views/image.hpp"
#include <stdio.h>
#include <string>

autoBuilder::autoBuilder(std::vector<Command> commands,
                         pros::Controller *controller, rd::Console *console, rd::Image *image,
                         bool startingState)
    : commands(commands), controller(controller), console(console), image(image),
      flag(startingState) {}

void autoBuilder::init() {
  controller->clear();
  pros::delay(50);
  controller->print(0, 0, "autoBuilder on!");
  image->focus();
  console->clear();
  printf(R"(
d8888b. d88  dP   dP d88888P M""""""""`M                        dP               dP                oo dP       dP                   
  	`88  88  88   88     d8' Mmmmmm   .M                        88               88                   88       88                   
.aaadP'  88  88aaa88    d8'  MMMMP  .MMM    .d8888b. dP    dP d8888P .d8888b.    88d888b. dP    dP dP 88 .d888b88 .d8888b. 88d888b. 
88'      88       88   d8'   MMP  .MMMMM    88'  `88 88    88   88   88'  `88    88'  `88 88    88 88 88 88'  `88 88ooood8 88'  `88 
88.      88       88  d8'    M' .MMMMMMM    88.  .88 88.  .88   88   88.  .88    88.  .88 88.  .88 88 88 88.  .88 88.  ... 88       
Y88888P d88P      dP d8'     M         M    `88888P8 `88888P'   dP   `88888P'    88Y8888' `88888P' dP dP `88888P8 `88888P' dP     
		
)");
}

void autoBuilder::run() {
  controller->print(2, 0, "%-16s", commands[controllerIndex].name.c_str());
  while (flag) {

    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
      controllerIndex = (controllerIndex - 1 + commands.size()) % commands.size();
      controller->print(2, 0, "%-16s", commands[controllerIndex].name.c_str());
    }
    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
      controllerIndex = (controllerIndex + 1) % commands.size();
      controller->print(2, 0, "%-16s", commands[controllerIndex].name.c_str());
    }

    if (controller->get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
      controller->rumble(".");
      std::string code = commands[controllerIndex].codeGen();
      printf("%s\n", code.c_str());
      console->printf("%s\n", code.c_str());
    }

    pros::delay(100);
  }
}

void autoBuilder::setFlag(bool state) { flag = state; }