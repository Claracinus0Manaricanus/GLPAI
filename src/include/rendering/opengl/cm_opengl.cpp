#include "cm_opengl.hpp"
#include <vector>

#include "../../types/classes/gameObject.hpp"
#define STORAGE_CLASS GameObject*

// constructors
OGL_Renderer::OGL_Renderer() {}

OGL_Renderer::OGL_Renderer(OGL_RendererData data) {}

// getters

// setters

// rendering
void OGL_Renderer::render(OGL_Renderable& toRender, Camera& camera,
                          OGL_Program& program, PointLight& light) {
  program.use();

  // setting matrices
  // camera
  camera.calculateOVM();
  program.setMat4("CVM", camera.getOVM());

  // object
  if (toRender.dataStorage != NULL) {
    // OVM
    ((STORAGE_CLASS)toRender.dataStorage)->calculateOVM();
    program.setMat4("OVM", ((STORAGE_CLASS)toRender.dataStorage)->getOVM());

    program.setVec4("color", ((STORAGE_CLASS)toRender.dataStorage)->getColor());
    program.setFloat("metalness",
                     ((STORAGE_CLASS)toRender.dataStorage)->getMetalness());
  }

  // light
  program.setVec3("lPos", light.getPosition());
  program.setVec3("lCol", light.getColor());
  program.setFloat("lStrength", light.getStrength());

  // texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(toRender.texture.target, toRender.texture.texID);
  program.setUnsignedInt("tex", 0);

  // vertex array
  glBindVertexArray(toRender.vertexArray);
  glDrawElements(GL_TRIANGLES, toRender.indexBufferlength, GL_UNSIGNED_INT,
                 (void*)0);

  glBindTexture(toRender.texture.target, 0);
  glBindVertexArray(0);
}

// utility
OGL_Renderable OGL_Renderer::genRenderable(Mesh genFrom, void* dataStorage) {
  OGL_Renderable temp = {0, 0, 0};
  std::vector<Vertex> vertexData = genFrom.getAllVertices();
  std::vector<uint32_t> indexData = genFrom.getIndexBuffer();
  temp.indexBufferlength = indexData.size();

  glGenBuffers(1, &temp.vertexDataBuffer);
  glGenBuffers(1, &temp.indexbuffer);
  glGenVertexArrays(1, &temp.vertexArray);

  // vertex data
  glBindVertexArray(temp.vertexArray);
  glBindBuffer(GL_ARRAY_BUFFER, temp.vertexDataBuffer);
  glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex),
               vertexData.data(), GL_STATIC_DRAW);

  // index data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.indexbuffer);
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

  temp.dataStorage = dataStorage;

  // texture
  if (((GameObject*)dataStorage)->hasTexture()) {
    glGenTextures(1, &temp.texture.texID);
    temp.texture.target = GL_TEXTURE_2D;
    temp.texture.width = ((GameObject*)dataStorage)->getTextureWidth();
    temp.texture.height = ((GameObject*)dataStorage)->getTextureHeight();

    if (((GameObject*)dataStorage)->getTextureChannel() == 3) {
      temp.texture.format = GL_RGB;
    } else {
      temp.texture.format = GL_RGBA;
    }

    glBindTexture(temp.texture.target, temp.texture.texID);
    glTexImage2D(temp.texture.target, 0, temp.texture.format,
                 temp.texture.width, temp.texture.height, 0,
                 temp.texture.format, GL_UNSIGNED_BYTE,
                 ((GameObject*)dataStorage)->getTextureData());

    glTexParameteri(temp.texture.target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(temp.texture.target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(temp.texture.target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(temp.texture.target, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(temp.texture.target, 0);

  } else {
    temp.texture.texID = 0;
    temp.texture.target = 0;
    temp.texture.format = 0;
    temp.texture.width = 0;
    temp.texture.height = 0;
  }

  return temp;
}