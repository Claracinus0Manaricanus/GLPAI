#ifndef CM_TYPES
#define CM_TYPES

struct Vec2 {
  float x, y;

  Vec2 operator/(float c);
};

struct Vec3 {
  float x, y, z;

  Vec3 operator/(float c);
};

struct Vec4 {
  float x, y, z, w;

  Vec4 operator/(float c);
};

#endif
