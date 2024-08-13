#ifndef CM_SKYBOX
#define CM_SKYBOX

typedef struct {
  unsigned char* textures[6]; // +X, -X, +Y, -Y, +Z, -Z
  int widths[6];
  int height[6];
  int channels[6];
} Cubemap;

class Skybox {
private:
  Cubemap skyboxMap;

public:
  Skybox();
  Skybox(Cubemap& data);
  Skybox(const char* filenames[6]);
};

#endif
