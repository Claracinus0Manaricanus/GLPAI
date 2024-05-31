#include "operations.hpp"
#include <cmath>

// vector products

Vec3 Vector::Cross(Vec3 a, Vec3 b) {
  return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

float Vector::Dot(Vec3 a, Vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
float Vector::Dot(Vec2 a, Vec2 b) { return a.x * b.x + a.y * b.y; }

// vector utilities

float Vector::Length(Vec3 a) { return sqrt(Vector::Dot(a, a)); }
float Vector::Length(Vec2 a) { return sqrt(Vector::Dot(a, a)); }

// vector utilities

Vec3 Vector::Normalize(Vec3 a) { return (a / Vector::Length(a)); }
