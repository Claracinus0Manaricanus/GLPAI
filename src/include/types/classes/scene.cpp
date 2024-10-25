#include "scene.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cassert>
#include <cstdlib>

// constructors
Scene::Scene() {}

// destructors
Scene::~Scene() {}

// adders
TreeNode<GameObject_Storage>* Scene::addGameObject(
    TreeNode<GameObject_Storage>* parentNode, GameObject& toAdd,
    Element_Data tags) {
  int index = 0;
  int found = 0;
  for (GameObject_Tagged_Storage* i : gOTagList) {
    if (i->tag == tags.tag) {
      found = 1;
      break;
    }
    if (i->tag > tags.tag)
      break;

    index++;
  }

  if (!found) { // if not found
    GameObject_Tagged_Storage* tmp_list = new GameObject_Tagged_Storage();
    tmp_list->tag = tags.tag;
    tmp_list->list.setFreeEnabled(0);
    gOTagList.insert(gOTagList.cbegin() + index, tmp_list);
  }

  gOTagList[index]->list.addNode(gameObjects.addChildTo(
      parentNode,
      new GameObject_Storage({new GameObject(toAdd), {tags.name, tags.tag}})));

  return gOTagList[index]->list.last()->value;
}

TreeNode<GameObject_Storage>* Scene::addGameObject(
    TreeNode<GameObject_Storage>* parentNode, GameObjectData& toAdd,
    Element_Data tags) {
  int index = 0;
  int found = 0;
  for (GameObject_Tagged_Storage* i : gOTagList) {
    if (i->tag == tags.tag) {
      found = 1;
      break;
    }
    if (i->tag > tags.tag)
      break;

    index++;
  }

  if (!found) { // if not found
    GameObject_Tagged_Storage* tmp_list = new GameObject_Tagged_Storage();
    tmp_list->tag = tags.tag;
    tmp_list->list.setFreeEnabled(0);
    gOTagList.insert(gOTagList.cbegin() + index, tmp_list);
  }

  gOTagList[index]->list.addNode(gameObjects.addChildTo(
      parentNode,
      new GameObject_Storage({new GameObject(toAdd), {tags.name, tags.tag}})));

  return gOTagList[index]->list.last()->value;
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
void removeGameObjectHelper(Scene* scene, TreeNode<GameObject_Storage>* node) {
  for (int i = 0; i < node->childCount; i++) {
    removeGameObjectHelper(scene, node->childs[i]);
  }

  cm_LinkedList<TreeNode<GameObject_Storage>>* tmp_list =
      scene->getGameObjects(node->value->metaData.tag);

  tmp_list->rewind();
  for (int i = 0; i < tmp_list->size(); i++) {
    if (node == tmp_list->next()->value)
      break;
  }

  tmp_list->previous();
  tmp_list->removeNode(tmp_list->current());
}

void Scene::removeGameObject(TreeNode<GameObject_Storage>* toRemove) {
  removeGameObjectHelper(this, toRemove);
  gameObjects.deleteNode(toRemove);
}

// getters
cm_Tree<GameObject_Storage>* Scene::getGameObjects() { return &gameObjects; }

cm_LinkedList<TreeNode<GameObject_Storage>>* Scene::getGameObjects(int tag) {
  // performance is good enough for now but if the program slows this search
  // function can be changed with a faster one

  for (int i = 0; i < gOTagList.size(); i++) {
    if (gOTagList[i]->tag == tag)
      return &gOTagList[i]->list;
  }

  return nullptr;
}

std::vector<GameObject_Tagged_Storage*>& Scene::getGameObjectTags() {
  return gOTagList;
}

cm_LinkedList<TreeNode<GameObject_Storage>>* Scene::getGameObjectTag(
    uint32_t index) {
  assert(index < gOTagList.size());
  return &gOTagList[index]->list;
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
