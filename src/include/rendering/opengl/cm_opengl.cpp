#include "cm_opengl.hpp"
#include <vector>

#include "../../types/classes/gameObject.hpp"
#define STORAGE_CLASS GameObject*

// constructors
OGL_Renderer::OGL_Renderer(OGL_RendererData data) {
  this->program = data.program;
  this->camera = data.camera;
}

// getters

// setters
void OGL_Renderer::setProgram(OGL_Program* program) { this->program = program; }

// rendering
void OGL_Renderer::render(OGL_Renderable& toRender) {
  program->use();

  // setting matrices
  if (camera != NULL) {
    camera->calculateOVM();
    program->setMat4("CVM", camera->getOVM());
  }
  if (toRender.dataStorage != NULL) {
    // OVM
    ((STORAGE_CLASS)toRender.dataStorage)->calculateOVM();
    program->setMat4("OVM", ((STORAGE_CLASS)toRender.dataStorage)->getOVM());

    program->setVec4("color",
                     ((STORAGE_CLASS)toRender.dataStorage)->getColor());
    program->setFloat("metalness",
                      ((STORAGE_CLASS)toRender.dataStorage)->getMetalness());
  }

  glBindVertexArray(toRender.vertexArray);
  glDrawElements(GL_TRIANGLES, toRender.indexBufferlength, GL_UNSIGNED_INT,
                 (void*)0);

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

  return temp;
}