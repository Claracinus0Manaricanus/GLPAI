#ifndef CM_PHYSICS
#define CM_PHYSICS

#include "vectors.hpp"

typedef struct {
  Vec3 start;
  Vec3 direction;
} Ray;

typedef struct {
  Vec3 normal;
  float distance; // distance from origin
} Surface;

typedef struct {
  Vec3 hitPosition;
  Vec3 hitNormal;
  float tConstant; // calculated using rays direction vector
} RayHit;

#endif
