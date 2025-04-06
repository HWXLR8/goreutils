#include <iostream>

#include <signal.h>
#include <stdlib.h>

#include "gamepad.hpp"

bool run = true;

void sigint(int sig) {
  std::cout << "\nquitting.";
  run = false;
}

Joystick::Joystick() {
}

Joystick::~Joystick() {
  SDL_JoystickClose(js_);
  std::cout << "\033[?25h"; // show the cursor
}

void Joystick::list() {
  int num_joysticks = SDL_NumJoysticks();
  if (num_joysticks < 1) {
    SDL_Quit();
    throw std::runtime_error("no joysticks found");
  }

  std::cout << "Detected the following joysticks:" << std::endl;
  for (int i = 0; i < num_joysticks; ++i) {
    std::cout << "joystick " << i << ": " << SDL_JoystickNameForIndex(i) << std::endl;
  }
  std::cout << std::endl;
}

void Joystick::open(int index) {
  index_ = index;
  js_ = SDL_JoystickOpen(index_);
  if (js_ == NULL) {
    SDL_Quit();
    throw std::runtime_error("failed to open joystick. SDL_Error: " + std::string(SDL_GetError()));
  }
  num_buttons_ = SDL_JoystickNumButtons(js_);
  std::cout << "\033[?25l"; // hide the cursor
}

void Joystick::read() {
  std::cout << "\r";

    //  D-pad (hat)
    uint8_t hat_state = SDL_JoystickGetHat(js_, 0);

    if (hat_state & SDL_HAT_UP) { std::cout << "U"; } else { std::cout << "*"; }
    if (hat_state & SDL_HAT_DOWN) { std::cout << "D"; } else { std::cout << "*"; }
    if (hat_state & SDL_HAT_LEFT) { std::cout << "L"; } else { std::cout << "*"; }
    if (hat_state & SDL_HAT_RIGHT) { std::cout << "R "; } else { std::cout << "* "; }

    // buttons
    for (int i = 0; i < num_buttons_; ++i) {
        if (SDL_JoystickGetButton(js_, i)) { std::cout << "1"; } else { std::cout << "0"; }
    }

    std::cout << "\n\r";

    // analog axes
    const int axis_display_width = 32;
    for (int i = 0; i < SDL_JoystickNumAxes(js_); ++i) {
      std::cout << "\r";

      Sint16 axis_value = SDL_JoystickGetAxis(js_, i);
      int normalized = (axis_value + 32768) * 256 / 65536;
      int position = (normalized * axis_display_width) / 256;

      // axis visualizer
      std::cout << "analog " << i << " [";
      for (int j = 0; j < axis_display_width; ++j) {
        std::cout << (j == position ? "*" : " ");
      }
      std::cout << "] " << normalized << "/256";

      // move to next line if not last axis
      if (i < SDL_JoystickNumAxes(js_) - 1) std::cout << "\n\r";
    }

    std::cout.flush();

    // move cursor back up for next update
    std::cout << "\033[" << SDL_JoystickNumAxes(js_) << "A";  // ANSI escape: move up N lines

}

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
    throw std::runtime_error("SDL init failed. SDL_Error: " + std::string(SDL_GetError()));
    return 1;
  }

  Joystick* j = new Joystick();

  // no index, list joysticks
  if (argc == 1) {
    j->list();
    std::cout << "No joystick selected." << std::endl;
    std::cout << "Usage: ./gamepad [index]" << std::endl;
    return 0;
  }

  signal(SIGINT, sigint);

  j->open(atoi(argv[1]));
  while (run) {
    SDL_PumpEvents();
    j->read();
    // SDL_Delay(10);
  }

  delete j;
  SDL_Quit();
  return 0;
}
