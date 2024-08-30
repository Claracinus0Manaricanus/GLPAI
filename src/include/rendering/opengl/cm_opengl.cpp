#include "cm_opengl.hpp"
#include "include/OGL_Program.hpp"
#include "types/classes/cubemap.hpp"
#include <cstdint>
#include <vector>

// constructors
OGL_Renderer::OGL_Renderer() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);

  glDepthFunc(GL_LEQUAL);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

OGL_Renderer::OGL_Renderer(OGL_RendererData data) {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);

  glDepthFunc(GL_LEQUAL);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// destructors
OGL_Renderer::~OGL_Renderer() {
  for (OGL_Mesh mesh : meshes) {
    glDeleteBuffers(1, &mesh.indexbuffer);
    glDeleteBuffers(1, &mesh.vertexDataBuffer);
    glDeleteVertexArrays(1, &mesh.vertexArray);
  }

  for (OGL_Material material : materials) {
    if (material.texture.texID != 0)
      glDeleteTextures(1, &material.texture.texID);
  }

  for (OGL_Program* toFree : programs) {
    delete toFree;
  }
}

// getters
const char* OGL_Renderer::getProgramError(int index) {
  return programs[index]->getError();
}

// setters
int OGL_Renderer::register_mesh(Mesh& mesh) {
  OGL_Mesh tmpMesh;
  std::vector<Vertex> vertexData = mesh.getAllVertices();
  std::vector<uint32_t> indexData = mesh.getIndexBuffer();
  tmpMesh.indexBufferlength = indexData.size();

  glGenBuffers(1, &tmpMesh.vertexDataBuffer);
  glGenBuffers(1, &tmpMesh.indexbuffer);
  glGenVertexArrays(1, &tmpMesh.vertexArray);

  // vertex data
  glBindVertexArray(tmpMesh.vertexArray);
  glBindBuffer(GL_ARRAY_BUFFER, tmpMesh.vertexDataBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex),
               vertexData.data(), GL_STATIC_DRAW);

  // index data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpMesh.indexbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(int),
               indexData.data(), GL_STATIC_DRAW);

  // vertex data structure
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);

  meshes.push_back(tmpMesh);

  return (meshes.size() - 1);
}

int OGL_Renderer::register_meshes(std::vector<Mesh>& meshes) {
  int retVal = 0;

  for (Mesh mesh : meshes) {
    retVal = register_mesh(mesh);
  }

  return retVal;
}

int OGL_Renderer::register_material(Material& material) {
  OGL_Material tmpMat;

  if (material.hasTexture()) {
    glGenTextures(1, &tmpMat.texture.texID);
    tmpMat.texture.target = GL_TEXTURE_2D;
    tmpMat.texture.width = material.getTextureWidth();
    tmpMat.texture.height = material.getTextureHeight();

    if (material.getTextureChannel() == 3) {
      tmpMat.texture.format = GL_RGB;
    } else {
      tmpMat.texture.format = GL_RGBA;
    }

    glBindTexture(tmpMat.texture.target, tmpMat.texture.texID);
    glTexImage2D(tmpMat.texture.target, 0, tmpMat.texture.format,
                 tmpMat.texture.width, tmpMat.texture.height, 0,
                 tmpMat.texture.format, GL_UNSIGNED_BYTE,
                 material.getTextureData());

    glTexParameteri(tmpMat.texture.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(tmpMat.texture.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(tmpMat.texture.target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(tmpMat.texture.target, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(tmpMat.texture.target, 0);

  } else {
    tmpMat.texture.texID = 0;
    tmpMat.texture.target = 0;
    tmpMat.texture.format = 0;
    tmpMat.texture.width = 0;
    tmpMat.texture.height = 0;
  }

  tmpMat.color = material.getColor();
  tmpMat.metallic = material.getMetallic();
  tmpMat.prg_ID = material.getPrgID();

  materials.push_back(tmpMat);

  return (materials.size() - 1);
}

void OGL_Renderer::setMaterialColor(int index, Vec4 color) {
  materials[index].color = color;
}

int OGL_Renderer::register_cubemap(Cubemap& skybox) {
  OGL_Cubemap tmpMap;

  glGenTextures(1, &tmpMap.texID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, tmpMap.texID);

  for (int i = 0; i < 6; i++) {
    Image& side = skybox.getTexture(i);
    if (side.getDataPointer() != nullptr)
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                   side.getWidth(), side.getHeight(), 0, GL_RGB,
                   GL_UNSIGNED_BYTE, side.getDataPointer());
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

  cubemaps.push_back(tmpMap);

  return (cubemaps.size() - 1);
}

int OGL_Renderer::register_program(OGL_ProgramData data) {
  programs.push_back(new OGL_Program(data));

  return programs.size() - 1;
}

// rendering
void OGL_Renderer::render(Scene& scene, Camera& camera) {
  OGL_Program* activePrg = programs[0];

  // camera
  camera.calculateOVM();

  for (PointLight light : scene.getPointLights()) {
    for (GameObject obj : scene.getGameObjects()) {

      activePrg = programs[materials[obj.getMaterial()].prg_ID];
      activePrg->use();

      activePrg->setMat4("CVM", camera.getOVM());
      activePrg->setVec3("cameraPos", camera.getPosition());

      // object
      // OVM
      obj.calculateOVM();
      activePrg->setMat4("OVM", obj.getOVM());

      activePrg->setVec4("color", materials[obj.getMaterial()].color);
      activePrg->setFloat("metallic", materials[obj.getMaterial()].metallic);

      // light
      activePrg->setVec3("lPos", light.getPosition());
      activePrg->setVec3("lCol", light.getColor());
      activePrg->setFloat("lStrength", light.getStrength());

      // texture
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(materials[obj.getMaterial()].texture.target,
                    materials[obj.getMaterial()].texture.texID);
      activePrg->setUnsignedInt("tex", 0);

      // vertex array
      glBindVertexArray(meshes[obj.getMesh()].vertexArray);
      glDrawElements(GL_TRIANGLES, meshes[obj.getMesh()].indexBufferlength,
                     GL_UNSIGNED_INT, (void*)0);

      glBindTexture(materials[obj.getMaterial()].texture.target, 0);
    }
    glBlendFunc(GL_ONE, GL_ONE);
  }

  // blend settings
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBindVertexArray(0);
}

void OGL_Renderer::render_skybox(int index, int prg, Camera& camera) {
  if (skyVertArr == 0) {
    glGenVertexArrays(1, &skyVertArr);
    glGenBuffers(1, &skyVertBuffer);
    glGenBuffers(1, &skyIndBuffer);

    Box b1 = {{0, 0, 0}, {1, 1, 1}};
    Mesh bM(b1, 1);
    std::vector<Vertex> vertexData = bM.getAllVertices();
    std::vector<uint32_t> indexData = bM.getIndexBuffer();

    // vertex data
    glBindVertexArray(skyVertArr);
    glBindBuffer(GL_ARRAY_BUFFER, skyVertBuffer);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex),
                 vertexData.data(), GL_STATIC_DRAW);

    // index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyIndBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(int),
                 indexData.data(), GL_STATIC_DRAW);

    // vertex data structure
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
  }

  programs[prg]->use();

  camera.calculateOVM();
  programs[prg]->setMat4("CVM", camera.getOVM());

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemaps[index].texID);
  programs[prg]->setInt("cubemap", 0);

  glBindVertexArray(skyVertArr);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
