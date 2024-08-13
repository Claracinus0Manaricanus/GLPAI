#ifndef CM_SKYBOX
#define CM_SKYBOX

#include <types/classes/material.hpp>

#define CM_CUBE_MAP_POSITIVE_X 0
#define CM_CUBE_MAP_NEGATIVE_X 1
#define CM_CUBE_MAP_POSITIVE_Y 2
#define CM_CUBE_MAP_NEGATIVE_Y 3
#define CM_CUBE_MAP_POSITIVE_Z 4
#define CM_CUBE_MAP_NEGATIVE_Z 5

typedef struct {
  TextureData textures[6]; // +X, -X, +Y, -Y, +Z, -Z
} Cubemap;

class Skybox {
private:
  Cubemap skyboxMap;

public:
  // cosntructors
  Skybox();
  Skybox(Cubemap& data);
  Skybox(const char* filenames[6]);

  // setters

  // getters
  TextureData& getTexture(int side);
};

#endif
