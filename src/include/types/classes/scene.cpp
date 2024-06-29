#include "scene.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cassert>

// constructors
Scene::Scene() {}

Scene::Scene(SceneData& data) {
  //
  this->gameObjects = data.gameObjects;
}

// destructors
Scene::~Scene() {}

// adders
void Scene::addGameObject(GameObject& toAdd) { gameObjects.push_back(toAdd); }

void Scene::addGameObject(GameObjectData& toAdd) {
  GameObject temp(toAdd);
  gameObjects.push_back(temp);
}

// removers
void Scene::removeGameObject(uint32_t index) {
  assert(index < gameObjects.size());
  gameObjects.erase(gameObjects.cbegin() + index);
}

// getters
GameObject& Scene::getGameObject(uint32_t index) {
  assert(index < gameObjects.size());
  return gameObjects[index];
}

GameObject& Scene::getLastLoadedGameObject() {
  return gameObjects[gameObjects.size() - 1];
}

std::vector<GameObject>& Scene::getGameObjects() { return gameObjects; }

// loaders ( import )
// helper functions
void searchNode(aiNode* node, const aiScene* impScene,
                std::vector<GameObject>& objs) {
  // load from node
  if (node->mNumMeshes != 0) {
    uint32_t numVertices = impScene->mMeshes[node->mMeshes[0]]->mNumVertices;
    uint32_t numFaces = impScene->mMeshes[node->mMeshes[0]]->mNumFaces;

    std::vector<Vertex> vertices(numVertices);
    std::vector<uint32_t> indices(numFaces * 3);

    for (int i = 0; i < numVertices; i++) {
      Vertex tempV;
      vertices[i].position = {
          impScene->mMeshes[node->mMeshes[0]]->mVertices[i].x,
          impScene->mMeshes[node->mMeshes[0]]->mVertices[i].y,
          impScene->mMeshes[node->mMeshes[0]]->mVertices[i].z};
      vertices[i].normal = {impScene->mMeshes[node->mMeshes[0]]->mNormals[i].x,
                            impScene->mMeshes[node->mMeshes[0]]->mNormals[i].y,
                            impScene->mMeshes[node->mMeshes[0]]->mNormals[i].z};
      vertices[i].uv = {0, 0};
    }
    for (int i = 0; i < numFaces; i++) {
      indices[i * 3] =
          impScene->mMeshes[node->mMeshes[0]]->mFaces[i].mIndices[0];
      indices[i * 3 + 1] =
          impScene->mMeshes[node->mMeshes[0]]->mFaces[i].mIndices[1];
      indices[i * 3 + 2] =
          impScene->mMeshes[node->mMeshes[0]]->mFaces[i].mIndices[2];
    }

    GameObjectData gData;
    gData.meshD.vertices = vertices;
    gData.meshD.indexBuffer = indices;

    objs.emplace_back(gData);
  }

  // search children
  for (int i = 0; i < node->mNumChildren; i++) {
    searchNode(node->mChildren[i], impScene, objs);
  }
}

int Scene::loadFrom(const char* filename) {
  Assimp::Importer importer;
  const aiScene* importedScene =
      importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenNormals);
  if (importedScene == nullptr)
    return -1;

  // load recursively from the tree ( starting at importedScene->mRootNode )
  searchNode(importedScene->mRootNode, importedScene, gameObjects);
  return 0;
}

// savers ( export )
int Scene::saveTo(const char* filename) {
  // use assimp to save to a file
  return 0;
}