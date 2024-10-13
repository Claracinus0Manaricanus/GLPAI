#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>

#include "window.hpp"

// constructors
cm_glfw_Window::cm_glfw_Window(int width, int height) {
  if (glfwInit() == GLFW_FALSE) {
    latestError = glfwGetError((const char**)&errorMessage);
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  winHandler = glfwCreateWindow(width, height, "default", NULL, NULL);
  if (winHandler == NULL) {
    latestError = glfwGetError((const char**)&errorMessage);
    return;
  }

  // we trust in god with this one
  glfwMakeContextCurrent(winHandler);

  if ((latestError = glewInit()) != GLEW_OK) {
    errorMessage = (char*)glewGetErrorString(latestError);
  }
}

// destructors
cm_glfw_Window::~cm_glfw_Window() {
  glfwDestroyWindow(winHandler);
  glfwTerminate();
}

// getters
int cm_glfw_Window::shouldClose() { return glfwWindowShouldClose(winHandler); }

IVec2 cm_glfw_Window::getWindowResolution() {
  glfwGetFramebufferSize(winHandler, &resolution.x, &resolution.y);

  return resolution;
}

float cm_glfw_Window::getAspectRatio() {
  getWindowResolution();
  return (float)resolution.y / resolution.y;
}

IVec2 cm_glfw_Window::getCursorPos() {
  glfwGetCursorPos(winHandler, &cursorPosX, &cursorPosY);

  return (IVec2){(int)floor(cursorPosX), (int)floor(cursorPosY)};
}

Vec2 cm_glfw_Window::getCursorPosNormalized() {
  getWindowResolution();
  getCursorPos();

  return (Vec2){(float)cursorPosX / resolution.x,
                -(float)cursorPosY / resolution.y};
}

int cm_glfw_Window::getKey(uint32_t keycode) {
  return glfwGetKey(winHandler, keycode) == GLFW_PRESS;
}

// setters
void cm_glfw_Window::setShouldClose(int winShouldClose) {
  glfwSetWindowShouldClose(winHandler, winShouldClose);
}

int cm_glfw_Window::showCursor(int toggle) {
  if (toggle) {
    glfwSetInputMode(winHandler, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  } else {
    glfwSetInputMode(winHandler, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
  }
  return 0;
}

void setGrab(int grabbed) {
  // this function is not implemented
}

int cm_glfw_Window::setRelativeMouseMode(int enabled) {
  if (enabled) {
    glfwSetInputMode(winHandler, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  } else {
    glfwSetInputMode(winHandler, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }

  return 0;
}

void cm_glfw_Window::setMousePos(float x, float y) {
  getWindowResolution();
  glfwSetCursorPos(winHandler, x * resolution.x, y * resolution.y);
}

// utility
int cm_glfw_Window::updateScreen() {
  glfwSwapBuffers(winHandler);

  return 0;
}

// event handling
int cm_glfw_Window::checkEvents() {
  glfwPollEvents();
  return 0;
}

// error control
int cm_glfw_Window::isFine() { return latestError == GLFW_NO_ERROR; }
const char* cm_glfw_Window::getError() { return errorMessage; }

// opengl specific stuff
void cm_glfw_Window::setClearColor(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
}

void cm_glfw_Window::clearScreen() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void cm_glfw_Window::updateViewport() {
  getWindowResolution();
  glViewport(0, 0, resolution.x, resolution.y);
}

int cm_glfw_Window::setSwapInterval(int interval) {
  glfwSwapInterval(interval);
  return 0;
}
