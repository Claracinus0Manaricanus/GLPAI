#include <cm_math/operations.hpp>
#include <cstdio>
#include <math.h>
#include <physics/collisions.hpp>
#include <rendering/opengl/cm_opengl.hpp>
#include <rendering/opengl/include/OGL_Program.hpp>
#include <rendering/window/window.hpp>
#include <types/classes/gameObject.hpp>
#include <types/classes/scene.hpp>
#include <types/physics.hpp>
#include <utility/printUtil.hpp>

int main(int argc, char** arg) {
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
  OGL_Program prg_basic({"src/include/rendering/opengl/shaders/basic/vert.glsl",
                         "src/include/rendering/opengl/shaders/basic/frag.glsl",
                         NULL});
  if (prg_basic.getError() != NULL)
    printf("%s\n", prg_basic.getError());

  OGL_Program prg_texture(
      {"src/include/rendering/opengl/shaders/texture/vert.glsl",
       "src/include/rendering/opengl/shaders/texture/frag.glsl", NULL});
  if (prg_texture.getError() != NULL)
    printf("%s\n", prg_texture.getError());

  OGL_Program prg_skybox(
      {"src/include/rendering/opengl/shaders/skybox/vert.glsl",
       "src/include/rendering/opengl/shaders/skybox/frag.glsl", NULL});
  if (prg_skybox.getError() != NULL)
    printf("%s\n", prg_skybox.getError());

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

  const int mesh_ground = newRen.register_mesh(ground);

  std::vector<Mesh> meshes = Scene::import("assets/models/dragon_vrip.ply");
  const int mesh_dragon = newRen.register_meshes(meshes);

  MaterialData mData;
  mData.color = {1, 1, 1, 1};
  mData.metallic = 0.95f;
  Material tmpMat(mData);
  if (tmpMat.loadTexture("assets/images/arkKnightsChars.jpg") != 0) {
    printf("no tex!\n");
    return -1;
  }
  const int material_arkTex = newRen.register_material(tmpMat);

  mData.color = {1, 0, 0, 1};
  mData.metallic = 1.0f;
  Material tmpMat2(mData);
  const int material_redMetallic = newRen.register_material(tmpMat2);

  tmpMat.loadTexture("assets/images/equ.jpg");
  const int material_equ = newRen.register_material(tmpMat);

  // Scene
  Scene mainScene;

  GameObjectData gData = {
      {{0, 0, 0}, {0, 0, 0}, {2, 2, 2}}, mesh_ground, material_arkTex};
  mainScene.addGameObject(gData);

  gData.transformD = {{0, 0, 0}, {0, 0, 0}, {10, 10, 10}};
  gData.meshID = mesh_dragon;
  gData.materialID = material_redMetallic;
  mainScene.addGameObject(gData);

  PointLightData lData = {
      {-1.0f, 2.0f, -1.0f},
      {1, 1, 1},
      1,
  };
  mainScene.addPointLight(lData);
  lData.position = (Vec3){1.0f, 2.0f, 1.0f};
  mainScene.addPointLight(lData);

  // box test
  Box box1 = {{0, 0, 0}, {1, 1, 1}};

  Mesh justBox(box1);

  justBox.calculateNormals();

  const int mesh_box = newRen.register_mesh(justBox);

  gData.meshID = mesh_box;
  gData.materialID = material_arkTex;
  gData.transformD = {{5, 1, 0}, {0, 0, 0}, {1, 1, 1}};

  mainScene.addGameObject(gData);

  // sphere test
  Sphere sph1 = {{0, 0, 0}, 1};
  Mesh sphM(sph1, 100, 1);

  const int mesh_sphere = newRen.register_mesh(sphM);

  gData.transformD.position = {-5, 1, 0};
  gData.meshID = mesh_sphere;
  gData.materialID = material_equ;
  mainScene.addGameObject(gData);

  // skybox
  const char* skyFiles[6] = {
      "assets/skybox/starryCSky/px.png", "assets/skybox/starryCSky/nx.png",
      "assets/skybox/starryCSky/py.png", "assets/skybox/starryCSky/ny.png",
      "assets/skybox/starryCSky/pz.png", "assets/skybox/starryCSky/nz.png"};
  Skybox sky0(skyFiles);
  newRen.register_skybox(sky0);

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
    sph1.position = mainScene.getGameObject(3).getPosition();
    ray.direction = cam.getForward();
    collided = Physics::checkCollisionRaySphere(ray, sph1, &out);
    /*if (collided) {
      printf("collided with sphere!\nposition: ");
      println(out.hitPosition);
      printf("normal: ");
      println(out.hitNormal);
    }*/
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

    /*if (collided) {
      print(out);
      println(cam.getPosition());
      }*/

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
    mainScene.getPointLight(0).setPosition(cam.getPosition());

    // event polling
    mainWin.checkEvents();

    // rendering
    mainWin.clearScreen();
    cam.setAspectRatio(mainWin.getAspectRatio());
    newRen.render(mainScene, prg_texture, prg_basic, cam);
    newRen.render_skybox(0, prg_skybox, cam);

    // swap buffers
    mainWin.updateScreen();
  }

  return 0;
}
