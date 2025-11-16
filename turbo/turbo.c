#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
  if (SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0) {
    fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
    return 1;
  }

  if (SDL_NumJoysticks() < 1) {
    fprintf(stderr, "No joysticks/gamepads found.\n");
    SDL_Quit();
    return 1;
  }

  SDL_Joystick *joy = SDL_JoystickOpen(0);
  if (!joy) {
    fprintf(stderr, "SDL_JoystickOpen failed: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
  }

  SDL_JoystickEventState(SDL_ENABLE);

  printf("Joystick 0 opened.\n");
  printf("press and hold any button with turbo enabled.\n");

  int running = 1;

  int active_button = -1;
  unsigned int press_count = 0;   // number of JOYBUTTONDOWN events
  Uint32 first_down_ms = 0;       // time of first down
  Uint32 last_down_ms = 0;        // time of latest down
  Uint32 last_event_ms = 0;       // time of last event (down or up)
  const Uint32 IDLE_MS = 200;     // idle time before finalizing measurement

  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        running = 0;
      }
      else if (e.type == SDL_JOYBUTTONDOWN) {
        int btn = e.jbutton.button;
        Uint32 now = SDL_GetTicks();

        if (active_button == -1) {
          // Start tracking this button
          active_button = btn;
          press_count = 0;
          first_down_ms = now;
          last_down_ms = now;
          last_event_ms = now;
          printf("Now tracking button %d\n", btn);
        }

        if (btn == active_button) {
          if (press_count == 0) {
            first_down_ms = now;
          }
          press_count++;
          last_down_ms = now;
          last_event_ms = now;

          if (press_count >= 5) { // wait for a few pulses for a stable estimate
            double dt = (last_down_ms - first_down_ms) / 1000.0;
            if (dt > 0.0) {
              // presses-1 intervals over dt seconds
              double rate = (press_count - 1) / dt;
              printf("\rButton %d: %.2f Hz (presses: %u, window: %.2fs)",
                     active_button, rate, press_count, dt);
              fflush(stdout);
            }
          }
        }
      }
      else if (e.type == SDL_JOYBUTTONUP) {
        int btn = e.jbutton.button;
        Uint32 now = SDL_GetTicks();

        if (btn == active_button) {
          // Just update last_event_ms; don't end measurement here
          last_event_ms = now;
        }
      }
    }

    // Check for inactivity on the active button to finalize measurement
    if (active_button != -1 && press_count > 0) {
      Uint32 now = SDL_GetTicks();
      if (now - last_event_ms > IDLE_MS) {
        if (press_count > 1) {
          double dt = (last_down_ms - first_down_ms) / 1000.0;
          if (dt > 0.0) {
            double rate = (press_count - 1) / dt;
            printf("\nButton %d final result: %.2f Hz (presses: %u over %.2fs)\n",
                   active_button, rate, press_count, dt);
          } else {
            printf("\nButton %d: not enough time to estimate turbo rate.\n",
                   active_button);
          }
        } else {
          // Only one or zero presses: not meaningful
          printf("\nButton %d: not enough pulses to measure turbo.\n",
                 active_button);
        }

        active_button = -1;
        press_count = 0;
      }
    }

    SDL_Delay(1);
  }

  SDL_JoystickClose(joy);
  SDL_Quit();
  return 0;
}
