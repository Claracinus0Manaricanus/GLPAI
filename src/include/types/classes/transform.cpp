#include "transform.hpp"
#include <cassert>
#include <cm_math/operations.hpp>
#include <cmath>

// constructors
Transform::Transform() {
  position = {0, 0, 0};
  rotation = {0, 0, 0};
  scale = {1, 1, 1};

  forward = {0, 0, -1}; // negative for math reasons
  right = {1, 0, 0};
  up = {0, 1, 0};

  rotMat = {{
      {right.x, up.x, -forward.x, 0},
      {right.y, up.y, -forward.y, 0},
      {right.z, up.z, -forward.z, 0},
      {0, 0, 0, 1},
  }};

  calculateOVM();
}

Transform::Transform(TransformData data) {
  position = data.position;
  rotation = data.rotation;
  scale = data.scale;

  calculateDirections();

  calculateOVM();
}

// destructors
Transform::~Transform() {}

// setters
void Transform::setPosition(Vec3 position) { this->position = position; }

void Transform::move(Vec3 movement) { this->position += movement; }

void Transform::localMove(Vec3 movement) {
  this->position += right * movement.x;
  this->position += up * movement.y;
  this->position += forward * movement.z;
}

void Transform::setRotation(Vec3 rotation) {
  this->rotation = rotation;
  calculateDirections();
}

void Transform::rotate(Vec3 rotation) {
  this->rotation += rotation;
  calculateDirections();
}

void Transform::setScale(Vec3 scale) { this->scale = scale; }

// getters
Vec3& Transform::getPosition() { return position; }
Vec3& Transform::getRotation() { return rotation; }
Vec3& Transform::getScale() { return scale; }

Vec3& Transform::getForward() { return forward; }
Vec3& Transform::getRight() { return right; }
Vec3& Transform::getUp() { return up; }

Mat4& Transform::getOVM() { return OVM; }
Mat4& Transform::getRotMat() { return rotMat; }

// utility
void Transform::calculateDirections() {
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

  q1 = Vector::Normalize({0, 0, sinVal, cosVal});
  q2 = Vector::Normalize({0, 0, -sinVal, cosVal});

  fV4 = q1 * fV4 * q2;
  rV4 = q1 * rV4 * q2;
  uV4 = q1 * uV4 * q2;

  forward = {-fV4.x, -fV4.y, -fV4.z};
  right = {rV4.x, rV4.y, rV4.z};
  up = {uV4.x, uV4.y, uV4.z};

  rotMat = {{
      {rV4.x, uV4.x, fV4.x, 0},
      {rV4.y, uV4.y, fV4.y, 0},
      {rV4.z, uV4.z, fV4.z, 0},
      {0, 0, 0, 1},
  }};
}

void Transform::calculateOVM(Mat4* relativeTo) {
  Mat4 scMat = {{
      {scale.x, 0, 0, 0},
      {0, scale.y, 0, 0},
      {0, 0, scale.z, 0},
      {0, 0, 0, 1},
  }};

  Mat4 tra = {{
      {1, 0, 0, position.x},
      {0, 1, 0, position.y},
      {0, 0, 1, position.z},
      {0, 0, 0, 1},
  }};

  if (relativeTo != nullptr) {
    OVM = (*relativeTo) * tra * rotMat * scMat;
  } else {
    OVM = tra * rotMat * scMat;
  }
}
