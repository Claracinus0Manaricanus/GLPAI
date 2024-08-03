#ifndef CM_TYPES
#define CM_TYPES

struct IVec2 {
  int x, y;
};

struct Vec2 {
  float x, y;

  float& operator[](int index);
  Vec2 operator/(float c);
  Vec2& operator*=(Vec2 a);
  Vec2 operator*(Vec2 a);
  Vec2& operator+=(Vec2 a);
  Vec2 operator+(Vec2 a);
  Vec2& operator-=(Vec2 a);
  Vec2 operator-(Vec2 a);
  Vec2 operator-();
  Vec2 operator*(float a);
};

struct Vec3 {
  float x, y, z;

  float& operator[](int index);
  Vec3 operator/(float c);
  Vec3& operator*=(Vec3 a);
  Vec3 operator*(Vec3 a);
  Vec3& operator+=(Vec3 a);
  Vec3 operator+(Vec3 a);
  Vec3& operator-=(Vec3 a);
  Vec3 operator-(Vec3 a);
  Vec3 operator-();
  Vec3 operator*(float a);
};

struct Vec4 {
  float x, y, z, w;

  float& operator[](int index);
  Vec4 operator/(float c);
  Vec4& operator+=(Vec4 a);
  Vec4 operator+(Vec4 a);
  Vec4& operator-=(Vec4 a);
  Vec4 operator-(Vec4 a);
  Vec4 operator-();
  Vec4 operator*(float a);
  Vec4 operator*(Vec4 a); // quaternion multiplication
};

struct Mat4 {
  Vec4 row[4];

  Mat4 operator*(Mat4 a);
  Vec4 operator*(Vec4 a);
  Vec3 operator*(Vec3 a);

  void transpose();
};

#endif
