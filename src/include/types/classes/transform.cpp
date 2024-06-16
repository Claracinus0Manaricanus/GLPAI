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

  rotMat = rotX * rotY;

  // I have no idea WTF is going on here
  // Even though I wrote the math down
  // It works as if this was the transpose of
  // the rotation matrix and that is what I don't understand
  right = {rotMat.row[0].x, rotMat.row[0].y, -rotMat.row[0].z};
  up = {rotMat.row[1].x, rotMat.row[1].y, -rotMat.row[1].z};
  forward = {rotMat.row[2].x, rotMat.row[2].y, -rotMat.row[2].z};
}

void Transform::calculateOVM() {
  Mat4 tra = {{
      {1, 0, 0, position.x},
      {0, 1, 0, position.y},
      {0, 0, 1, position.z},
      {0, 0, 0, 1},
  }};

  OVM = tra * rotMat;
}