#ifndef CM_SCENE
#define CM_SCENE

#include "gameObject.hpp"
#include <vector>

struct SceneData {
  std::vector<GameObject> gameObjects;
};

class Scene {
private:
  std::vector<GameObject> gameObjects;
  // lights

public:
  // constructors
  Scene();
  Scene(SceneData& data);

  // destructors
  ~Scene();

  // adders
  void addGameObject(GameObject& toAdd);
  void addGameObject(GameObjectData& toAdd);
  // void addLight();

  // removers
  void removeGameObject(uint32_t index);

  // getters
  GameObject& getGameObject(uint32_t index);
  GameObject& getLastLoadedGameObject();
  std::vector<GameObject>& getGameObjects();

  // loaders
  int import(const char* filename);

  // savers ( export )
  int saveTo(const char* filename);
};

#endif