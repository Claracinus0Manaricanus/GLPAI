#include "vectors.hpp"

// Vec2

Vec2 Vec2::operator/(float c) { return {this->x / c, this->y / c}; }

Vec2 Vec2::operator-(Vec2 a) { return {this->x - a.x, this->y - a.y}; }

// Vec3

Vec3 Vec3::operator/(float c) {
  return {this->x / c, this->y / c, this->z / c};
}

Vec3 Vec3::operator-(Vec3 a) {
  return {this->x - a.x, this->y - a.y, this->z - a.z};
}

// Vec4

Vec4 Vec4::operator/(float c) {
  return {this->x / c, this->y / c, this->z / c, this->w / c};
}

Vec4 Vec4::operator-(Vec4 a) {
  return {this->x - a.x, this->y - a.y, this->z - a.z, this->w - a.w};
}
