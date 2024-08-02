#ifndef CM_OPENGL_RENDERER
#define CM_OPENGL_RENDERER

#include "../../types/classes/camera.hpp"
#include "../../types/classes/light.hpp"
#include "../../types/classes/material.hpp"
#include "../../types/classes/mesh.hpp"
#include "../../types/classes/scene.hpp"
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
  std::vector<OGL_Mesh> meshes;
  std::vector<OGL_Material> materials;

public:
  // constructors
  OGL_Renderer();
  OGL_Renderer(OGL_RendererData data);

  // destructors
  ~OGL_Renderer();

  // getters

  // setters
  void register_mesh(Mesh& mesh);
  void register_material(Material& material);

  // rendering
  void render(int mesh, int material, OGL_Program& program,
              Transform& transform, Camera& camera, PointLight& light);
  void render(Scene& scene, OGL_Program& program, Camera& camera);
};

#endif
