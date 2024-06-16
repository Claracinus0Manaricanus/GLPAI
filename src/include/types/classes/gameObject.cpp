#include "gameObject.hpp"

// constructors
GameObject::GameObject() {}

GameObject::GameObject(GameObjectData data)
    : Transform(data.transformD), Mesh(data.meshD) {}

// destructor
GameObject::~GameObject() {}