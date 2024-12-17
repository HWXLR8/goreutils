#pragma once

#include <SDL2/SDL.h>

class Joystick {
 public:
  Joystick(int index = 0);
  ~Joystick();
  void read();

 private:
  SDL_Joystick *js_ = nullptr;
  int index_;
  int num_buttons_;

  void list();
  void open();
};
