#ifndef CM_OPENGL_RENDERER
#define CM_OPENGL_RENDERER

#include "../../types/classes/camera.hpp"
#include "../../types/classes/mesh.hpp"
#include "include/OGL_Program.hpp"
#include "include/OGL_Types.hpp"
#include <GL/glew.h>

// header for using opengl at rendering
// functions for rendering usage with opengl
// are defined here

//data structures
struct OGL_RendererData{
  OGL_Program* program;
  Camera* camera;
};

class OGL_Renderer {
protected:
  // instance vars
  // to add:
  Camera* camera;
  // lights
  // material
  OGL_Program* program;

public:
  // constructors
  OGL_Renderer(OGL_RendererData data);

  // getters

  // setters
  void setProgram(OGL_Program* program);
  void setCamera(Camera* camera);

  // rendering
  void render(OGL_Renderable& toRender);

  // utility
  static OGL_Renderable genRenderable(Mesh genFrom, void* dataStorage);
};

#endif