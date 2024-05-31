#include "vectors.hpp"

// Vec2

Vec2 Vec2::operator/(float c) { return {this->x / c, this->y / c}; }

// Vec3

Vec3 Vec3::operator/(float c) {
  return {this->x / c, this->y / c, this->z / c};
}

// Vec4

Vec4 Vec4::operator/(float c) {
  return {this->x / c, this->y / c, this->z / c, this->w / c};
}
