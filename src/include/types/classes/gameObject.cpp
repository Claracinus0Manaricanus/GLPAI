#include "gameObject.hpp"

// constructors
GameObject::GameObject() {}

GameObject::GameObject(GameObjectData data)
    : Transform(data.transformD), Mesh(data.meshD), Material(data.materialD) {}

// destructor
GameObject::~GameObject() {}

// setters

// getters