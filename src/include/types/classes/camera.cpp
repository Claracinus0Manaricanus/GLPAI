#include "camera.hpp"
#include <math.h>

// constructors
Camera::Camera() : Transform() { this->fov = 60; }

Camera::Camera(CameraData caData) : Transform(caData.trData) {
  this->fov = caData.fov;
}

// CVM generation
void Camera::calculateOVM() {
  rotMat = {{
      {right.x, up.x, -forward.x, 0},
      {right.y, up.y, -forward.y, 0},
      {right.z, up.z, -forward.z, 0},
      {0, 0, 0, 1},
  }};

  Mat4 tra = {{
      {1, 0, 0, -position.x},
      {0, 1, 0, -position.y},
      {0, 0, -1, position.z}, // negative because of opengl
      {0, 0, 0, 1},
  }};

  Mat4 proj = {{
      {tan(fov), 0, 0, 0},
      {0, tan(fov), 0, 0},
      {0, 0, 1, -0.003f},
      {0, 0, 1, 0},
  }};

  OVM = proj * rotMat * tra;
}