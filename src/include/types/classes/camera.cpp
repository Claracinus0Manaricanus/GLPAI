#include "camera.hpp"
#include <math.h>

#define PI 3.14159265f

// constructors
Camera::Camera() : Transform() {
  this->fov = 60;
  this->aspectRatio = 1;
}

Camera::Camera(CameraData caData) : Transform(caData.trData) {
  this->fov = caData.fov;
  this->aspectRatio = caData.aspectRatio;
}

// setters
void Camera::setAspectRatio(float aspectRatio) {
  this->aspectRatio = aspectRatio;
}

// CVM generation
void Camera::calculateOVM() {
  Mat4 tra = {{
      {1, 0, 0, -position.x},
      {0, 1, 0, -position.y},
      {0, 0, -1, position.z}, // negative because of opengl
      {0, 0, 0, 1},
  }};

  Mat4 proj = {{
      {1 / tan(fov * PI / 180.0f), 0, 0, 0},
      {0, 1 / (aspectRatio * tan(fov * PI / 180.0f)), 0, 0},
      {0, 0, 1, -0.003f},
      {0, 0, 1, 0},
  }};

  OVM = proj * rotMat * tra;
}