#include "autoBuilder.hpp"

autoBuilder::autoBuilder(std::vector<Command> commands,
                         pros::Controller controller, lemlib::Chassis *chassis,
                         bool startingState)
    : commands(commands), controller(controller), chassis(chassis),
      flag(startingState) {}

void autoBuilder::init() {
  controller.clear();
  controller.print(1, 1, "autoBuilder on.");
  controller.print(2, 1, "Action:");

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
  if (!flag) {
    return;
  }

  while (true) {
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_LEFT)) {
      controllerIndex =
          (controllerIndex - 1 + commands.size()) % commands.size();
    }
    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_RIGHT)) {
      controllerIndex = (controllerIndex + 1) % commands.size();
    }

    controller.print(3, 1, "%s", commands[controllerIndex].name);

    if (controller.get_digital_new_press(pros::E_CONTROLLER_DIGITAL_A)) {
      controller.rumble(".");
      printf("%s \n",
             fmt::to_string(commands[controllerIndex].codeGen).c_str());
    }

    pros::delay(100);
  }
}

void autoBuilder::setFlag(bool state) { flag = state; }