#ifndef CM_PRIMITIVES
#define CM_PRIMITIVES

#include "vectors.hpp"

typedef struct {
  Vec3 position;
  Vec3 normal;
  Vec4 Color;
  Vec2 uv;
} Vertex;

typedef struct {
  Vertex vertices[3];
} Triangle;

#endif
