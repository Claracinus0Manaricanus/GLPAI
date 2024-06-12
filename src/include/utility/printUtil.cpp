#include "printUtil.hpp"
#include <cstdio>

// vectors
void print(Vec2& a) { printf("X: %f, Y: %f", a.x, a.y); }
void println(Vec2& a) {
  print(a);
  printf("\n");
}

void print(Vec3& a) { printf("X: %f, Y: %f, Z: %f", a.x, a.y, a.z); }
void println(Vec3& a) {
  print(a);
  printf("\n");
}

void print(Vec4& a) {
  printf("X: %f, Y: %f, Z: %f, W: %f", a.x, a.y, a.z, a.w);
}
void println(Vec4& a) {
  print(a);
  printf("\n");
}

// matrices
void print(Mat4& a) {
  for (int i = 0; i < 4; i++) {
    printf("row %d: ", i);
    println(a.row[i]);
  }
}

// physics
void print(Ray& a) {
  printf("Start: ");
  println(a.start);
  printf("Direction: ");
  println(a.direction);
}

void print(RayHit& a) {
  printf("hitPosition: ");
  println(a.hitPosition);
  printf("hitNormal: ");
  println(a.hitNormal);
  printf("tConstant: %f\n", a.tConstant);
}

// classes
void print(Transform& tr) {
  printf("Position: ");
  println(tr.getPosition());
  printf("Rotation: ");
  println(tr.getRotation());
  printf("Scale: ");
  println(tr.getScale());
}

// primitives
void print(Vertex& a) {
  printf("Position: X: %f, Y: %f, Z: %f\n"
         "Normal:   X: %f, Y: %f, Z: %f\n"
         "Color:    R: %f, G: %f, B: %f, A: %f\n"
         "UV:       U: %f, V: %f\n",
         a.position.x, a.position.y, a.position.z, a.normal.x, a.normal.y,
         a.normal.z, a.color.x, a.color.y, a.color.z, a.color.w, a.uv.x,
         a.uv.y);
}

void print(Triangle& a) {
  printf("V0:\n");
  print(a.vertices[0]);

  printf("V1:\n");
  print(a.vertices[1]);

  printf("V2:\n");
  print(a.vertices[2]);
}
