#include "window.hpp"
#include <GL/glew.h>
#include <cassert>
#include <cstdio>

// constructors
Window::Window(int width, int height, uint32_t flags) {
  // instance vars
  winShouldClose = 0;
  renderHandler = NULL;

  // SDL init
  latestError = SDL_Init(SDL_INIT_EVERYTHING);
  if (latestError != 0)
    return;

  // window creation
  winHandler = SDL_CreateWindow("default", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, width, height, flags);
  if (winHandler == NULL) {
    latestError = -1;
    return;
  }

  // renderer creation or setting up for other renderers
  if (flags & SDL_WINDOW_OPENGL) {
    rendererType = CM_OPENGL;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_CreateContext(winHandler);
    assert(glewInit() == GLEW_OK);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
  } else if (flags & SDL_WINDOW_VULKAN) {
    rendererType = CM_VULKAN;
  } else {
    renderHandler = SDL_CreateRenderer(winHandler, -1, 0);
    if (renderHandler == NULL) {
      latestError = -1;
      return;
    }
    rendererType = CM_SDL_RENDERER;
  }
}

// destructor
Window::~Window() {
  if (rendererType == CM_SDL_RENDERER)
    SDL_DestroyRenderer(renderHandler);
  SDL_DestroyWindow(winHandler);
}

// getters
int Window::shouldClose() { return winShouldClose; }

IVec2 Window::getWindowResolution() {
  int w = 0, h = 0;
  SDL_GetWindowSize(winHandler, &w, &h);

  return (IVec2){w, h};
}

float Window::getAspectRatio() {
  IVec2 wRes = getWindowResolution();
  return (float)wRes.y / (float)wRes.x;
}

IVec2 Window::getCursorPos() {
  int x = 0, y = 0;
  SDL_GetMouseState(&x, &y);

  return (IVec2){x, y};
}

Vec2 Window::getCursorPosNormalized() {
  int x = 0, y = 0, w = 0, h = 0;
  SDL_GetMouseState(&x, &y);
  SDL_GetWindowSize(winHandler, &w, &h);

  return (Vec2){2 * ((x + 1.0f) / w) - 1, 1 - 2 * ((y + 1.0f) / h)};
}

const uint8_t* Window::getKeyboardState() { return SDL_GetKeyboardState(NULL); }

uint32_t Window::getTicks() { return SDL_GetTicks(); }

uint32_t Window::getTicks64() { return SDL_GetTicks64(); }

// setters
void Window::setShouldClose(int winShouldClose) {
  this->winShouldClose = winShouldClose;
}

int Window::showCursor(int toggle) { return SDL_ShowCursor(toggle); }

void Window::setGrab(SDL_bool grabbed) {
  SDL_SetWindowGrab(winHandler, grabbed);
}

int Window::setRelativeMouseMode(SDL_bool enabled) {
  return SDL_SetRelativeMouseMode(enabled);
}

void Window::setMousePos(float x, float y) {
  IVec2 res = getWindowResolution();
  int mouseX = ((x + 1.0f) / 2.0f) * res.x;
  int mouseY = ((-y + 1.0f) / 2.0f) * res.y;

  SDL_WarpMouseInWindow(winHandler, mouseX, mouseY);
}

// utility
int Window::updateScreen() {
  switch (rendererType) {
  case CM_OPENGL:
    SDL_GL_SwapWindow(winHandler);
    break;
  case CM_VULKAN:
    return -1;
    break;
  case CM_SDL_RENDERER:
    SDL_RenderPresent(renderHandler);
    break;
  }

  return rendererType;
}

// event handling
int Window::checkEvents(void (*keyCallback)(uint32_t type, SDL_Keysym key)) {
  int retOfPollEvent = SDL_PollEvent(&event);

  if (retOfPollEvent == 0)
    return 0;

  switch (event.type) {
  case SDL_QUIT:
    winShouldClose = 1;
    break;

  case SDL_KEYDOWN:
    if (keyCallback != NULL)
      keyCallback(event.key.type, event.key.keysym);
    break;

  case SDL_KEYUP:
    if (keyCallback != NULL)
      keyCallback(event.key.type, event.key.keysym);
    break;

  case SDL_TEXTINPUT:
    // WIP
    break;
  }

  return retOfPollEvent;
}

// error control
int Window::isFine() { return (latestError == 0); }

const char* Window::getError() { return SDL_GetError(); }

// opengl specific stuff
void Window::setClearColor(float r, float g, float b, float a) {
  assert(rendererType == CM_OPENGL);
  glClearColor(r, g, b, a);
}

void Window::clearScreen() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::updateViewport() {
  IVec2 winRes = getWindowResolution();
  glViewport(0, 0, winRes.x, winRes.y);
}

int Window::setSwapInterval(int interval) {
  return SDL_GL_SetSwapInterval(interval);
}