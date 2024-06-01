#ifndef CM_OPERATIONS
#define CM_OPERATIONS

#include "../types/primitives.hpp"
#include "../types/vectors.hpp"

// This file includes operations for
// vectors primitives and such
// it uses namespaces to divide operations
// into categories

namespace Vector {

// vector products
Vec3 Cross(Vec3 a, Vec3 b);
float Dot(Vec3 a, Vec3 b);
float Dot(Vec2 a, Vec2 b);

// vector utilit (length and such)
float Length(Vec3 a);
float Length(Vec2 a);

// normalizers
Vec3 Normalize(Vec3 a);
Vec2 Normalize(Vec2 a);

} // namespace Vector

namespace Primitive {

float Area(Triangle &a);

} // namespace Primitive

#endif
