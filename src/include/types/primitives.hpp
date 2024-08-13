#ifndef CM_PRIMITIVES
#define CM_PRIMITIVES

#include "vectors.hpp"

typedef struct {
  Vec3 position;
  Vec3 normal;
  Vec2 uv;
} Vertex;

typedef struct {
  Vertex vertices[3];
} Triangle;

typedef struct {
  Vec3 position;   // position in 3D space
  Vec3 dimensions; // half; width, height, depth (x, y, z) so the actual values
                   // are twice these
} Box;

typedef struct {
  Vec3 position;
  float radius;
} Sphere;

#endif
