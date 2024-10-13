#ifndef CM_GLFW_WINDOW
#define CM_GLFW_WINDOW

#include <GLFW/glfw3.h>
#include <types/vectors.hpp>

class cm_glfw_Window {
protected:
  // instance vars
  // error handling
  int latestError;
  char* errorMessage;
  // window related
  GLFWwindow* winHandler;
  // graphics option being used (vulkan opengl and such)
  int rendererType;
  // window data
  IVec2 resolution = {0, 0};
  double cursorPosX = 0;
  double cursorPosY = 0;

public:
  // constructor
  cm_glfw_Window(int width, int height);

  // destructor
  ~cm_glfw_Window();

  // getters
  int shouldClose();
  IVec2 getWindowResolution();
  float getAspectRatio();
  IVec2 getCursorPos();
  Vec2 getCursorPosNormalized();
  int getKey(uint32_t keycode);

  // setters
  void setShouldClose(int winShouldClose);
  int showCursor(int toggle);
  void setGrab(int grabbed); // not implemented
  int setRelativeMouseMode(int enabled);
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
