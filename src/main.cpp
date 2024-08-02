#include "include/cm_math/operations.hpp"
#include "include/physics/collisions.hpp"
#include "include/rendering/opengl/cm_opengl.hpp"
#include "include/rendering/window/window.hpp"
#include "include/types/classes/gameObject.hpp"
#include "include/types/classes/scene.hpp"
#include "include/types/physics.hpp"
#include "include/utility/printUtil.hpp"
#include <cstdio>

int main(int argc, char** arg) {
  // imports
  Scene mainScene;

  GameObjectData gData = {{{0, 1, 0}, {0, 0, 0}, {2, 2, 2}}, 0, 0};
  mainScene.addGameObject(gData);

  gData.transformD = {{0, 1, 0}, {0, 0, 0}, {10, 10, 10}};
  gData.meshID = 1;
  gData.materialID = 1;
  mainScene.addGameObject(gData);

  PointLightData lData = {
      {-1.0f, 2.0f, -1.0f},
      {1, 1, 1},
      10,
  };
  mainScene.addPointLight(lData);
  lData.position = (Vec3){1.0f, 2.0f, 1.0f};
  mainScene.addPointLight(lData);

  // Window
  Window mainWin(800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  if (!mainWin.isFine()) {
    printf("window has a problem!\n%s\n", mainWin.getError());
    return 1;
  }

  mainWin.setClearColor(0, 0, 0, 1);
  mainWin.setSwapInterval(1);
  const uint8_t* keyStates = mainWin.getKeyboardState();
  mainWin.setRelativeMouseMode(SDL_TRUE);
  mainWin.setMousePos(0, 0);

  // OpenGL
  OGL_Program prg_basic({"src/include/rendering/opengl/shaders/basic/vert.sha",
                         "src/include/rendering/opengl/shaders/basic/frag.sha",
                         NULL});
  if (prg_basic.getError() != NULL)
    printf("%s\n", prg_basic.getError());

  OGL_Program prg_texture(
      {"src/include/rendering/opengl/shaders/texture/vert.sha",
       "src/include/rendering/opengl/shaders/texture/frag.sha", NULL});
  if (prg_texture.getError() != NULL)
    printf("%s\n", prg_texture.getError());

  // camera
  Camera cam;
  cam.setPosition({0, 10, 0});

  // OGL_Renderer
  OGL_Renderer newRen;

  // generating ground
  Mesh ground;
  Vertex tempToInsert;
  int ground_size = 10;
  for (int i = 0; i < ground_size; i++) {
    for (int k = 0; k < ground_size; k++) {
      tempToInsert.position = {(float)i - ((float)ground_size / 2), 0,
                               (float)k - ((float)ground_size / 2)};
      tempToInsert.normal = {0.0f, 1.0f, 0.0f};
      tempToInsert.uv = {(float)i / ground_size, (float)k / ground_size};
      ground.addVertex(tempToInsert);
    }
  }

  for (int i = 0; i < ground_size - 1; i++) {
    for (int k = 0; k < ground_size - 1; k++) {
      ground.addFace(i * ground_size + k, i * ground_size + k + 1,
                     (i + 1) * ground_size + k);
      ground.addFace((i + 1) * ground_size + k, i * ground_size + k + 1,
                     (i + 1) * ground_size + k + 1);
    }
  }

  newRen.register_mesh(ground);

  std::vector<Mesh> meshes = Scene::import("assets/models/dragon_vrip.ply");
  newRen.register_mesh(meshes[0]);

  MaterialData mData;
  mData.color = {1, 1, 1, 1};
  Material tmpMat(mData);
  if (tmpMat.loadTexture("assets/images/arkKnightsChars.jpg") != 0) {
    printf("no tex!\n");
    return -1;
  }
  newRen.register_material(tmpMat);

  mData.color.y = 0;
  mData.color.z = 0;
  Material tmpMat2(mData);
  newRen.register_material(tmpMat2);

  // vars
  Vec2 mousePos = {0, 0};
  uint32_t lastMiliSec = mainWin.getTicks();
  float deltaTime = 0;
  int collided = 0;
  char captureMouse = 1, escA = 1;

  float G = -9.8f;
  float upVel = 0;
  float camHeight = 1.7f;

  RayHit out = {{0, 0, 0}, {0, 0, 0}, 0};
  Ray ray = {{0, 0, 0}, {0, 0, -1}};

  // main loop
  while (!mainWin.shouldClose()) {
    // testing area //
    // testing area //

    // update resolution
    mainWin.updateViewport();

    // get deltaTime
    deltaTime = (mainWin.getTicks() - lastMiliSec) / 1000.0f;
    lastMiliSec = mainWin.getTicks();

    // mouse test
    if (keyStates[SDL_SCANCODE_ESCAPE] && captureMouse && escA) {
      captureMouse = 0;
      mainWin.setRelativeMouseMode(SDL_FALSE);
      escA = 0;
    } else if (keyStates[SDL_SCANCODE_ESCAPE] && escA) {
      captureMouse = 1;
      mainWin.setRelativeMouseMode(SDL_TRUE);
      mainWin.setMousePos(0, 0);
      escA = 0;
    }

    if (!keyStates[SDL_SCANCODE_ESCAPE])
      escA = 1;

    if (captureMouse) {
      mousePos = mainWin.getCursorPosNormalized();
      mainWin.setMousePos(0, 0);
    } else {
      mousePos = {0, 0};
    }

    abs(mousePos.x) > 0.004f ? mousePos.x = mousePos.x : mousePos.x = 0;
    abs(mousePos.y) > 0.004f ? mousePos.y = mousePos.y : mousePos.y = 0;
    cam.rotate(
        {mousePos.y, -mousePos.x,
         (keyStates[SDL_SCANCODE_Q] - keyStates[SDL_SCANCODE_E]) * deltaTime});

    // movement
    Vec3 movement = {
        ((float)keyStates[SDL_SCANCODE_D] - (float)keyStates[SDL_SCANCODE_A]),
        0,
        ((float)keyStates[SDL_SCANCODE_W] - (float)keyStates[SDL_SCANCODE_S])};

    Vec3 dir =
        Vector::Normalize((Vec3){cam.getRight().x, 0, cam.getRight().z}) *
            movement.x +
        Vector::Normalize((Vec3){cam.getForward().x, 0, cam.getForward().z}) *
            movement.z;

    cam.move(Vector::Normalize(dir) * deltaTime);

    // camera reset
    if (keyStates[SDL_SCANCODE_R] || cam.getPosition().y < -100.0f) {
      cam.setRotation({0, 0, 0});
      cam.setPosition({0, camHeight, 0});
      upVel = 0;
    }

    // ray
    ray.start = cam.getPosition();
    ray.direction = {0, -1, 0};

    // collision test
    collided = Physics::checkCollisionRayMesh(ray, ground,
                                              mainScene.getGameObject(0), &out);

    if (collided) {
      print(out);
      println(cam.getPosition());
    }

    if (keyStates[SDL_SCANCODE_LCTRL])
      camHeight = 0.85f;
    else
      camHeight = 1.7f;

    upVel += G * deltaTime;
    if (collided && out.tConstant < camHeight &&
        keyStates[SDL_SCANCODE_SPACE]) {
      upVel = 4.5f;
    } else if (collided && out.tConstant < camHeight) {
      upVel = 0;
      cam.setPosition({cam.getPosition().x, out.hitPosition.y + camHeight,
                       cam.getPosition().z});
    }

    cam.move({0, upVel * deltaTime, 0});

    // event polling
    mainWin.checkEvents();

    // rendering
    mainWin.clearScreen();
    cam.setAspectRatio(mainWin.getAspectRatio());
    newRen.render(mainScene, prg_texture, prg_basic, cam);

    // swap buffers
    mainWin.updateScreen();
  }

  return 0;
}
