#include "include/cm_math/operations.hpp"
#include "include/physics/collisions.hpp"
#include "include/rendering/opengl/cm_opengl.hpp"
#include "include/rendering/window/window.hpp"
#include "include/types/classes/gameObject.hpp"
#include "include/types/physics.hpp"
#include "include/utility/printUtil.hpp"
#include <cstdio>

// assimp
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

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
  triangle.vertices[0] = {{-20.0f, -0.5f, 20.0f}, {0, 1, 0}, {0, 0}};

  triangle.vertices[1] = {{20.0f, -0.5f, 20.0f}, {0, 1, 0}, {0, 0}};

  triangle.vertices[2] = {{0.0f, -0.5f, -20.0f}, {0, 1, 0}, {0, 0}};
  newGOBJ.addTriangle(triangle);

  // imports
  Assimp::Importer importer;
  const aiScene* imported =
      importer.ReadFile("assets/models/dragon_vrip.ply", aiProcess_Triangulate | aiProcess_GenNormals);
  if (imported == nullptr) {
    printf("import error!\n");
  }

  uint32_t faceNum = imported->mMeshes[0]->mNumFaces;
  uint32_t vertNum = imported->mMeshes[0]->mNumVertices;

  std::vector<uint32_t> indBuff(faceNum * 3);
  std::vector<Vertex> vertices(vertNum);

  for (int i = 0; i < vertNum; i++) {
    vertices[i].position = {imported->mMeshes[0]->mVertices[i].x,
                            imported->mMeshes[0]->mVertices[i].y,
                            imported->mMeshes[0]->mVertices[i].z};
    vertices[i].normal = {imported->mMeshes[0]->mNormals[i].x,
                          imported->mMeshes[0]->mNormals[i].y,
                          imported->mMeshes[0]->mNormals[i].z};
    vertices[i].uv = {0, 0};
  }

  for (int i = 0; i < faceNum; i++) {
    indBuff[i * 3] = imported->mMeshes[0]->mFaces[i].mIndices[0];
    indBuff[i * 3 + 1] = imported->mMeshes[0]->mFaces[i].mIndices[1];
    indBuff[i * 3 + 2] = imported->mMeshes[0]->mFaces[i].mIndices[2];
  }

  GameObjectData gData;
  gData.meshD.vertices = vertices;
  gData.meshD.indexBuffer = indBuff;
  gData.transformD.position = {0, 0, 0};
  gData.transformD.scale = {10, 10, 10};

  GameObject importedOBJ(gData);

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
  OGL_Renderable testRen2 = newRen.genRenderable(importedOBJ, &importedOBJ);
  newRen.setProgram(&prg);

  // vars
  Vec2 mousePos = {0, 0};
  uint32_t lastMiliSec = test.getTicks();
  float deltaTime = 0;
  int collided = 0;
  char captureMouse = 1, escA = 1;

  float G = -9.8f;
  float upVel = 0;
  float camHeight = 1.7f;

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
    if (keyStates[SDL_SCANCODE_R] || cam.getPosition().y < -100.0f) {
      cam.setRotation({0, 0, 0});
      cam.setPosition({0, camHeight, 0});
    }

    // ray
    ray.start = cam.getPosition();
    ray.direction = {0, -1, 0};

    // collision testing
    collided = Physics::checkCollisionRayGameObject(ray, newGOBJ, &out);

    if(keyStates[SDL_SCANCODE_LCTRL]) camHeight = 0.85f;
    else camHeight = 1.7f;

    upVel += G * deltaTime;
    if (collided && out.tConstant < camHeight && keyStates[SDL_SCANCODE_SPACE]) {
      upVel = 4.5f;
    } else if (collided && out.tConstant < camHeight) {
      upVel = 0;
      cam.setPosition(
          {cam.getPosition().x, out.hitPosition.y + camHeight, cam.getPosition().z});
    }

    cam.move({0, upVel * deltaTime, 0});

    // event polling
    test.checkEvents();

    // rendering
    test.clearScreen();
    cam.setAspectRatio(test.getAspectRatio());
    newRen.render(testRen);
    newRen.render(testRen2);

    // swap buffers
    test.updateScreen();
  }

  return 0;
}
