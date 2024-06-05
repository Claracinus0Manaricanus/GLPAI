#ifndef CM_OPENGL
#define CM_OPENGL

#include "../../types/classes/mesh.hpp"
#include "include/OGL_Types.hpp"
#include <GL/glew.h>
#include <vector>

// header for using opengl at rendering
// functions for rendering usage with opengl
// are defined here

class OGL_Renderer {
private:
  // instance vars
  std::vector<uint32_t> programs;

public:
  // constructors
  OGL_Renderer();
  OGL_Renderer(std::vector<uint32_t>& programs);

  // getters

  // setters
  int addProgram(uint32_t& programID);

  // utility
  static OGL_Renderable genRenderable(Mesh genFrom);
};

#endif