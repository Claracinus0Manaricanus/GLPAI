#include "transform.hpp"
#include <math.h>

// constructors
Transform::Transform() {
  position = {0, 0, 0};
  rotation = {0, 0, 0};
  scale = {1, 1, 1};

  forward = {0, 0, -1}; // negative for math reasons
  right = {1, 0, 0};
  up = {0, 1, 0};

  calculateOVM();
}

Transform::Transform(TransformData data) {
  position = data.position;
  rotation = data.rotation;
  scale = data.scale;

  forward = {0, 0, -1}; // negative for math reasons
  right = {1, 0, 0};
  up = {0, 1, 0};

  calculateOVM();
}

// setters
void Transform::setPosition(Vec3 position) {
  this->position = position;
  calculateOVM();
}

void Transform::move(Vec3 movement) {
  this->position += movement;
  calculateOVM();
}

void Transform::localMove(Vec3 movement) {
  this->position += rotMat * movement;
  calculateOVM();
}

void Transform::setRotation(Vec3 rotation) {
  this->rotation = rotation;
  calculateDirections();
  calculateOVM();
}

void Transform::rotate(Vec3 rotation) {
  this->rotation += rotation;
  calculateDirections();
  calculateOVM();
}

// getters
Vec3& Transform::getPosition() { return position; }
Vec3& Transform::getRotation() { return rotation; }
Vec3& Transform::getScale() { return scale; }

Vec3& Transform::getForward() { return forward; }
Vec3& Transform::getRight() { return right; }
Vec3& Transform::getUp() { return up; }

Mat4& Transform::getOVM() { return OVM; }

// utility
void Transform::calculateDirections() {
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

  forward = rotX * rotY * (Vec3){0, 0, -1};
  right = rotX * rotY * (Vec3){1, 0, 0};
  up = rotX * rotY * (Vec3){0, 1, 0};
}

void Transform::calculateOVM() {
  rotMat = {{
      {right.x, up.x, -forward.x, 0},
      {right.y, up.y, -forward.y, 0},
      {right.z, up.z, -forward.z, 0},
      {0, 0, 0, 1},
  }};

  Mat4 tra = {{
      {1, 0, 0, position.x},
      {0, 1, 0, position.y},
      {0, 0, 1, position.z},
      {0, 0, 0, 1},
  }};

  OVM = tra * rotMat;
}