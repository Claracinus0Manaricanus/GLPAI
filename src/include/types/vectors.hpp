#ifndef CM_TYPES
#define CM_TYPES

struct IVec2{
  int x,y;
};

struct Vec2 {
  float x, y;

  Vec2 operator/(float c);
  Vec2 operator-(Vec2 a);
};

struct Vec3 {
  float x, y, z;

  Vec3 operator/(float c);
  Vec3 operator-(Vec3 a);
};

struct Vec4 {
  float x, y, z, w;

  Vec4 operator/(float c);
  Vec4 operator-(Vec4 a);
};

#endif
