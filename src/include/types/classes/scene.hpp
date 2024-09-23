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
protected:
  // Scene objects
  std::vector<GameObject> gameObjects;
  std::vector<Element_Data> gameObjectTags;

  std::vector<PointLight> pointLights;
  std::vector<DirectLight> directLights;

public:
  // constructors
  Scene();
  Scene(SceneData& data);

  // destructors
  ~Scene();

  // adders
  void addGameObject(GameObject& toAdd, Element_Data tags = {nullptr, 0});
  void addGameObject(GameObjectData& toAdd, Element_Data tags = {nullptr, 0});
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
  int getGameObjectIndex(const char* name); // returns the index of the
                                            // GameObject with the name, if no
                                            // match is found returns -1
  GameObject& getLastLoadedGameObject();
  std::vector<GameObject>& getGameObjects();
  IVec2 getGameObjects(int tag); // returns the start and end index of
                                 // GameOjects that have the tag
  std::vector<Element_Data>& getGameObjectTags();
  Element_Data getGameObjectTag(uint32_t index);

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
