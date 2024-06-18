#ifndef CM_GAMEOBJECT
#define CM_GAMEOBJECT

#include "mesh.hpp"
#include "transform.hpp"
#include "material.hpp"

// After the addition of material class it will be added here as well

struct GameObjectData {
    MeshData meshD;
    TransformData transformD;
    MaterialData materialD;
};

// wraps transform and mesh data to make easily renderable middle class
// Used for rendering processes data storage
class GameObject : public Transform, public Mesh, public Material{
protected:
public:
  // constructor
  GameObject();
  GameObject(GameObjectData data);

  // destructors
  ~GameObject();

  // setters

  // getters
};

#endif