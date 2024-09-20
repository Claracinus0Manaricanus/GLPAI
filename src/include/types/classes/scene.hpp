#ifndef CM_SCENE
#define CM_SCENE

#include <cstdint>
#include <types/classes/gameObject.hpp>
#include <types/classes/light.hpp>
#include <types/classes/mesh.hpp>
#include <vector>

struct SceneData {
  std::vector<GameObject> gameObjects;
  std::vector<PointLight> pointLights;
  std::vector<DirectLight> directLights;
};

// ElementData is for use with arrays, its purpose is to give elements
// additional information.
struct Element_Data {
  const char* name;
  unsigned int tag;
};

class Scene {
private:
  // Scene objects
  std::vector<GameObject> gameObjects;
  std::vector<PointLight> pointLights;
  std::vector<DirectLight> directLights;

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
  void addDirectLight(DirectLight& toAdd);
  void addDirectLight(DirectLightData& toAdd);

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
  DirectLight& getDirectLight(uint32_t index);
  DirectLight& getLastLoadedDirectLight();
  std::vector<DirectLight>& getDirectLights();

  // loaders
  static std::vector<Mesh> import(const char* filename);

  // savers ( export )
  int saveTo(const char* filename);
};

#endif
