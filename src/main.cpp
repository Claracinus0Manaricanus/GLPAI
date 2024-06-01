#include "include/cm_math/operations.hpp"
#include "include/physics/collisions.hpp"
#include "include/types/physics.hpp"
#include "include/utility/printUtil.hpp"
#include <cstdio>

int main(int argc, char **arg) {
  printf("starting\n");

  RayHit out = {{0, 0, 0}, {0, 0, 0}, 0};
  Ray ray = {{0, 10, 0}, {0, -1, 0}};

  Triangle triangle;
  triangle.vertices[0] = {{0, 0, 1}, {0, 1, 0}, {1, 1, 1, 1}, {0, 0}};

  triangle.vertices[1] = {{-1, 0, -1}, {0, 1, 0}, {1, 1, 1, 1}, {0, 0}};

  triangle.vertices[2] = {{1, 0, -1}, {0, 1, 0}, {1, 1, 1, 1}, {0, 0}};

  printf("\n");
  printf("Triangle:\n");
  print(triangle);
  printf("\n");

  printf("\nRay:\n");
  print(ray);
  printf("\n");

  int collided = Physics::checkCollisionRayTriangle(ray, triangle, &out);

  if (collided) {
    printf("HIT!\n");
    print(out);
  }

  return 0;
}
