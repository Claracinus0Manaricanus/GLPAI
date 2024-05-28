#include "include/physics/collisions.hpp"
#include "include/types/physics.hpp"
#include <cstdio>

int main(int argc, char **arg) {
  printf("starting\n");

  RayHit out = {{0, 0, 0}, {0, 0, 0}, 0};
  Ray ray = {{0, 10, 0}, {0, -1, 0}};

  int collided = Physics::checkCollisionRaySurface(ray, {{0, 1, 0}, 0}, &out);

  Triangle triangle;
  triangle.vertices[0] = {{1, 0, 1}, {0, 1, 0}, {1, 1, 1, 1}, {0, 0}};

  triangle.vertices[1] = {{-1, 0, 1}, {0, 1, 0}, {1, 1, 1, 1}, {0, 0}};

  triangle.vertices[2] = {{0, 0, -41}, {0, 1, 0}, {1, 1, 1, 1}, {0, 0}};

  collided = Physics::checkCollisionRayTriangle(ray, triangle, &out);

  if (collided) {
    printf("Position: x:%f y:%f z:%f\nNormal: x:%f y:%f z:%f\ntConstant: %f\n",
           out.hitPosition.x, out.hitPosition.y, out.hitPosition.z,
           out.hitNormal.x, out.hitNormal.y, out.hitNormal.z, out.tConstant);
  }

  return 0;
}
