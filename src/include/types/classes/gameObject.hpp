#ifndef CM_GAMEOBJECT
#define CM_GAMEOBJECT

#include <types/classes/transform.hpp>

struct GameObjectData {
  TransformData transformD;
  int meshID = -1;
  int materialID = -1;
  int hitvolumeID = -1;
};

class GameObject : public Transform {
protected:
  // for mesh and material use registered data from renderer
  int meshID;
  int materialID;
  int hitvolumeID;

public:
  // constructor
  GameObject();
  GameObject(GameObjectData data);

  // destructors
  ~GameObject();

  // setters
  void setMesh(int meshID);
  void setMaterial(int materialID);
  void setHitvolume(int hitvolumeID);

  // getters
  int getMesh();
  int getMaterial();
  int getHitvolume();
};

#endif
