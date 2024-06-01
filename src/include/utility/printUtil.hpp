#ifndef CM_PRINT_UTIL
#define CM_PRINT_UTIL

#include "../types/physics.hpp"
#include "../types/primitives.hpp"
#include "../types/vectors.hpp"

// vectors
void print(Vec2 &a);
void println(Vec2 &a);
void print(Vec3 &a);
void println(Vec3 &a);
void print(Vec4 &a);
void println(Vec4 &a);

// physics
void print(Ray &a);
void print(RayHit &a);

// primitives

void print(Vertex &a);
void print(Triangle &a);

#endif
