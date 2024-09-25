#include "scene.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cassert>
#include <cstdlib>

// constructors
Scene::Scene() {}

Scene::Scene(SceneData& data) {
  //
  this->gameObjects = data.gameObjects;
  this->pointLights = data.pointLights;
  this->directLights = data.directLights;
}

// destructors
Scene::~Scene() {
  for (Element_Data& i : gameObjectTags) {
    free((void*)i.name);
  }
}

// adders
void Scene::addGameObject(GameObject& toAdd, Element_Data tags) {
  // needs optimization the elements are ordered so write a better algorithm
  // here
  int min = 0, max = gameObjects.size();
  int indexToInsert = max / 2;

  while (indexToInsert > min) {
    if (gameObjectTags[indexToInsert].tag > tags.tag) {
      max = indexToInsert;
    } else if (gameObjectTags[indexToInsert].tag < tags.tag) {
      min = indexToInsert;
    } else if (gameObjectTags[indexToInsert].tag == tags.tag) {
      break;
    }
    indexToInsert = (min + max) / 2;
  }
  if (max > 0)
    if (gameObjectTags[indexToInsert].tag < tags.tag)
      indexToInsert++;

  gameObjects.insert(indexToInsert + gameObjects.begin(), toAdd);

  char* copyFromTags = nullptr;
  if (tags.name != nullptr) {
    int length = strlen(tags.name) + 1;
    copyFromTags = (char*)malloc(length);
    memcpy(copyFromTags, tags.name, length);
  }
  gameObjectTags.insert(indexToInsert + gameObjectTags.begin(),
                        {copyFromTags, tags.tag});
}

void Scene::addGameObject(GameObjectData& toAdd, Element_Data tags) {
  // needs optimization the elements are ordered so write a better algorithm
  // here
  int min = 0, max = gameObjects.size();
  int indexToInsert = max / 2;

  while (indexToInsert > min) {
    if (gameObjectTags[indexToInsert].tag > tags.tag) {
      max = indexToInsert;
    } else if (gameObjectTags[indexToInsert].tag < tags.tag) {
      min = indexToInsert;
    } else if (gameObjectTags[indexToInsert].tag == tags.tag) {
      break;
    }
    indexToInsert = (min + max) / 2;
  }
  if (max > 0)
    if (gameObjectTags[indexToInsert].tag < tags.tag)
      indexToInsert++;

  gameObjects.emplace(indexToInsert + gameObjects.begin(), toAdd);

  char* copyFromTags = nullptr;
  if (tags.name != nullptr) {
    int length = strlen(tags.name) + 1;
    copyFromTags = (char*)malloc(length);
    memcpy(copyFromTags, tags.name, length);
  }
  gameObjectTags.insert(indexToInsert + gameObjectTags.begin(),
                        {copyFromTags, tags.tag});
}

void Scene::addPointLight(PointLight& toAdd) { pointLights.push_back(toAdd); }

void Scene::addPointLight(PointLightData& toAdd) {
  pointLights.emplace_back(toAdd);
}

void Scene::addDirectLight(DirectLight& toAdd) {
  directLights.push_back(toAdd);
}

void Scene::addDirectLight(DirectLightData& toAdd) {
  directLights.emplace_back(toAdd);
}

// removers
void Scene::removeGameObject(uint32_t index) {
  assert(index < gameObjects.size());
  gameObjects.erase(gameObjects.cbegin() + index);
  gameObjectTags.erase(gameObjectTags.cbegin() + index);
}

// getters
GameObject& Scene::getGameObject(uint32_t index) {
  assert(index < gameObjects.size());
  return gameObjects[index];
}

int Scene::getGameObjectIndex(const char* name) {
  for (int i = 0; i < gameObjects.size(); i++) {
    if (gameObjectTags[i].name == nullptr)
      continue;
    if (strcmp(gameObjectTags[i].name, name) == 0)
      return i;
  }

  return -1;
}

GameObject& Scene::getLastLoadedGameObject() {
  return gameObjects[gameObjects.size() - 1];
}

std::vector<GameObject>& Scene::getGameObjects() { return gameObjects; }

IVec2 Scene::getGameObjects(int tag) {
  // performance is good enough for now but if the program slows this search
  // function can be changed with a faster one

  int min = -1, max = -1;
  for (int i = 0; i < gameObjectTags.size(); i++) {
    if (gameObjectTags[i].tag == tag && min == -1) {
      min = i;
    } else if (gameObjectTags[i].tag != tag && min != -1) {
      break;
    }
    max = i;
  }

  return {min, max};
}

std::vector<Element_Data>& Scene::getGameObjectTags() { return gameObjectTags; }

Element_Data Scene::getGameObjectTag(uint32_t index) {
  assert(index < gameObjectTags.size());
  return gameObjectTags[index];
}

PointLight& Scene::getPointLight(uint32_t index) {
  assert(index < pointLights.size());
  return pointLights[index];
}

PointLight& Scene::getLastLoadedPointLight() {
  return pointLights[pointLights.size() - 1];
}

std::vector<PointLight>& Scene::getPointLights() { return pointLights; }

DirectLight& Scene::getDirectLight(uint32_t index) {
  assert(index < directLights.size());
  return directLights[index];
}

DirectLight& Scene::getLastLoadedDirectLight() {
  return directLights[directLights.size() - 1];
}

std::vector<DirectLight>& Scene::getDirectLights() { return directLights; }

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
            impScene->mMeshes[node->mMeshes[0]]->mTextureCoords[0][i].x;
        vertices[i].uv.y =
            -impScene->mMeshes[node->mMeshes[0]]->mTextureCoords[0][i].y;
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
