#include "collisions.hpp"
#include "../cm_math/operations.hpp"

// {A, B, C} is the surfaces normal
// D is the surfaces distance from origin
// {x, y, z} is rays start position
// {a, b, c} is rays direction
// t is the scaler constant that gets multiplied with
// rays direction and added to rays start position to
// get their intersection
//
// The Formula:
// t = -(A*x + B*y + C*z + D) / (A*a + B*b + C*c)
//
// returns 1 if intersects
// 0 otherwise
// outputs a RayHit on intersection to RayHit* out
int Physics::checkCollisionRaySurface(Ray ray, Surface surface, RayHit *out) {

  float d = cm_Dot(surface.normal, ray.direction);
  if (d == 0)
    return 0;

  float t = -(cm_Dot(surface.normal, ray.start) + surface.distance) / d;
  Vec3 intersection = {ray.start.x + ray.direction.x * t,
                       ray.start.y + ray.direction.y * t,
                       ray.start.z + ray.direction.z * t};

  out->hitPosition = intersection;
  out->hitNormal = surface.normal;
  out->tConstant = t;

  return 1;
}

// uses checkCollisionRaySurface to see where ray hits
// on the triangles plane then uses that point to check
// if intersection point is in the triangle
//
// return 1 if intersects
// 0 otherwise
// outputs a RayHit on intersection to RayHit* out
int Physics::checkCollisionRayTriangle(Ray ray, Triangle triangle,
                                       RayHit *out) {
  // finding surfaces distance from origin using the triangle
  float distance =
      -(triangle.vertices[0].normal.x * triangle.vertices[0].position.x +
        triangle.vertices[0].normal.y * triangle.vertices[0].position.y +
        triangle.vertices[0].normal.z * triangle.vertices[0].position.z);

  // check rays collision with the surface triangle is on
  int collided = checkCollisionRaySurface(
      ray, {triangle.vertices[0].normal, distance}, out);
  if (!collided) // no need to continue if ray doesn't intersect the surface
    return 0;

  // check if intersection point is in the triangle

  return 1;
}
