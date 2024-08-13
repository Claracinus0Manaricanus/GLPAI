#ifndef CM_SCENE
#define CM_SCENE

#include <cstdint>
#include <types/classes/gameObject.hpp>
#include <types/classes/light.hpp>
#include <types/classes/mesh.hpp>
#include <types/classes/skybox.hpp>
#include <vector>

struct SceneData {
  std::vector<GameObject> gameObjects;
  std::vector<PointLight> pointLights;
};

class Scene {
private:
  // Scene objects
  std::vector<GameObject> gameObjects;
  std::vector<PointLight> pointLights;

  // skybox and canvas
  Skybox activeSkybox;

public:
  // constructors
  Scene();
  Scene(SceneData& data);

  // destructors
  ~Scene();

  // adders
  void addGameObject(GameObject& toAdd);
  void addGameObject(GameObjectData& toAdd);
  void addPointLight(PointLight& toAdd);
  void addPointLight(PointLightData& toAdd);

  // removers
  void removeGameObject(uint32_t index);

  // getters
  // note: after getting an object it should be used immediately
  // and not stored, since std::vector can change where its data is
  // located the reference to the object might get invalidated
  GameObject& getGameObject(uint32_t index);
  GameObject& getLastLoadedGameObject();
  std::vector<GameObject>& getGameObjects();
  PointLight& getPointLight(uint32_t index);
  PointLight& getLastLoadedPointLight();
  std::vector<PointLight>& getPointLights();

  // loaders
  static std::vector<Mesh> import(const char* filename);

  // savers ( export )
  int saveTo(const char* filename);
};

#endif
