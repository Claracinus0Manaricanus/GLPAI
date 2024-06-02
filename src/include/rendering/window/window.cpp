#include "window.hpp"

// constructors
Window::Window(int width, int height, Uint32 flags) {
  // instance vars
  winShouldClose = 0;

  latestError = SDL_Init(SDL_INIT_EVERYTHING);
  if (latestError != 0)
    return;

  this->width = width;
  this->height = height;

  winHandler = SDL_CreateWindow("default", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, width, height, flags);
  if (winHandler == NULL) {
    latestError = -1;
    return;
  }
}

// destructor
Window::~Window() { SDL_DestroyWindow(winHandler); }

// getters
int Window::shouldClose() { return winShouldClose; }

// setters
void Window::setShouldClose(int winShouldClose) {
  this->winShouldClose = winShouldClose;
}

// event handling
void Window::checkEvents() {
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT:
    winShouldClose = 1;
    break;
  case SDL_KEYDOWN:
    break;
  case SDL_KEYUP:
    break;
  }
}

// error control
int Window::isFine() { return (latestError == 0); }

const char *Window::getError() { return SDL_GetError(); }