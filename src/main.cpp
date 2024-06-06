#include "include/physics/collisions.hpp"
#include "include/rendering/opengl/cm_opengl.hpp"
#include "include/rendering/window/window.hpp"
#include "include/types/classes/mesh.hpp"
#include "include/types/physics.hpp"
#include "include/utility/printUtil.hpp"
#include <cstdio>

void keyCallback(uint32_t type, SDL_Keysym key) {
  if (type == SDL_KEYDOWN) {
    printf("%s\n", SDL_GetKeyName(key.sym));
  }
}

int main(int argc, char** arg) {
  RayHit out = {{0, 0, 0}, {0, 0, 0}, 0};
  Ray ray = {{0, 0, 10}, {0, 0, -1}};

  Triangle triangle;
  triangle.vertices[0] = {{-0.5f, -0.5f, 0}, {0, 0, 1}, {0, 0, 1, 1}, {0, 0}};

  triangle.vertices[1] = {{0.5f, -0.5f, 0}, {0, 0, 1}, {0, 1, 0, 1}, {0, 0}};

  triangle.vertices[2] = {{0, 0.5f, 0}, {0, 0, 1}, {1, 0, 0, 1}, {0, 0}};

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

  // class tests
  std::vector<Vertex> verVec = {triangle.vertices[0], triangle.vertices[1],
                                triangle.vertices[2]};

  Mesh newMesh;
  newMesh.addTriangle(triangle);

  verVec = newMesh.getAllVertices();

  std::vector<uint32_t> ind = newMesh.getIndexBuffer();

  printf("\nprinting newMesh\n");
  for (Vertex a : verVec) {
    printf("\n");
    print(a);
    printf("\n");
  }

  Window test(800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  if (!test.isFine()) {
    printf("window has a problem!\n%s\n", test.getError());
    return 1;
  }

  test.setClearColor(0, 0, 0, 1);

  OGL_Renderer newRen;

  OGL_Renderable testRen = newRen.genRenderable(newMesh);

  OGL_Program prg({"src/include/rendering/opengl/shaders/basic/vert.sha", "src/include/rendering/opengl/shaders/basic/frag.sha", NULL});
  if(prg.getError() != NULL)
    printf("%s\n", prg.getError());

  while (!test.shouldClose()) {
    test.checkEvents(keyCallback);
    test.clearScreen();
    newRen.renderOGL_Renderable(prg, testRen);
    test.updateScreen();
  }

  return 0;
}
