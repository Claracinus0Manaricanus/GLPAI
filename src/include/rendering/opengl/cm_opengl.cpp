#include "cm_opengl.hpp"
#include "include/OGL_Program.hpp"
#include "types/classes/cubemap.hpp"
#include <GL/glew.h>
#include <cstdint>
#include <vector>

// init functions
void OGL_Renderer::init_defaultMeshes() {
  glGenVertexArrays(1, &default_meshes[0].vertexArray);
  glGenBuffers(1, &default_meshes[0].vertexDataBuffer);
  glGenBuffers(1, &default_meshes[0].indexbuffer);

  Box b1 = {{0, 0, 0}, {1, 1, 1}};
  Mesh bM(b1, 1);
  std::vector<Vertex> vertexData = bM.getAllVertices();
  std::vector<uint32_t> indexData = bM.getIndexBuffer();

  // vertex data
  glBindVertexArray(default_meshes[0].vertexArray);
  glBindBuffer(GL_ARRAY_BUFFER, default_meshes[0].vertexDataBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex),
               vertexData.data(), GL_STATIC_DRAW);

  // index data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, default_meshes[0].indexbuffer);
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

  glGenVertexArrays(1, &default_meshes[1].vertexArray);
  glGenBuffers(1, &default_meshes[1].vertexDataBuffer);
  glGenBuffers(1, &default_meshes[1].indexbuffer);

  Mesh plane;
  Vertex tmpVertex = {{-1, -1, 0}, {0, 0, 0}, {0, 0}};
  plane.addVertex(tmpVertex);
  tmpVertex = {{-1, 1, 0}, {0, 0, 0}, {0, 1}};
  plane.addVertex(tmpVertex);
  tmpVertex = {{1, 1, 0}, {0, 0, 0}, {1, 1}};
  plane.addVertex(tmpVertex);
  tmpVertex = {{1, -1, 0}, {0, 0, 0}, {1, 0}};
  plane.addVertex(tmpVertex);

  plane.addFace(0, 2, 1);
  plane.addFace(0, 3, 2);

  vertexData = plane.getAllVertices();
  indexData = plane.getIndexBuffer();

  // vertex data
  glBindVertexArray(default_meshes[1].vertexArray);
  glBindBuffer(GL_ARRAY_BUFFER, default_meshes[1].vertexDataBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex),
               vertexData.data(), GL_STATIC_DRAW);

  // index data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, default_meshes[1].indexbuffer);
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

// constructors
OGL_Renderer::OGL_Renderer() {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);

  glDepthFunc(GL_LEQUAL);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  init_defaultMeshes();
}

OGL_Renderer::OGL_Renderer(OGL_RendererData data) {
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_BLEND);

  glDepthFunc(GL_LEQUAL);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  init_defaultMeshes();
}

// destructors
OGL_Renderer::~OGL_Renderer() {
  for (OGL_Mesh mesh : meshes) {
    glDeleteBuffers(1, &mesh.indexbuffer);
    glDeleteBuffers(1, &mesh.vertexDataBuffer);
    glDeleteVertexArrays(1, &mesh.vertexArray);
  }

  for (Material* material : materials) {
    if (material->getTexID() != 0)
      glDeleteTextures(1, (unsigned int*)&material->getTexID());

    delete material;
  }

  for (OGL_Program* toFree : programs) {
    delete toFree;
  }
}

// getters
const char* OGL_Renderer::getProgramError(int index) {
  return programs[index]->getError();
}

