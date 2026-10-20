#pragma once
#include "lemlib/chassis/chassis.hpp"
#include "pros/misc.hpp"
#include "robodash/views/console.hpp"
#include "robodash/views/image.hpp"
#include <functional>
#include <vector>
#include <string>
#include <unordered_map>




class autoBuilder {
  public:

    struct Command {
      std::string name;
      std::string tpl; // e.g. "go({x}, {y});" - {key} filled from the snapshot
    };

    // name -> value, read fresh on every button press
    using State = std::unordered_map<std::string, double>;

    autoBuilder(std::vector<Command> commands, pros::Controller *controller,
                rd::Console* console, rd::Image* image, std::function<State()> snapshot,
                bool startingState = false);
    
    void init();
    void setFlag(bool state);
    void run();

  private:
    int controllerIndex = 0;
    std::vector<Command> commands;
    pros::Controller *controller;
    rd::Console *console;
    rd::Image *image;
    std::function<State()> snapshot;
    std::string render(const std::string &tpl);
    bool flag;
};