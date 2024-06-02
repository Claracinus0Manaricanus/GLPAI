#ifndef CM_WINDOW
#define CM_WINDOW

#include <SDL2/SDL.h>

class Window {
private:
  // instance vars
  int latestError;
  // window related
  int width, height;
  SDL_Window *winHandler;
  // event handling
  SDL_Event event;
  int winShouldClose;

public:
  // constructor
  Window(int width, int height, Uint32 flags);

  // destructor
  ~Window();

  // getters
  int shouldClose();

  // setters
  void setShouldClose(int winShouldClose);

  // event handling
  void checkEvents();

  // error control
  int isFine();
  const char *getError();
};

#endif