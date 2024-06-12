#include "camera.hpp"

// constructors
Camera::Camera(CameraData caData) : Transform() { this->fov = caData.fov; }

Camera::Camera(CameraData caData, TransformData trData) : Transform(trData) {
  this->fov = caData.fov;
}

//CVM generation
void Camera::calculateOVM(){
  Mat4 rot = {{
      {right.x, up.x, -forward.x, 0},
      {right.y, up.y, -forward.y, 0},
      {right.z, up.z, -forward.z, 0},
      {0, 0, 0, 1},
  }};

  Mat4 tra = {{
      {1, 0, 0, -position.x},
      {0, 1, 0, -position.y},
      {0, 0, 1, -position.z},
      {0, 0, 0, 1},
  }};

  OVM = tra * rot;
}