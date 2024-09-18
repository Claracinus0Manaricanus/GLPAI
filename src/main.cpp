#include <SDL2/SDL_scancode.h>
#include <cm_math/operations.hpp>
#include <cstdio>
#include <math.h>
#include <physics/collisions.hpp>
#include <rendering/opengl/cm_opengl.hpp>
#include <rendering/opengl/include/OGL_Program.hpp>
#include <rendering/window/window.hpp>
#include <types/classes/cubemap.hpp>
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

  // camera
  Camera cam;
  cam.setPosition({0, 10, 0});

  // OGL_Renderer
  OGL_Renderer newRen;
  const int resolution_width = 1920;
  const int resolution_height = 1080;
  printf("%d\n",
         newRen.create_framebuffer(resolution_width, resolution_height));

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

  const int prg_framebuffer = newRen.register_program(
      {"src/include/rendering/opengl/shaders/framebuffer/vert.glsl",
       "src/include/rendering/opengl/shaders/framebuffer/frag.glsl", NULL});
  if (newRen.getProgramError(prg_framebuffer) != NULL)
    printf("%s\n", newRen.getProgramError(prg_framebuffer));

  // importing meshes
  std::vector<Mesh> ground = Scene::import("assets/models/plane.obj");
  const int mesh_ground = newRen.register_meshes(ground);

  std::vector<Mesh> meshes = Scene::import("assets/models/dragon_vrip.ply");
  const int mesh_dragon = newRen.register_meshes(meshes);

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

  mData.color = {1, 0, 0, 1};
  mData.metallic = 1.0f;
  mData.prg_ID = prg_basic;
  Material tmpMat2(mData);
  const int material_redMetallic = newRen.register_material(tmpMat2);

  tmpMat.loadTexture("assets/images/equ.jpg");
  mData.prg_ID = prg_texture;
  const int material_equ = newRen.register_material(tmpMat);

  // Scene
  Scene mainScene;

  GameObjectData gData = {
      {{0, 0, 0}, {0, -PI / 2, 0}, {10, 10, 10}}, mesh_ground, material_arkTex};
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
  Mesh sphM(sph1, 100);

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
  Cubemap sky0(skyFiles);
  newRen.register_cubemap(sky0);

  // colon
  meshes = Scene::import("assets/models/colon.obj");
  const int mesh_colon = newRen.register_meshes(meshes);

  gData.materialID = material_redMetallic;
  gData.meshID = mesh_colon;
  gData.transformD.position = {0, 0, 0};
  mainScene.addGameObject(gData);

  // vars
  Vec2 mousePos = {0, 0};
  uint32_t lastMiliSec = mainWin.getTicks();
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

  SphereHit sph_out;
  Sphere player_sphere;

  // main loop
  while (!mainWin.shouldClose()) {
    // testing area //
    player_sphere.position = cam.getPosition();
    player_sphere.position.y -= camHeight / 2;
    player_sphere.radius = 0.2f;
    sph1.position = mainScene.getGameObject(3).getPosition();
    collided =
        Physics::checkCollisionSphereSphere(sph1, player_sphere, &sph_out);
    if (collided) {
      cam.move(sph_out.hitNormal * sph_out.overlap_distance);
    }
    // testing area //

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
    cam.rotate({mousePos.y, -mousePos.x, 0});

    // movement
    Vec3 movement = {
        ((float)keyStates[SDL_SCANCODE_D] - (float)keyStates[SDL_SCANCODE_A]),
        0,
        ((float)keyStates[SDL_SCANCODE_W] - (float)keyStates[SDL_SCANCODE_S])};

    if (keyStates[SDL_SCANCODE_LSHIFT]) {
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

    Vec3 dir =
        Vector::Normalize((Vec3){cam.getRight().x, 0, cam.getRight().z}) *
            movement.x +
        Vector::Normalize((Vec3){cam.getForward().x, 0, cam.getForward().z}) *
            movement.z;

    if (abs(cam_z_rot) < camSpeed * PI / 80.0f) {
      cam_z_rot += deltaTime * -movement.x * PI / 5.0f;
    } else if (movement.x != 0) {
      cam_z_rot = -movement.x * camSpeed * PI / 80.0f;
    }

    if (movement.x == 0) {
      cam_z_rot -= cam_z_rot * 10.0f * deltaTime;
    }

    cam.setRotation({cam.getRotation().x, cam.getRotation().y, cam_z_rot});

    cam.move(Vector::Normalize(dir) * camSpeed * deltaTime);

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
    collided = Physics::checkCollisionRayMesh(ray, ground[0],
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
    newRen.useFramebuffer(0);
    mainWin.clearScreen();
    cam.setAspectRatio((float)resolution_height / (float)resolution_width);
    newRen.render(mainScene, cam);
    newRen.render_skybox(0, prg_skybox, cam);
    mainWin.updateViewport();
    newRen.renderFramebuffer(0, prg_framebuffer);

    // swap buffers
    mainWin.updateScreen();
  }

  return 0;
}
