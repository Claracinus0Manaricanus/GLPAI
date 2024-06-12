#ifndef CM_TYPES
#define CM_TYPES

struct IVec2 {
  int x, y;
};

struct Vec2 {
  float x, y;

  Vec2 operator/(float c);
  Vec2& operator+=(Vec2 a);
  Vec2& operator-=(Vec2 a);
  Vec2 operator-(Vec2 a);
  Vec2 operator-();
};

struct Vec3 {
  float x, y, z;

  Vec3 operator/(float c);
  Vec3& operator+=(Vec3 a);
  Vec3& operator-=(Vec3 a);
  Vec3 operator-(Vec3 a);
  Vec3 operator-();
};

struct Vec4 {
  float x, y, z, w;

  Vec4 operator/(float c);
  Vec4& operator+=(Vec4 a);
  Vec4& operator-=(Vec4 a);
  Vec4 operator-(Vec4 a);
  Vec4 operator-();
};

struct Mat4{
    Vec4 row[4];

    Mat4 operator*(Mat4 a);
};

#endif
