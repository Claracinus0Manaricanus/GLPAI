#include "gameObject.hpp"

// constructors
GameObject::GameObject() : Transform() {
  meshID = 0;
  materialID = 0;
}

GameObject::GameObject(GameObjectData data) : Transform(data.transformD) {
  meshID = data.meshID;
  materialID = data.materialID;
}

// destructor
GameObject::~GameObject() {}

// setters
void GameObject::setMesh(int meshID) { this->meshID = meshID; }
void GameObject::setMaterial(int materialID) { this->materialID = materialID; }

// getters
int GameObject::getMesh() { return meshID; }
int GameObject::getMaterial() { return materialID; }
