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
    material.setTexID(textures.size() - 1);
  }

  materials.push_back(new Material(material));
  materials[materials.size() - 1]->loadTexture(
      {material.getTextureData(), material.getTextureWidth(),
       material.getTextureHeight(), material.getTextureChannel()},
      0);

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

// rendering
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
