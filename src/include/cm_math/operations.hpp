#ifndef CM_OPERATIONS
#define CM_OPERATIONS

#include "../types/primitives.hpp"
#include "../types/vectors.hpp"

//constants
#define PI 3.14159265f
#define PI_D 3.14159265

// This file includes operations for
// vectors primitives and such
// it uses namespaces to divide operations
// into categories

namespace Vector {

// vector products
Vec3 Cross(Vec3 a, Vec3 b);
float Dot(Vec4 a, Vec4 b);
float Dot(Vec3 a, Vec3 b);
float Dot(Vec2 a, Vec2 b);

// vector utilit (length and such)
float Length(Vec4 a);
float Length(Vec3 a);
float Length(Vec2 a);

// normalizers
Vec4 Normalize(Vec4 a);
Vec3 Normalize(Vec3 a);
Vec2 Normalize(Vec2 a);

// triangle area between two vectors
float Area(Vec2 a, Vec2 b);
float Area(Vec3 a, Vec3 b);

} // namespace Vector

namespace Primitive {

float Area(Triangle &a);

} // namespace Primitive

#endif
