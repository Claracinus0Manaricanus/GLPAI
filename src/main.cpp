#include "include/physics/collisions.hpp"
#include "include/rendering/opengl/cm_opengl.hpp"
#include "include/rendering/window/window.hpp"
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
  Ray ray = {{0, 0, 0}, {0, 0, -1}};

  // class tests
  Mesh newMesh;
  Triangle triangle;
  triangle.vertices[0] = {{-0.5f, -0.5f, 0}, {0, 0, 1}, {1, 0, 0, 1}, {0, 0}};

  triangle.vertices[1] = {{0.5f, -0.5f, 0}, {0, 0, 1}, {1, 0, 0, 1}, {0, 0}};

  triangle.vertices[2] = {{0, 0.5f, 0}, {0, 0, 1}, {1, 0, 0, 1}, {0, 0}};
  newMesh.addTriangle(triangle);

  triangle.vertices[0] = {
      {-20.0f, -0.5f, 20.0f}, {0, 1, 0}, {1, 1, 1, 1}, {0, 0}};

  triangle.vertices[1] = {
      {20.0f, -0.5f, 20.0f}, {0, 1, 0}, {1, 1, 1, 1}, {0, 0}};

  triangle.vertices[2] = {
      {0.0f, -0.5f, -20.0f}, {0, 1, 0}, {1, 1, 1, 1}, {0, 0}};
  newMesh.addTriangle(triangle);

  triangle.vertices[0] = {{-0.5f, -0.5f, -1}, {0, 0, 1}, {0, 1, 0, 1}, {0, 0}};

  triangle.vertices[1] = {{0.5f, -0.5f, -1}, {0, 0, 1}, {0, 1, 0, 1}, {0, 0}};

  triangle.vertices[2] = {{0, 0.5f, -1}, {0, 0, 1}, {0, 1, 0, 1}, {0, 0}};
  newMesh.addTriangle(triangle);

  // Window
  Window test(800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  if (!test.isFine()) {
    printf("window has a problem!\n%s\n", test.getError());
    return 1;
  }

  test.setClearColor(0, 0, 0, 1);
  test.setSwapInterval(1);
  const uint8_t* keyStates = test.getKeyboardState();

  // OpenGL
  OGL_Program prg({"src/include/rendering/opengl/shaders/basic/vert.sha",
                   "src/include/rendering/opengl/shaders/basic/frag.sha",
                   NULL});
  if (prg.getError() != NULL)
    printf("%s\n", prg.getError());

  // camera
  Camera cam;

  OGL_RendererData renData = {&prg, &cam};
  OGL_Renderer newRen(renData);

  OGL_Renderable testRen = newRen.genRenderable(newMesh);
  newRen.setProgram(&prg);

  // vars
  Vec2 mousePos = {0, 0};
  uint32_t lastMiliSec = test.getTicks();
  float deltaTime = 0;
  int collided = 0;

  // main loop
  while (!test.shouldClose()) {
    // update resolution
    test.updateViewport();

    // get deltaTime
    deltaTime = (test.getTicks() - lastMiliSec) / 1000.0f;
    lastMiliSec = test.getTicks();

    // mouse test
    mousePos = test.getCursorPosNormalized();
    Vec3 movement = {
        ((float)keyStates[SDL_SCANCODE_D] - (float)keyStates[SDL_SCANCODE_A]) *
            deltaTime,
        ((float)keyStates[SDL_SCANCODE_E] - (float)keyStates[SDL_SCANCODE_Q]) *
            deltaTime,
        ((float)keyStates[SDL_SCANCODE_W] - (float)keyStates[SDL_SCANCODE_S]) *
            deltaTime};
    cam.localMove(movement);

    cam.rotate({-((float)keyStates[SDL_SCANCODE_UP] -
                  (float)keyStates[SDL_SCANCODE_DOWN]) *
                    deltaTime,
                -((float)keyStates[SDL_SCANCODE_LEFT] -
                  (float)keyStates[SDL_SCANCODE_RIGHT]) *
                    deltaTime,
                0});

    if (keyStates[SDL_SCANCODE_R])
      cam.setRotation({0, 0, 0});

    // ray
    ray.start = cam.getPosition();
    ray.direction = cam.getForward();

    // collision testing
    collided = Physics::checkCollisionRayMesh(ray, newMesh, &out);
    if (collided)
      print(out);

    println(cam.getPosition());
    println(cam.getRotation());
    cam.setAspectRatio(test.getAspectRatio());

    // testing
    test.checkEvents();
    test.clearScreen();
    newRen.render(testRen);
    test.updateScreen();
  }

  return 0;
}
