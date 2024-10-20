#include <GLFW/glfw3.h>
#include <cm_math/operations.hpp>
#include <cmath>
#include <cstdio>
#include <physics/collisions.hpp>
#include <platform/cm_time.hpp>
#include <rendering/opengl/cm_opengl.hpp>
#include <rendering/opengl/include/OGL_Program.hpp>
#include <rendering/window/glfw/window.hpp>
#include <rendering/window/sdl/window.hpp>
#include <types/classes/cubemap.hpp>
#include <types/classes/gameObject.hpp>
#include <types/classes/scene.hpp>
#include <types/data_structures/linked_list.hpp>
#include <types/data_structures/tree.hpp>
#include <types/physics.hpp>
#include <utility/printUtil.hpp>

int main(int argc, char** arg) {
  // Window
  // cm_sdl_Window mainWin(800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
  cm_glfw_Window mainWin(800, 600);

  if (!mainWin.isFine()) {
    printf("window has a problem!\n%s\n", mainWin.getError());
    return 1;
  }

  mainWin.setClearColor(0, 0, 0, 1);
  mainWin.setSwapInterval(1);
  mainWin.setRelativeMouseMode(SDL_TRUE);
  mainWin.setMousePos(0, 0);

  // camera
  int current_cam = 0;
  Camera cams[2];
  cams[0].setPosition({0, 0.2f, 0});
  cams[1].setPosition({10, 10, 10});
  cams[1].setRotation({-PI / 4, PI / 4, 0});

  // OGL_Renderer
  OGL_Renderer newRen;
  const int resolution_width = 1920;
  const int resolution_height = 1080;
  newRen.create_framebuffer(resolution_width, resolution_height);

  // programs
  const int prg_skybox = newRen.register_program(
      {"src/include/rendering/opengl/shaders/skybox/vert.glsl",
       "src/include/rendering/opengl/shaders/skybox/frag.glsl", NULL});
  if (newRen.getProgramError(prg_skybox) != NULL)
    printf("%s\n", newRen.getProgramError(prg_skybox));

  const int prg_basic = newRen.register_program(
      {"src/include/rendering/opengl/shaders/basic/vert.glsl",
       "src/include/rendering/opengl/shaders/basic/frag.glsl", NULL});
  if (newRen.getProgramError(prg_basic) != NULL)
    printf("%s\n", newRen.getProgramError(prg_basic));

  const int prg_texture = newRen.register_program(
      {"src/include/rendering/opengl/shaders/texture/vert.glsl",
       "src/include/rendering/opengl/shaders/texture/frag.glsl", NULL});
  if (newRen.getProgramError(prg_texture) != NULL)
    printf("%s\n", newRen.getProgramError(prg_texture));

  const int prg_white = newRen.register_program(
      {"shaders/white/vert.glsl", "shaders/white/frag.glsl", NULL});
  if (newRen.getProgramError(prg_white) != NULL)
    printf("%s\n", newRen.getProgramError(prg_white));

  // importing meshes
  std::vector<Mesh> meshes = Scene::import("assets/models/train/seat.obj");
  const int mesh_seat = newRen.register_meshes(meshes);

  meshes = Scene::import("assets/models/train/train_head.obj");
  const int mesh_train_head = newRen.register_meshes(meshes);

  meshes = Scene::import("assets/models/train/train_compartment.obj");
  const int mesh_train_compartment = newRen.register_meshes(meshes);

  Sphere sph1 = {{0, 0, 0}, 1};
  Mesh sphM(sph1, 100);

  const int mesh_sphere = newRen.register_mesh(sphM);

  // materials
  MaterialData mData;
  mData.color = {1, 1, 1, 1};
  mData.metallic = 0.95f;
  mData.prg_ID = prg_texture;
  Material tmpMat(mData);
  if (tmpMat.loadTexture("assets/images/arkKnightsChars.jpg") != 0) {
    printf("no tex!\n");
    return -1;
  }
  const int material_arkTex = newRen.register_material(tmpMat);

  tmpMat.loadTexture("assets/images/train/seat_color.png");
  const int material_seat = newRen.register_material(tmpMat);

  tmpMat.setColor({1, 1, 1, 1});
  tmpMat.setMetalness(1.0f);
  tmpMat.setPrgID(prg_white);
  tmpMat.resetTexture();
  const int material_white = newRen.register_material(tmpMat);

  tmpMat.setColor({0.6f, 0.6f, 0.6f, 1.0f});
  tmpMat.setPrgID(prg_basic);
  tmpMat.setMetalness(0);
  const int material_gray = newRen.register_material(tmpMat);

  // Scene
  Scene mainScene;

  // game objects
  GameObjectData gData;

  gData.transformD.position = {0, 0, 0};
  gData.transformD.scale = {1, 1, 1};
  gData.meshID = -1;
  gData.materialID = -1;

  auto parentNode = mainScene.addGameObject(nullptr, gData);

  gData.transformD.position = {0, 0, -12};
  gData.transformD.scale = {1, 1, 1};
  gData.meshID = mesh_train_head;
  gData.materialID = material_gray;

  mainScene.addGameObject(nullptr, gData);

  gData.transformD.position = {0, 0, 0};
  gData.transformD.scale = {1, 1, 1};
  gData.meshID = mesh_seat;
  gData.materialID = material_seat;

  for (int i = -3; i < 3; i++) {
    for (int k = -1; k < 2; k++) {
      if (k == 0)
        continue;
      gData.transformD.rotation = {0, k * PI / 2, 0};
      for (int j = 0; j < 4; j++) {
        gData.transformD.position = {(float)k, 0, i * 3.0f + 0.75f + j * 0.5f};
        mainScene.addGameObject(parentNode, gData, {nullptr, 2});
      }
    }
  }

  gData.transformD.position = {0, 0, 0};
  gData.transformD.rotation = {0, 0, 0};
  gData.meshID = mesh_train_compartment;
  gData.materialID = material_gray;
  for (int i = -3; i < 3; i++) {
    gData.transformD.position = {0, 0, i * 3.0f};
    mainScene.addGameObject(parentNode, gData, {"compartment", 1});
  }

  gData.transformD.scale = {0.1f, 0.1f, 0.1f};
  gData.meshID = mesh_sphere;
  gData.materialID = material_white;
  for (int i = -3; i < 3; i++) {
    gData.transformD.position = {0, 2.5f, 3 * (float)i - 0.75f};
    mainScene.addGameObject(nullptr, gData);
  }

  // lights
  PointLightData lData = {{0, 0, 0}, {1, 1, 1}, 1};

  for (int i = -3; i < 3; i++) {
    lData.position = {0, 2.4f, 3 * (float)i - 0.75f};
    mainScene.addPointLight(lData);
  }

  // skybox
  const char* skyFiles[6] = {
      "assets/skybox/stars.png", "assets/skybox/stars.png",
      "assets/skybox/stars.png", "assets/skybox/stars.png",
      "assets/skybox/stars.png", "assets/skybox/stars.png"};

  Cubemap sky0(skyFiles);
  newRen.register_cubemap(sky0);

  // vars
  Vec2 mousePos = {0, 0};
  cm_Time timer;
  float deltaTime = 0;
  int collided = 0;
  char captureMouse = 1, escA = 1;

  float G = -9.8f;
  float upVel = 0;
  float camHeight = 1.7f;
  float cam_z_rot = 0;
  float camSpeed = 1.0f;

  RayHit out = {{0, 0, 0}, {0, 0, 0}, 0};
  Ray ray = {{0, 0, 0}, {0, 0, -1}};
  cm_LinkedList<TreeNode<GameObject_Storage>>* compartments =
      mainScene.getGameObjects(1);

  SphereHit sph_out;
  Sphere player_sphere;

  timer.getDeltaTime();
  Surface groundPlane = {{0, 1, 0}, 0};

  // main loop
  while (!mainWin.shouldClose()) {
    // get deltaTime
    deltaTime = timer.getDeltaTime();

    // mouse test
    if (mainWin.getKey(GLFW_KEY_ESCAPE) && captureMouse && escA) {
      captureMouse = 0;
      current_cam = 1;
      mainWin.setRelativeMouseMode(SDL_FALSE);
      escA = 0;
    } else if (mainWin.getKey(GLFW_KEY_ESCAPE) && escA) {
      captureMouse = 1;
      current_cam = 0;
      mainWin.setRelativeMouseMode(SDL_TRUE);
      mainWin.setMousePos(0, 0);
      escA = 0;
    }

    if (!mainWin.getKey(GLFW_KEY_ESCAPE))
      escA = 1;

    if (captureMouse) {
      mousePos = mainWin.getCursorPosNormalized();
      mainWin.setMousePos(0, 0);
    } else {
      mousePos = {0, 0};
    }

    abs(mousePos.x) > 0.004f ? mousePos.x = mousePos.x : mousePos.x = 0;
    abs(mousePos.y) > 0.004f ? mousePos.y = mousePos.y : mousePos.y = 0;
    cams[0].rotate({mousePos.y, -mousePos.x, 0});

    // movement
    Vec3 movement = {
        ((float)mainWin.getKey(GLFW_KEY_D) - (float)mainWin.getKey(GLFW_KEY_A)),
        0,
        ((float)mainWin.getKey(GLFW_KEY_W) -
         (float)mainWin.getKey(GLFW_KEY_S))};

    if (mainWin.getKey(GLFW_KEY_LEFT_SHIFT)) {
      if (camSpeed < 4.0f) {
        camSpeed += camSpeed * 4.0f * deltaTime;
      } else {
        camSpeed = 4.0f;
      }
    } else {
      if (camSpeed > 1.0f) {
        camSpeed -= camSpeed * 4.0f * deltaTime;
      } else {
        camSpeed = 1.0f;
      }
    }

    Vec3 dir = Vector::Normalize(
                   (Vec3){cams[0].getRight().x, 0, cams[0].getRight().z}) *
                   movement.x +
               Vector::Normalize(
                   (Vec3){cams[0].getForward().x, 0, cams[0].getForward().z}) *
                   movement.z;

    if (abs(cam_z_rot) < camSpeed * PI / 80.0f) {
      cam_z_rot += deltaTime * -movement.x * PI / 5.0f;
    } else if (movement.x != 0) {
      cam_z_rot = -movement.x * camSpeed * PI / 80.0f;
    }

    if (movement.x == 0) {
      cam_z_rot -= cam_z_rot * 10.0f * deltaTime;
    }

    cams[0].setRotation(
        {cams[0].getRotation().x, cams[0].getRotation().y, cam_z_rot});

    cams[0].move(Vector::Normalize(dir) * camSpeed * deltaTime);

    // camera reset
    if (mainWin.getKey(GLFW_KEY_R) || cams[0].getPosition().y < -100.0f) {
      cams[0].setRotation({0, 0, 0});
      cams[0].setPosition({0, camHeight, 0});
      upVel = 0;
    }

    // ray
    ray.start = cams[0].getPosition();
    ray.direction = {0, -1, 0};

    // collision test
    if (compartments != nullptr) {
      do {
        collided = Physics::checkCollisionRayMesh(
            ray, meshes[0], *compartments->next()->value->value->ptr, &out);
        if (collided)
          break;
      } while (compartments->current()->previous != nullptr);
      compartments->rewind();
    }
    if (!collided) {
      collided = Physics::checkCollisionRaySurface(ray, groundPlane, &out);
    }

    /*if (collided) {
      print(out);
      println(cams[0].getPosition());
    }*/

    if (mainWin.getKey(GLFW_KEY_LEFT_CONTROL))
      camHeight = 0.85f;
    else
      camHeight = 1.7f;

    upVel += G * deltaTime;
    if (collided && out.tConstant < camHeight &&
        mainWin.getKey(GLFW_KEY_SPACE)) {
      upVel = 4.5f;
    } else if (collided && out.tConstant < camHeight) {
      upVel = 0;
      cams[0].setPosition({cams[0].getPosition().x,
                           out.hitPosition.y + camHeight,
                           cams[0].getPosition().z});
    }

    cams[0].move({0, upVel * deltaTime, 0});

    // event polling
    mainWin.checkEvents();

    // rendering
    newRen.useFramebuffer(0);
    mainWin.clearScreen();
    cams[current_cam].setAspectRatio((float)resolution_height /
                                     (float)resolution_width);
    newRen.render(mainScene, cams[current_cam]);
    newRen.render_skybox(0, prg_skybox, cams[current_cam]);

    newRen.renderFramebuffer(0, mainWin.getWindowResolution());

    // swap buffers
    mainWin.updateScreen();
  }

  return 0;
}
