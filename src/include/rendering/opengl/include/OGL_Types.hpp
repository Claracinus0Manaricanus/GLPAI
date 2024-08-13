#ifndef CM_OGL_TYPES
#define CM_OGL_TYPES

#include <cstdint>
#include <types/vectors.hpp>

// Structs and other stuff for opengl rendering
// Basically to make life livable

struct OGL_Texture {
  unsigned int texID;
  int target;
  int format;
  int width;
  int height;
};

struct OGL_Cubemap {
  unsigned int texID;
};

struct OGL_Mesh {
  uint32_t vertexDataBuffer;
  uint32_t indexbuffer;
  uint32_t indexBufferlength;
  uint32_t vertexArray;
};

struct OGL_Material {
  OGL_Texture texture;
  Vec4 color;
  float metallic;
};

struct OGL_ProgramData {
  const char* vertexSourcePath;
  const char* fragSourcePath;
  const char* geometrySourcePath;
};

#endif
