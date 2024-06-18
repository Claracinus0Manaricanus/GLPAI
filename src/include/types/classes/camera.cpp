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
void Camera::calculateDirections() {
  Mat4 rotX = {{
      {1, 0, 0, 0},
      {0, cos(rotation.x), sin(rotation.x), 0},
      {0, -sin(rotation.x), cos(rotation.x), 0},
      {0, 0, 0, 1},
  }};

  Mat4 rotY = {{
      {cos(rotation.y), 0, -sin(rotation.y), 0},
      {0, 1, 0, 0},
      {sin(rotation.y), 0, cos(rotation.y), 0},
      {0, 0, 0, 1},
  }};

  rotMat = rotX * rotY;

  // Because camera and worlds rotations are transposes of each other
  // We take the transpose of this matrix for direction vectors
  right = {rotMat.row[0].x, rotMat.row[0].y, -rotMat.row[0].z};
  up = {rotMat.row[1].x, rotMat.row[1].y, -rotMat.row[1].z};
  forward = {rotMat.row[2].x, rotMat.row[2].y, -rotMat.row[2].z};
}

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