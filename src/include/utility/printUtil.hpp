#ifndef CM_PRINT_UTIL
#define CM_PRINT_UTIL

#include "../types/classes/transform.hpp"
#include "../types/physics.hpp"
#include "../types/primitives.hpp"
#include "../types/vectors.hpp"

// vectors
void print(Vec2& a);
void println(Vec2& a);
void print(Vec3& a);
void println(Vec3& a);
void print(Vec4& a);
void println(Vec4& a);

// matrices
void print(Mat4& a);

// physics
void print(Ray& a);
void print(RayHit& a);

// classes
void print(Transform& tr);

// primitives
void print(Vertex& a);
void print(Triangle& a);
void print(Box& a);

#endif
