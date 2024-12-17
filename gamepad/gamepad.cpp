#include <iostream>

#include <signal.h>

#include "gamepad.hpp"

bool run = true;

void sigint(int sig) {
  std::cout << "\nquitting.";
  run = false;
}

Joystick::Joystick(int index) {
  index_ = index;
  list();
  this->open();
  num_buttons_ = SDL_JoystickNumButtons(js_);
  std::cout << "\033[?25l"; // hide the cursor
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

void Joystick::open() {
  js_ = SDL_JoystickOpen(index_);
  if (js_ == NULL) {
    SDL_Quit();
    throw std::runtime_error("failed to open joystick. SDL_Error: " + std::string(SDL_GetError()));
  }
}

void Joystick::read() {
  std::cout << "\r";

  uint8_t hat_state = SDL_JoystickGetHat(js_, 0);

  // D-pad up
  if (hat_state & SDL_HAT_UP) {
    std::cout << "U";
  } else {
    std::cout << "*";
  }

  // D-pad down
  if (hat_state & SDL_HAT_DOWN) {
    std::cout << "D";
  } else {
    std::cout << "*";
  }

  // D-pad left
  if (hat_state & SDL_HAT_LEFT) {
    std::cout << "L";
  } else {
    std::cout << "*";
  }

  // D-pad right
  if (hat_state & SDL_HAT_RIGHT) {
    std::cout << "R ";
  } else {
    std::cout << "* ";
  }

  for (int i = 0; i < num_buttons_; ++i) {
    if (SDL_JoystickGetButton(js_, i)) {
      std::cout << "1";
    } else {
      std::cout << "0";
    }
  }
  std::cout.flush();
}

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_JOYSTICK) < 0) {
    throw std::runtime_error("SDL init failed. SDL_Error: " + std::string(SDL_GetError()));
    return 1;
  }

  Joystick* j = new Joystick();

  signal(SIGINT, sigint);
  while (run) {
    SDL_PumpEvents();
    j->read();
    // SDL_Delay(10);
  }

  delete j;
  SDL_Quit();
  return 0;
}
