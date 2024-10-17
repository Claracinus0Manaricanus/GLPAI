#ifndef CM_SCENE
#define CM_SCENE

#include <cstdint>
#include <types/classes/gameObject.hpp>
#include <types/classes/light.hpp>
#include <types/classes/mesh.hpp>
#include <types/data_structures/linked_list.hpp>
#include <types/data_structures/tree.hpp>
#include <vector>

struct SceneData {};

// ElementData is for use with arrays, its purpose is to give elements
// additional information.
struct Element_Data {
  const char* name;
  unsigned int tag;
};

struct GameObject_Storage {
  GameObject* ptr;
  Element_Data metaData;
};

struct GameObject_Tagged_Storage {
  cm_LinkedList<TreeNode<GameObject_Storage>> list;
  int tag;
};

class Scene {
protected:
  // Scene objects
  cm_Tree<GameObject_Storage> gameObjects;
  std::vector<GameObject_Tagged_Storage*> gOTagList;

  std::vector<PointLight> pointLights;
  std::vector<DirectLight> directLights;

public:
  // constructors
  Scene();

  // destructors
  ~Scene();

  // adders
  TreeNode<GameObject_Storage>* addGameObject(
      TreeNode<GameObject_Storage>* parentNode, GameObject& toAdd,
      Element_Data tags = {nullptr, 0});
  TreeNode<GameObject_Storage>* addGameObject(
      TreeNode<GameObject_Storage>* parentNode, GameObjectData& toAdd,
      Element_Data tags = {nullptr, 0});
  void addPointLight(PointLight& toAdd);
  void addPointLight(PointLightData& toAdd);
  void addDirectLight(DirectLight& toAdd);
  void addDirectLight(DirectLightData& toAdd);

  // removers
  void removeGameObject(uint32_t index); // WIP

  // getters
  cm_Tree<GameObject_Storage>* getGameObjects();
  cm_LinkedList<TreeNode<GameObject_Storage>>* getGameObjects(
      int tag); // returns a linkedList of gameObjects with tag
  std::vector<GameObject_Tagged_Storage*>& getGameObjectTags();
  cm_LinkedList<TreeNode<GameObject_Storage>>* getGameObjectTag(uint32_t index);

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
