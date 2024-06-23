#include "operations.hpp"
#include <cmath>

// namespace Vector

// vector products

Vec3 Vector::Cross(Vec3 a, Vec3 b) {
  return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

float Vector::Dot(Vec4 a, Vec4 b) {
  return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}
float Vector::Dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
float Vector::Dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }

// vector utilities

float Vector::Length(Vec4 a) { return sqrt(Vector::Dot(a, a)); }
float Vector::Length(Vec3 a) { return sqrt(Vector::Dot(a, a)); }
float Vector::Length(Vec2 a) { return sqrt(Vector::Dot(a, a)); }

Vec4 Vector::Normalize(Vec4 a) {
  float len;
  return ((len = Vector::Length(a)) != 0) ? (a / len) : (Vec4){0, 0, 0, 0};
}

Vec3 Vector::Normalize(Vec3 a) {
  float len;
  return ((len = Vector::Length(a)) != 0) ? (a / len) : (Vec3){0, 0, 0};
}

Vec2 Vector::Normalize(Vec2 a) {
  float len;
  return ((len = Vector::Length(a)) != 0) ? (a / len) : (Vec2){0, 0};
}

// triangle area between two vectors ( Determinant / 2 )
float Vector::Area(Vec2 a, Vec2 b) { return fabs(a.x * b.y - a.y * b.x) / 2; }
float Vector::Area(Vec3 a, Vec3 b) {
  return Vector::Length(Vector::Cross(a, b)) / 2;
}

// namsepace Primitive

float Primitive::Area(Triangle& a) {
  Vec3 v1 = a.vertices[1].position - a.vertices[0].position;
  Vec3 v2 = a.vertices[2].position - a.vertices[0].position;

  return Vector::Length(Vector::Cross(v1, v2)) / 2;
}
