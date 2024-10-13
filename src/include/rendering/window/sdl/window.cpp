#include "window.hpp"
#include <GL/glew.h>
#include <SDL2/SDL_keyboard.h>
#include <cassert>
#include <cstdio>

// constructors
cm_sdl_Window::cm_sdl_Window(int width, int height, uint32_t flags) {
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
cm_sdl_Window::~cm_sdl_Window() {
  if (rendererType == CM_SDL_RENDERER)
    SDL_DestroyRenderer(renderHandler);

  // this function caused SIGSEGV on wayland, I don't know why
  SDL_DestroyWindow(winHandler);
  SDL_Quit();
}

// getters
int cm_sdl_Window::shouldClose() { return winShouldClose; }

IVec2 cm_sdl_Window::getWindowResolution() {
  int w = 0, h = 0;
  SDL_GetWindowSize(winHandler, &w, &h);

  return (IVec2){w, h};
}

float cm_sdl_Window::getAspectRatio() {
  IVec2 wRes = getWindowResolution();
  return (float)wRes.y / (float)wRes.x;
}

IVec2 cm_sdl_Window::getCursorPos() {
  int x = 0, y = 0;
  SDL_GetMouseState(&x, &y);

  return (IVec2){x, y};
}

Vec2 cm_sdl_Window::getCursorPosNormalized() {
  int x = 0, y = 0, w = 0, h = 0;
  SDL_GetMouseState(&x, &y);
  SDL_GetWindowSize(winHandler, &w, &h);

  return (Vec2){2 * ((x + 1.0f) / w) - 1, 1 - 2 * ((y + 1.0f) / h)};
}

int cm_sdl_Window::getKey(uint32_t keycode) {
  return SDL_GetKeyboardState(NULL)[keycode];
}

// setters
void cm_sdl_Window::setShouldClose(int winShouldClose) {
  this->winShouldClose = winShouldClose;
}

int cm_sdl_Window::showCursor(int toggle) { return SDL_ShowCursor(toggle); }

void cm_sdl_Window::setGrab(SDL_bool grabbed) {
  SDL_SetWindowGrab(winHandler, grabbed);
}

int cm_sdl_Window::setRelativeMouseMode(SDL_bool enabled) {
  return SDL_SetRelativeMouseMode(enabled);
}

void cm_sdl_Window::setMousePos(float x, float y) {
  IVec2 res = getWindowResolution();
  int mouseX = ((x + 1.0f) / 2.0f) * res.x;
  int mouseY = ((-y + 1.0f) / 2.0f) * res.y;

  SDL_WarpMouseInWindow(winHandler, mouseX, mouseY);
}

// utility
int cm_sdl_Window::updateScreen() {
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
int cm_sdl_Window::checkEvents() {
  int retOfPollEvent = SDL_PollEvent(&event);

  if (retOfPollEvent == 0)
    return 0;

  switch (event.type) {
  case SDL_QUIT:
    winShouldClose = 1;
    break;

  case SDL_KEYDOWN:
    break;

  case SDL_KEYUP:
    break;

  case SDL_TEXTINPUT:
    // WIP
    break;
  }

  return retOfPollEvent;
}

// error control
int cm_sdl_Window::isFine() { return (latestError == 0); }

const char* cm_sdl_Window::getError() { return SDL_GetError(); }

// opengl specific stuff
void cm_sdl_Window::setClearColor(float r, float g, float b, float a) {
  if (rendererType == CM_OPENGL)
    glClearColor(r, g, b, a);
}

void cm_sdl_Window::clearScreen() {
  if (rendererType == CM_OPENGL)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void cm_sdl_Window::updateViewport() {
  IVec2 winRes = getWindowResolution();
  if (rendererType == CM_OPENGL)
    glViewport(0, 0, winRes.x, winRes.y);
}

int cm_sdl_Window::setSwapInterval(int interval) {
  return SDL_GL_SetSwapInterval(interval);
}
