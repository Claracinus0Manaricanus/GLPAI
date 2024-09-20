#include "camera.hpp"
#include <cm_math/operations.hpp>
#include <math.h>

// constructors
Camera::Camera() : Transform() {
  this->fov = 60;
  this->aspectRatio = 1;
  this->near = 0.1f;
  this->far = 100.0f;
}

Camera::Camera(CameraData caData) : Transform(caData.trData) {
  this->fov = caData.fov;
  this->aspectRatio = caData.aspectRatio;
  this->near = caData.near;
  this->far = caData.far;
}

// setters
void Camera::setAspectRatio(float aspectRatio) {
  this->aspectRatio = aspectRatio;
}

// CVM generation
void Camera::calculateDirections() {
  float sinVal = sin(rotation.x / 2);
  float cosVal = cos(rotation.x / 2);

  Vec4 q1 = Vector::Normalize({sinVal, 0, 0, cosVal});
  Vec4 q2 = Vector::Normalize({-sinVal, 0, 0, cosVal});

  Vec4 fV4 = q1 * (Vec4){0, 0, 1, 0} * q2;
  Vec4 rV4 = q1 * (Vec4){1, 0, 0, 0} * q2;
  Vec4 uV4 = q1 * (Vec4){0, 1, 0, 0} * q2;

  sinVal = sin(rotation.y / 2);
  cosVal = cos(rotation.y / 2);

  q1 = Vector::Normalize({0, sinVal, 0, cosVal});
  q2 = Vector::Normalize({0, -sinVal, 0, cosVal});

  fV4 = q1 * fV4 * q2;
  rV4 = q1 * rV4 * q2;
  uV4 = q1 * uV4 * q2;

  sinVal = sin(rotation.z / 2);
  cosVal = cos(rotation.z / 2);

  q1 = Vector::Normalize(
      {fV4.x * sinVal, fV4.y * sinVal, fV4.z * sinVal, cosVal});
  q2 = Vector::Normalize(
      {-fV4.x * sinVal, -fV4.y * sinVal, -fV4.z * sinVal, cosVal});

  fV4 = q1 * fV4 * q2;
  rV4 = q1 * rV4 * q2;
  uV4 = q1 * uV4 * q2;

  forward = {-fV4.x, -fV4.y, -fV4.z};
  right = {rV4.x, rV4.y, rV4.z};
  up = {uV4.x, uV4.y, uV4.z};

  rotMat = {{
      {right.x, right.y, -right.z, 0},
      {up.x, up.y, -up.z, 0},
      {forward.x, forward.y, -forward.z, 0},
      {0, 0, 0, 1},
  }};
}

void Camera::calculateOVM() {
  Mat4 tra = {{
      {1, 0, 0, -position.x},
      {0, 1, 0, -position.y},
      {0, 0, -1, position.z}, // negative because of opengl
      {0, 0, 0, 1},
  }};

  Mat4 proj = {{
      {1 / (tan(fov * PI / 180.0f)), 0, 0, 0},
      {0, 1 / (aspectRatio * tan(fov * PI / 180.0f)), 0, 0},
      {0, 0, (far + near) / (far - near), -2 * far * near / (far - near)},
      {0, 0, 1, 0},
  }};

  OVM = proj * rotMat * tra;
}
