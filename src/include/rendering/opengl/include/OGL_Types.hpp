#ifndef CM_OGL_TYPES
#define CM_OGL_TYPES

#include <cstdint>

// Structs and other stuff for opengl rendering
// Basically to make life livable
struct OGL_Renderable {
  uint32_t vertexDataBuffer;
  uint32_t indexbuffer;
  uint32_t vertexArray;
};

#endif