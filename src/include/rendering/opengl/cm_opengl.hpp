#ifndef CM_OPENGL_RENDERER
#define CM_OPENGL_RENDERER

#include "../../types/classes/camera.hpp"
#include "../../types/classes/light.hpp"
#include "../../types/classes/mesh.hpp"
#include "include/OGL_Program.hpp"
#include "include/OGL_Types.hpp"
#include <GL/glew.h>

// header for using opengl at rendering
// functions for rendering usage with opengl
// are defined here

// data structures
struct OGL_RendererData {};

class OGL_Renderer {
protected:
  // instance vars

public:
  // constructors
  OGL_Renderer();
  OGL_Renderer(OGL_RendererData data);

  // getters

  // setters

  // rendering
  void render(OGL_Renderable& toRender, Camera& camera, OGL_Program& program,
              PointLight& light);

  // utility
  static OGL_Renderable genRenderable(Mesh genFrom, void* dataStorage);
};

#endif