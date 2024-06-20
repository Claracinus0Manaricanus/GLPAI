#include "include/cm_math/operations.hpp"
#include "include/physics/collisions.hpp"
#include "include/rendering/opengl/cm_opengl.hpp"
#include "include/rendering/window/window.hpp"
#include "include/types/classes/gameObject.hpp"
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

  // GameObject
  GameObjectData gobjData;
  gobjData.materialD.color = {1, 0, 0, 1};
  GameObject newGOBJ(gobjData);
  Triangle triangle;
  triangle.vertices[0] = {{-0.5f, -0.5f, 0}, {0, 0, 1}, {0, 0}};

  triangle.vertices[1] = {{0.5f, -0.5f, 0}, {0, 0, 1}, {0, 0}};

  triangle.vertices[2] = {{0, 0.5f, 0}, {0, 0, 1}, {0, 0}};
  newGOBJ.addTriangle(triangle);

  triangle.vertices[0] = {{-20.0f, -0.5f, 20.0f}, {0, 1, 0}, {0, 0}};

  triangle.vertices[1] = {{20.0f, -0.5f, 20.0f}, {0, 1, 0}, {0, 0}};

  triangle.vertices[2] = {{0.0f, -0.5f, -20.0f}, {0, 1, 0}, {0, 0}};
  newGOBJ.addTriangle(triangle);

  triangle.vertices[0] = {{-0.5f, -0.5f, -1}, {0, 0, 1}, {0, 0}};

  triangle.vertices[1] = {{0.5f, -0.5f, -1}, {0, 0, 1}, {0, 0}};

  triangle.vertices[2] = {{0, 0.5f, -1}, {0, 0, 1}, {0, 0}};
  newGOBJ.addTriangle(triangle);

  // Window
  Window test(800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  if (!test.isFine()) {
    printf("window has a problem!\n%s\n", test.getError());
    return 1;
  }

  test.setClearColor(0, 0, 0, 1);
  test.setSwapInterval(1);
  const uint8_t* keyStates = test.getKeyboardState();
  test.setRelativeMouseMode(SDL_TRUE);
  test.setMousePos(0, 0);

  // OpenGL
  OGL_Program prg({"src/include/rendering/opengl/shaders/basic/vert.sha",
                   "src/include/rendering/opengl/shaders/basic/frag.sha",
                   NULL});
  if (prg.getError() != NULL)
    printf("%s\n", prg.getError());

  // camera
  Camera cam;

  // OGL_Renderer
  OGL_RendererData renData = {&prg, &cam};
  OGL_Renderer newRen(renData);

  OGL_Renderable testRen = newRen.genRenderable(newGOBJ, &newGOBJ);
  newRen.setProgram(&prg);

  // vars
  Vec2 mousePos = {0, 0};
  uint32_t lastMiliSec = test.getTicks();
  float deltaTime = 0;
  int collided = 0;
  char captureMouse = 1, escA = 1;

  float G = -9.8f;
  float upVel = 0;

  // main loop
  while (!test.shouldClose()) {
    // update resolution
    test.updateViewport();

    // get deltaTime
    deltaTime = (test.getTicks() - lastMiliSec) / 1000.0f;
    lastMiliSec = test.getTicks();

    // mouse test
    if (keyStates[SDL_SCANCODE_ESCAPE] && captureMouse && escA) {
      captureMouse = 0;
      test.setRelativeMouseMode(SDL_FALSE);
      escA = 0;
    } else if (keyStates[SDL_SCANCODE_ESCAPE] && escA) {
      captureMouse = 1;
      test.setRelativeMouseMode(SDL_TRUE);
      test.setMousePos(0, 0);
      escA = 0;
    }

    if (!keyStates[SDL_SCANCODE_ESCAPE])
      escA = 1;

    if (captureMouse) {
      mousePos = test.getCursorPosNormalized();
      test.setMousePos(0, 0);
    } else {
      mousePos = {0, 0};
    }

    abs(mousePos.x) > 0.004f ? mousePos.x = mousePos.x : mousePos.x = 0;
    abs(mousePos.y) > 0.004f ? mousePos.y = mousePos.y : mousePos.y = 0;
    cam.rotate({-mousePos.y, mousePos.x, 0});

    // movement
    Vec3 movement = {
        ((float)keyStates[SDL_SCANCODE_D] - (float)keyStates[SDL_SCANCODE_A]),
        0,
        ((float)keyStates[SDL_SCANCODE_W] - (float)keyStates[SDL_SCANCODE_S])};

    Vec3 dir = Vector::Normalize({cam.getRight().x, 0, cam.getRight().z}) *
                   movement.x +
               Vector::Normalize({cam.getForward().x, 0, cam.getForward().z}) *
                   movement.z;

    cam.move(Vector::Normalize(dir) * deltaTime);

    // camera reset
    if (keyStates[SDL_SCANCODE_R]) {
      cam.setRotation({0, 0, 0});
      cam.setPosition({0, 0, 0});
    }

    // ray
    ray.start = cam.getPosition();
    ray.direction = {0, -1, 0};

    // collision testing
    collided = Physics::checkCollisionRayGameObject(ray, newGOBJ, &out);
    if (collided)
      print(out);

    upVel += G * deltaTime;
    if (collided && out.tConstant < 1.7f && keyStates[SDL_SCANCODE_SPACE]) {
      upVel = 4.5f;
    } else if (collided && out.tConstant < 1.7f) {
      upVel = 0;
      cam.setPosition(
          {cam.getPosition().x, out.hitPosition.y + 1.7f, cam.getPosition().z});
    }

    cam.move({0, upVel * deltaTime, 0});

    // print debug
    println(cam.getPosition());
    println(cam.getRotation());

    // event polling
    test.checkEvents();

    // rendering
    test.clearScreen();
    cam.setAspectRatio(test.getAspectRatio());
    newRen.render(testRen);

    // swap buffers
    test.updateScreen();
  }

  return 0;
}
