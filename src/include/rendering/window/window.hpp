#ifndef CM_WINDOW
#define CM_WINDOW

#define CM_OPENGL 0
#define CM_VULKAN 1
#define CM_SDL_RENDERER 2

#include "../../types/vectors.hpp"
#include <SDL2/SDL.h>

class Window {
protected:
  // instance vars
  int latestError;
  // window related
  SDL_Window* winHandler;
  // event handling
  SDL_Event event;
  int winShouldClose;
  // for default rendering
  SDL_Renderer* renderHandler;
  // graphics option being used (vulkan opengl and such)
  int rendererType;

public:
  // constructor
  Window(int width, int height, uint32_t flags);

  // destructor
  ~Window();

  // getters
  int shouldClose();
  IVec2 getWindowResolution();
  float getAspectRatio();
  IVec2 getCursorPos();
  Vec2 getCursorPosNormalized();
  const uint8_t* getKeyboardState();
  uint32_t getTicks();
  uint32_t getTicks64();

  // setters
  void setShouldClose(int winShouldClose);
  int showCursor(int toggle);
  void setGrab(SDL_bool grabbed);
  int setRelativeMouseMode(SDL_bool enabled);
  void setMousePos(float x, float y);

  // utility
  int updateScreen();

  // event handling
  int checkEvents();

  // error control
  int isFine();
  const char* getError();

  // opengl specific stuff
  void setClearColor(float r, float g, float b, float a);
  void clearScreen();
  void updateViewport();
  int setSwapInterval(int interval);

  // vulkan specific stuff
};

#endif
