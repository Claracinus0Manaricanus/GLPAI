#include "gameObject.hpp"

// constructors
GameObject::GameObject() : Transform() {
  meshID = -1;
  materialID = -1;
  hitvolumeID = -1;
}

GameObject::GameObject(GameObjectData data) : Transform(data.transformD) {
  meshID = data.meshID;
  materialID = data.materialID;
  hitvolumeID = data.hitvolumeID;
}

// destructor
GameObject::~GameObject() {}

// setters
void GameObject::setMesh(int meshID) { this->meshID = meshID; }
void GameObject::setMaterial(int materialID) { this->materialID = materialID; }
void GameObject::setHitvolume(int hitvolumeID) {
  this->hitvolumeID = hitvolumeID;
}

// getters
int GameObject::getMesh() { return meshID; }
int GameObject::getMaterial() { return materialID; }
int GameObject::getHitvolume() { return hitvolumeID; }