Material& OGL_Renderer::getMaterial(int index) { return *materials[index]; }

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

  OGL_Texture tmpTex;

  if (material.hasTexture() && material.getTexID() == -1) {
    glGenTextures(1, &tmpTex.texID);
    tmpTex.target = GL_TEXTURE_2D;
    tmpTex.width = material.getTextureWidth();
    tmpTex.height = material.getTextureHeight();

    if (material.getTextureChannel() == 3) {
      tmpTex.format = GL_RGB;
    } else {
      tmpTex.format = GL_RGBA;
    }

    glBindTexture(tmpTex.target, tmpTex.texID);
    glTexImage2D(tmpTex.target, 0, tmpTex.format, tmpTex.width, tmpTex.height,
                 0, tmpTex.format, GL_UNSIGNED_BYTE, material.getTextureData());

    glTexParameteri(tmpTex.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(tmpTex.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(tmpTex.target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(tmpTex.target, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(tmpTex.target, 0);

    textures.push_back(tmpTex);
  }

  materials.push_back(new Material(material));
  materials[materials.size() - 1]->loadTexture(
      {material.getTextureData(), material.getTextureWidth(),
       material.getTextureHeight(), material.getTextureChannel()},
      0);
  materials[materials.size() - 1]->setTexID(textures.size() - 1);

  return (materials.size() - 1);
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

int OGL_Renderer::create_framebuffer(int width, int height, int type) {
  OGL_Framebuffer tmpFramebuffer;
  glCreateFramebuffers(1, &tmpFramebuffer.ID);
  glBindFramebuffer(GL_FRAMEBUFFER, tmpFramebuffer.ID);

  // color buffer
  glGenTextures(1, &tmpFramebuffer.textures[0].texID);
  glBindTexture(GL_TEXTURE_2D, tmpFramebuffer.textures[0].texID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  tmpFramebuffer.textures[0].width = width;
  tmpFramebuffer.textures[0].height = height;
  tmpFramebuffer.textures[0].format = GL_RGBA;
  tmpFramebuffer.textures[0].target = GL_TEXTURE_2D;

  // depth buffer
  glGenTextures(1, &tmpFramebuffer.textures[1].texID);
  glBindTexture(GL_TEXTURE_2D, tmpFramebuffer.textures[1].texID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0,
               GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  tmpFramebuffer.textures[1].width = width;
  tmpFramebuffer.textures[1].height = height;
  tmpFramebuffer.textures[1].format = GL_DEPTH_COMPONENT;
  tmpFramebuffer.textures[1].target = GL_TEXTURE_2D;

  glBindTexture(GL_TEXTURE_2D, 0);

  // framebuffer
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         tmpFramebuffer.textures[0].texID, 0);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                         tmpFramebuffer.textures[1].texID, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    glDeleteTextures(1, &tmpFramebuffer.textures[0].texID);
    glDeleteTextures(1, &tmpFramebuffer.textures[0].texID);
    glDeleteFramebuffers(1, &tmpFramebuffer.ID);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return -1;
  }

  framebuffers.push_back(tmpFramebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return framebuffers.size() - 1;
}

// rendering
void OGL_Renderer::useFramebuffer(int index) {
  glViewport(0, 0, framebuffers[index].textures[0].width,
             framebuffers[index].textures[0].height);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffers[index].ID);
}

// takes Screen Resolution from caller
void OGL_Renderer::renderFramebuffer(int index, IVec2 screenRes) {
  glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffers[index].ID);
  glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

  glBlitFramebuffer(0, 0, framebuffers[index].textures[0].width,
                    framebuffers[index].textures[0].height, 0, 0, screenRes.x,
                    screenRes.y, GL_COLOR_BUFFER_BIT, GL_LINEAR);
}

void OGL_Renderer::render(Scene& scene, Camera& camera, int fullbright) {
  OGL_Program* activePrg = programs[0];

  // camera
  camera.calculateOVM();

  for (PointLight light : scene.getPointLights()) {
    for (GameObject obj : scene.getGameObjects()) {

      activePrg = programs[materials[obj.getMaterial()]->getPrgID()];
      activePrg->use();

      activePrg->setMat4("CVM", camera.getOVM());
      activePrg->setVec3("cameraPos", camera.getPosition());

      // object
      // OVM
      obj.calculateOVM();
      activePrg->setMat4("OVM", obj.getOVM());

      // material
      activePrg->setVec4("color", materials[obj.getMaterial()]->getColor());
      activePrg->setFloat("metallic",
                          materials[obj.getMaterial()]->getMetallic());
      for (int i = 0; i < materials[obj.getMaterial()]->getUniformCount();
           i++) {
        activePrg->setFloat(materials[obj.getMaterial()]->getUnfiromName(i),
                            materials[obj.getMaterial()]->getUniformValue(i));
      }

      // light
      activePrg->setVec3("lPos", light.getPosition());
      activePrg->setVec3("lCol", light.getColor());
      activePrg->setFloat("lStrength", light.getStrength());

      // texture
      if (materials[obj.getMaterial()]->getTexID() >= 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(textures[materials[obj.getMaterial()]->getTexID()].target,
                      textures[materials[obj.getMaterial()]->getTexID()].texID);
        activePrg->setInt("tex", 0);
      }

      // vertex array
      glBindVertexArray(meshes[obj.getMesh()].vertexArray);
      glDrawElements(GL_TRIANGLES, meshes[obj.getMesh()].indexBufferlength,
                     GL_UNSIGNED_INT, (void*)0);
    }
    if (fullbright)
      break;
    glBlendFunc(GL_ONE, GL_ONE);
  }

  // blend settings
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glBindVertexArray(0);
}

void OGL_Renderer::render_skybox(int index, int prg, Camera& camera) {
  programs[prg]->use();

  camera.calculateOVM();
  programs[prg]->setMat4("CVM", camera.getOVM());

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, cubemaps[index].texID);
  programs[prg]->setInt("cubemap", 0);

  glBindVertexArray(default_meshes[0].vertexArray);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
