#ifndef CM_OPENGL_RENDERER
#define CM_OPENGL_RENDERER

#include "../../types/classes/mesh.hpp"
#include "include/OGL_Types.hpp"
#include "include/OGL_Program.hpp"
#include <GL/glew.h>

// header for using opengl at rendering
// functions for rendering usage with opengl
// are defined here

class OGL_Renderer {
private:
  // instance vars
  // to add:
  // camera
  // lights
  // material

public:
  // constructors
  OGL_Renderer();

  // getters

  // setters

  // rendering
  void renderOGL_Renderable(OGL_Program& program, OGL_Renderable& toRender);

  // utility
  static OGL_Renderable genRenderable(Mesh genFrom);
};

#endif