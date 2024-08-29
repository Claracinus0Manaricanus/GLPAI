#ifndef CM_OPENGL_RENDERER
#define CM_OPENGL_RENDERER

#include "include/OGL_Program.hpp"
#include "include/OGL_Types.hpp"
#include "types/classes/cubemap.hpp"
#include <GL/glew.h>
#include <types/classes/camera.hpp>
#include <types/classes/light.hpp>
#include <types/classes/material.hpp>
#include <types/classes/mesh.hpp>
#include <types/classes/scene.hpp>

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
  std::vector<OGL_Cubemap> cubemaps;

  // for skyboxes
  uint32_t skyVertArr = 0;
  uint32_t skyVertBuffer = 0;
  uint32_t skyIndBuffer = 0;

public:
  // constructors
  OGL_Renderer();
  OGL_Renderer(OGL_RendererData data);

  // destructors
  ~OGL_Renderer();

  // getters

  // setters
  int register_mesh(Mesh& mesh);
  int register_meshes(std::vector<Mesh>& meshes);
  int register_material(Material& material);
  void setMaterialColor(int index, Vec4 color);
  int register_cubemap(Cubemap& skybox);

  // rendering
  void render(Scene& scene, OGL_Program& prg_texture,
              OGL_Program& prg_no_texture, Camera& camera);
  void render_skybox(int index, OGL_Program& prg, Camera& camera);
};

#endif
