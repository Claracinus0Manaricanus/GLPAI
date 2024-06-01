#include "printUtil.hpp"
#include <cstdio>

// vectors
void print(Vec2 &a) { printf("X: %f, Y: %f", a.x, a.y); }
void println(Vec2 &a) {
  print(a);
  printf("\n");
}

void print(Vec3 &a) { printf("X: %f, Y: %f, Z: %f", a.x, a.y, a.z); }
void println(Vec3 &a) {
  print(a);
  printf("\n");
}

void print(Vec4 &a) {
  printf("X: %f, Y: %f, Z: %f, W: %f", a.x, a.y, a.z, a.w);
}
void println(Vec4 &a) {
  print(a);
  printf("\n");
}
