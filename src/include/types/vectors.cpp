#include "vectors.hpp"

// Vec2

Vec2 Vec2::operator/(float c) { return {this->x / c, this->y / c}; }

Vec2& Vec2::operator+=(Vec2 a) {
  x += a.x;
  y += a.y;

  return *this;
}

Vec2& Vec2::operator-=(Vec2 a) {
  x -= a.x;
  y -= a.y;

  return *this;
}

Vec2 Vec2::operator-(Vec2 a) { return {this->x - a.x, this->y - a.y}; }

Vec2 Vec2::operator-() { return {-(this->x), -(this->y)}; }

// Vec3

Vec3 Vec3::operator/(float c) {
  return {this->x / c, this->y / c, this->z / c};
}

Vec3& Vec3::operator+=(Vec3 a) {
  x += a.x;
  y += a.y;
  z += a.z;

  return *this;
}

Vec3& Vec3::operator-=(Vec3 a) {
  x -= a.x;
  y -= a.y;
  z -= a.z;

  return *this;
}

Vec3 Vec3::operator-(Vec3 a) {
  return {this->x - a.x, this->y - a.y, this->z - a.z};
}

Vec3 Vec3::operator-() { return {-(this->x), -(this->y), -(this->z)}; }

// Vec4

Vec4 Vec4::operator/(float c) {
  return {this->x / c, this->y / c, this->z / c, this->w / c};
}

Vec4& Vec4::operator+=(Vec4 a) {
  x += a.x;
  y += a.y;
  z += a.z;
  w += a.w;

  return *this;
}

Vec4& Vec4::operator-=(Vec4 a) {
  x -= a.x;
  y -= a.y;
  z -= a.z;
  w -= a.w;

  return *this;
}

Vec4 Vec4::operator-(Vec4 a) {
  return {this->x - a.x, this->y - a.y, this->z - a.z, this->w - a.w};
}

Vec4 Vec4::operator-() {
  return {-(this->x), -(this->y), -(this->z), -(this->w)};
}

// Mat4
Mat4 Mat4::operator*(Mat4 a) {
  Mat4 tempRet = {{
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
      {0, 0, 0, 0},
  }};

  for (int i = 0; i < 4; i++) {
    tempRet.row[i].x = (row[i].x * a.row[0].x) + (row[i].y * a.row[1].x) +
                       (row[i].z * a.row[2].x) + (row[i].w * a.row[3].x);
    tempRet.row[i].y = (row[i].x * a.row[0].y) + (row[i].y * a.row[1].y) +
                       (row[i].z * a.row[2].y) + (row[i].w * a.row[3].y);
    tempRet.row[i].z = (row[i].x * a.row[0].z) + (row[i].y * a.row[1].z) +
                       (row[i].z * a.row[2].z) + (row[i].w * a.row[3].z);
    tempRet.row[i].w = (row[i].x * a.row[0].w) + (row[i].y * a.row[1].w) +
                       (row[i].z * a.row[2].w) + (row[i].w * a.row[3].w);
  }

  return tempRet;
}