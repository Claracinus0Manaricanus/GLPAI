#include "collisions.hpp"
#include "../cm_math/operations.hpp"
#include "types/primitives.hpp"
#include <cmath>

// !! CAUTION !!
// currently these functions are unoptimized and in a test stage

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
int Physics::checkCollisionRaySurface(Ray& ray, Surface surface, RayHit* out) {

  float d = Vector::Dot(surface.normal, ray.direction);
  if (d == 0)
    return 0;

  float t = -(Vector::Dot(surface.normal, ray.start) + surface.distance) / d;
  if (t < 0) // if t is negative the ray has to go backwards to hit the surface
    return 0;

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
int Physics::checkCollisionRayTriangle(Ray& ray, Triangle triangle,
                                       RayHit* out) {
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
  float totalArea =
      Vector::Area(triangle.vertices[1].position -
                       triangle.vertices[0].position,
                   out->hitPosition - triangle.vertices[0].position) +
      Vector::Area(triangle.vertices[2].position -
                       triangle.vertices[1].position,
                   out->hitPosition - triangle.vertices[1].position) +
      Vector::Area(triangle.vertices[0].position -
                       triangle.vertices[2].position,
                   out->hitPosition - triangle.vertices[2].position);

  float areaCheck = totalArea - Primitive::Area(triangle);
  if (areaCheck > 0.1f)
    return 0;

  return 1;
}

// uses checkCollisionRayTriangle to iterate over every
// triangle on a mesh and find the closest intersection
int Physics::checkCollisionRayMesh(Ray& ray, Mesh& mesh, Transform& transform,
                                   RayHit* out) {
  Triangle temp;
  std::vector<Vertex> vertices = mesh.getAllVertices();
  std::vector<uint32_t> indices = mesh.getIndexBuffer();

  RayHit tempClosest;
  int collided = 0;
  int collidedGlobal = 0;

  transform.calculateOVM();
  Mat4& ovm = transform.getOVM();

  for (int i = 0; i < vertices.size(); i++) {
    // we do this tempVec4 thingy because translation is also important for
    // positions
    Vec4 tempVec4 = ovm * (Vec4){vertices[i].position.x, vertices[i].position.y,
                                 vertices[i].position.z, 1};
    vertices[i].position = (Vec3){tempVec4.x, tempVec4.y, tempVec4.z};

    vertices[i].normal = Vector::Normalize(ovm * vertices[i].normal);
  }

  for (int i = 0; i < indices.size(); i += 3) {
    temp.vertices[0] = vertices[indices[i]];
    temp.vertices[1] = vertices[indices[i + 1]];
    temp.vertices[2] = vertices[indices[i + 2]];

    collided = checkCollisionRayTriangle(ray, temp, out);

    if (collided) {
      if (collidedGlobal == 0) {
        tempClosest = *out;
        collidedGlobal = 1;
      } else if (tempClosest.tConstant > out->tConstant) {
        tempClosest = *out;
      }
    }
  }

  if (collidedGlobal)
    (*out) = tempClosest;

  return collidedGlobal;
}

int Physics::checkCollisionRaySphere(Ray& ray, Sphere sphere, RayHit* out) {
  // adjusting the rays position in space according to the spheres location
  Vec3 newStart = ray.start - sphere.position;

  // Uses quadratic formula so we find a b and c first in the representation
  // a*t^2 + b*t + c
  float a = powf(ray.direction.x, 2) + powf(ray.direction.y, 2) +
            powf(ray.direction.z, 2);
  float b =
      2 * ((newStart.x * ray.direction.x) + (newStart.y * ray.direction.y) +
           (newStart.z * ray.direction.z));
  float c = powf(newStart.x, 2) + powf(newStart.y, 2) + powf(newStart.z, 2) -
            powf(sphere.radius, 2);

  float delta = powf(b, 2) - (4 * a * c);
  // if delta is negative there are no real solutions
  if (delta < 0)
    return 0;

  // the negative version of the formula gives the closest collision
  float negative = (-b - sqrt(delta)) / (2 * a);

  // if this value is negative positive version of the formula should be used
  // but I didn't implement it right now because this is true only if you are
  // inside the sphere
  if (negative < 0)
    return 0;

  // we don't use newStart position here because we need the world space not the
  // ray space we created
  out->hitPosition = ray.start + (ray.direction * negative);

  // we use newStart here because we need the sphere to be at origin to
  // calculate its normals this way and in the ray space we created the sphere
  // is at the origin
  out->hitNormal = Vector::Normalize(newStart + (ray.direction * negative));

  out->tConstant = negative;

  return 1;
}

int Physics::checkCollisionSphereSphere(Sphere& sphere0, Sphere& sphere1,
                                        SphereHit* out) {
  // the distance between spheres is needed to check if they collide
  float distance_between_spheres =
      Vector::Length(sphere0.position - sphere1.position);
  float radius_sum = sphere0.radius + sphere1.radius;
  if (distance_between_spheres > radius_sum) {
    return 0;
  }

  out->hitPosition = (sphere0.position * sphere0.radius / radius_sum) +
                     (sphere1.position * sphere1.radius / radius_sum);
  out->hitNormal = Vector::Normalize(sphere1.position - sphere0.position);
  out->overlap_distance = radius_sum - distance_between_spheres;

  return 1;
}
