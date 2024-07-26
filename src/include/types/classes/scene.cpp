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
  this->pointLights = data.pointLights;
}

// destructors
Scene::~Scene() {}

// adders
void Scene::addGameObject(GameObject& toAdd) { gameObjects.push_back(toAdd); }

void Scene::addGameObject(GameObjectData& toAdd) {
  gameObjects.emplace_back(toAdd);
}

void Scene::addPointLight(PointLight& toAdd) { pointLights.push_back(toAdd); }

void Scene::addPointLight(PointLightData& toAdd) {
  pointLights.emplace_back(toAdd);
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

PointLight& Scene::getPointLight(uint32_t index) {
  assert(index < pointLights.size());
  return pointLights[index];
}

PointLight& Scene::getLastLoadedPointLight() {
  return pointLights[pointLights.size() - 1];
}

std::vector<PointLight>& Scene::getPointLights() { return pointLights; }

// loaders ( import )
// helper functions
void searchNode(aiNode* node, const aiScene* impScene,
                std::vector<Mesh>& tempMeshes) {
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
      if (impScene->mMeshes[node->mMeshes[0]]->mTextureCoords[0] != nullptr) {
        vertices[i].uv.x =
            impScene->mMeshes[node->mMeshes[0]]->mTextureCoords[0][i].y;
        vertices[i].uv.y =
            impScene->mMeshes[node->mMeshes[0]]->mTextureCoords[0][i].x;
      } else {
        vertices[i].uv = {0, 0};
      }
    }
    for (int i = 0; i < numFaces; i++) {
      indices[i * 3] =
          impScene->mMeshes[node->mMeshes[0]]->mFaces[i].mIndices[0];
      indices[i * 3 + 1] =
          impScene->mMeshes[node->mMeshes[0]]->mFaces[i].mIndices[1];
      indices[i * 3 + 2] =
          impScene->mMeshes[node->mMeshes[0]]->mFaces[i].mIndices[2];
    }

    MeshData mData;
    mData.vertices = vertices;
    mData.indexBuffer = indices;

    tempMeshes.emplace_back(mData);
  }

  // search children
  for (int i = 0; i < node->mNumChildren; i++) {
    searchNode(node->mChildren[i], impScene, tempMeshes);
  }
}

std::vector<Mesh> Scene::import(const char* filename) {
  std::vector<Mesh> tmpMeshes;
  Assimp::Importer importer;
  const aiScene* importedScene =
      importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_GenNormals);
  if (importedScene == nullptr)
    return tmpMeshes;

  // load recursively from the tree ( starting at importedScene->mRootNode )
  searchNode(importedScene->mRootNode, importedScene, tmpMeshes);
  return tmpMeshes;
}

// savers ( export )
int Scene::saveTo(const char* filename) {
  // use assimp to save to a file
  return 0;
}