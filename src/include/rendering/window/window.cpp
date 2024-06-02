#include "window.hpp"

// constructors
Window::Window(int width, int height, Uint32 flags) {
  // instance vars
  winShouldClose = 0;
  renderHandler = NULL;

  // SDL init
  latestError = SDL_Init(SDL_INIT_EVERYTHING);
  if (latestError != 0)
    return;

  // window creation
  this->width = width;
  this->height = height;

  winHandler = SDL_CreateWindow("default", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, width, height, flags);
  if (winHandler == NULL) {
    latestError = -1;
    return;
  }

  // renderer creation or setting up for other renderers
  if (flags & SDL_WINDOW_OPENGL) {
    rendererType = CM_OPENGL;
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

// setters
void Window::setShouldClose(int winShouldClose) {
  this->winShouldClose = winShouldClose;
}

// utility
void Window::updateScreen() {
  switch (rendererType) {
  case CM_OPENGL:
    SDL_GL_SwapWindow(winHandler);
    break;
  case CM_VULKAN:
    break;
  case CM_SDL_RENDERER:
    SDL_RenderPresent(renderHandler);
    break;
  }
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