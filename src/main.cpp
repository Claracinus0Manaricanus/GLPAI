#include "include/cm_math/operations.hpp"
#include "include/physics/collisions.hpp"
#include "include/types/physics.hpp"
#include "include/utility/printUtil.hpp"
#include "rendering/opengl/include/cm_opengl.hpp"
#include <cstdio>

int main(int argc, char **arg) {
  printf("starting\n");

  RayHit out = {{0, 0, 0}, {0, 0, 0}, 0};
  Ray ray = {{0, 10, 0}, {0, -1, 0}};

  int collided = Physics::checkCollisionRaySurface(ray, {{0, 1, 0}, 0}, &out);

  Triangle triangle;
  triangle.vertices[0] = {{0, 0, 1}, {0, 1, 0}, {1, 1, 1, 1}, {0, 0}};

  triangle.vertices[1] = {{-1, 0, -1}, {0, 1, 0}, {1, 1, 1, 1}, {0, 0}};

  triangle.vertices[2] = {{1, 0, -1}, {0, 1, 0}, {1, 1, 1, 1}, {0, 0}};

  collided = Physics::checkCollisionRayTriangle(ray, triangle, &out);

  if (collided) {
    printf("Position: x:%f y:%f z:%f\nNormal: x:%f y:%f z:%f\ntConstant: %f\n",
           out.hitPosition.x, out.hitPosition.y, out.hitPosition.z,
           out.hitNormal.x, out.hitNormal.y, out.hitNormal.z, out.tConstant);
  }

  printf("%f\n", Vector::Length(Vector::Normalize(((Vec3){1, 1, 1}))));

  Vec3 a = {1, 1, 1};
  println(a);

  printf("Area: %f\n", Primitive::Area(triangle));

  return 0;
}
