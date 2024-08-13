#ifndef CM_GAMEOBJECT
#define CM_GAMEOBJECT

#include <types/classes/transform.hpp>

// After the addition of material class it will be added here as well

struct GameObjectData {
  TransformData transformD;
  int meshID;
  int materialID;
};

// wraps transform and mesh data to make easily renderable middle class
// Used for rendering processes data storage
class GameObject : public Transform {
protected:
  // for mesh and material use registered data from renderer
  int meshID;
  int materialID;

public:
  // constructor
  GameObject();
  GameObject(GameObjectData data);

  // destructors
  ~GameObject();

  // setters
  void setMesh(int meshID);
  void setMaterial(int materialID);

  // getters
  int getMesh();
  int getMaterial();
};

#endif
