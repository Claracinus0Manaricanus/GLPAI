#ifndef CM_WINDOW
#define CM_WINDOW

#define CM_OPENGL 0
#define CM_VULKAN 1
#define CM_SDL_RENDERER 2

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
  // for default rendering
  SDL_Renderer *renderHandler;
  // graphics option being used (vulkan opengl and such)
  int rendererType;

public:
  // constructor
  Window(int width, int height, Uint32 flags);

  // destructor
  ~Window();

  // getters
  int shouldClose();

  // setters
  void setShouldClose(int winShouldClose);

  // utility
  int updateScreen();

  // event handling
  int checkEvents(void (*keyCallback)(Uint32 type, SDL_Keysym key));

  // error control
  int isFine();
  const char *getError();

  // opengl specific stuff

  // vulkan specific stuff
};

#endif