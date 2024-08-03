#include "vectors.hpp"

// Vec2

float& Vec2::operator[](int index) { return ((float*)this)[index]; }

Vec2 Vec2::operator/(float c) { return {this->x / c, this->y / c}; }

Vec2& Vec2::operator*=(Vec2 a) {
  x *= a.x;
  y *= a.y;

  return *this;
}

Vec2 Vec2::operator*(Vec2 a) { return (Vec2){x * a.x, y * a.y}; }

Vec2& Vec2::operator+=(Vec2 a) {
  x += a.x;
  y += a.y;

  return *this;
}

Vec2 Vec2::operator+(Vec2 a) {
  Vec2 temp = {x + a.x, y + a.y};
  return temp;
}

Vec2& Vec2::operator-=(Vec2 a) {
  x -= a.x;
  y -= a.y;

  return *this;
}

Vec2 Vec2::operator-(Vec2 a) { return {this->x - a.x, this->y - a.y}; }

Vec2 Vec2::operator-() { return {-(this->x), -(this->y)}; }

Vec2 Vec2::operator*(float a) { return (Vec2){x * a, y * a}; }

// Vec3

float& Vec3::operator[](int index) { return ((float*)this)[index]; }

Vec3 Vec3::operator/(float c) {
  return {this->x / c, this->y / c, this->z / c};
}

Vec3& Vec3::operator*=(Vec3 a) {
  x *= a.x;
  y *= a.y;
  z *= a.z;

  return *this;
}

Vec3 Vec3::operator*(Vec3 a) { return (Vec3){x * a.x, y * a.y, z * a.z}; }

Vec3& Vec3::operator+=(Vec3 a) {
  x += a.x;
  y += a.y;
  z += a.z;

  return *this;
}

Vec3 Vec3::operator+(Vec3 a) {
  Vec3 temp = {x + a.x, y + a.y, z + a.z};
  return temp;
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

Vec3 Vec3::operator*(float a) { return (Vec3){x * a, y * a, z * a}; }

// Vec4

float& Vec4::operator[](int index) { return ((float*)this)[index]; }

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

Vec4 Vec4::operator+(Vec4 a) {
  Vec4 temp = {x + a.x, y + a.y, z + a.z, w + a.w};
  return temp;
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

Vec4 Vec4::operator*(float a) { return (Vec4){x * a, y * a, z * a, w * a}; }

Vec4 Vec4::operator*(Vec4 a) {
  Vec4 temp;

  temp.x = w * a.x + a.w * x + y * a.z - a.y * z;
  temp.y = w * a.y + a.w * y + a.x * z - x * a.z;
  temp.z = w * a.z + a.w * z + x * a.y - a.x * y;
  temp.w = w * a.w - x * a.x - y * a.y - z * a.z;

  return temp;
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

Vec4 Mat4::operator*(Vec4 a) {
  Vec4 temp = {0, 0, 0, 0};

  temp.x = row[0].x * a.x + row[0].y * a.y + row[0].z * a.z + row[0].w * a.w;
  temp.y = row[1].x * a.x + row[1].y * a.y + row[1].z * a.z + row[1].w * a.w;
  temp.z = row[2].x * a.x + row[2].y * a.y + row[2].z * a.z + row[2].w * a.w;
  temp.w = row[3].x * a.x + row[3].y * a.y + row[3].z * a.z + row[3].w * a.w;

  return temp;
}

Vec3 Mat4::operator*(Vec3 a) {
  Vec3 temp = {0, 0, 0};

  temp.x = row[0].x * a.x + row[0].y * a.y + row[0].z * a.z;
  temp.y = row[1].x * a.x + row[1].y * a.y + row[1].z * a.z;
  temp.z = row[2].x * a.x + row[2].y * a.y + row[2].z * a.z;

  return temp;
}

void Mat4::transpose() {
  for (int i = 0; i < 3; i++) {
    for (int k = i + 1; k < 4; k++) {
      float temp = row[k][i];
      row[k][i] = row[i][k];
      row[i][k] = temp;
    }
  }
}
