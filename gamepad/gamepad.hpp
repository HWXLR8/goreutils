#pragma once

#include <SDL2/SDL.h>

class Joystick {
 public:
  Joystick();
  ~Joystick();
  void open(int index);
  void read();
  void list();

 private:
  SDL_Joystick *js_ = nullptr;
  int index_ = 0;
  int num_buttons_;
};
