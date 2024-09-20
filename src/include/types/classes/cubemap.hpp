#ifndef CM_SKYBOX
#define CM_SKYBOX

#include <types/classes/image.hpp>

#define CM_CUBE_MAP_POSITIVE_X 0
#define CM_CUBE_MAP_NEGATIVE_X 1
#define CM_CUBE_MAP_POSITIVE_Y 2
#define CM_CUBE_MAP_NEGATIVE_Y 3
#define CM_CUBE_MAP_POSITIVE_Z 4
#define CM_CUBE_MAP_NEGATIVE_Z 5

class Cubemap {
protected:
  Image images[6];

public:
  // cosntructors
  Cubemap();
  Cubemap(const char* filenames[6]);

  // setters
  void loadFiles(const char* filenames[6]);

  // getters
  Image& getTexture(int side);
};

#endif
