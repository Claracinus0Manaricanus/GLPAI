#include "cm_opengl.hpp"
#include <vector>

// constructors
OGL_Renderer::OGL_Renderer(OGL_RendererData data) {
  this->program = data.program;
  this->camera = data.camera;

  if (camera != NULL)
    program->setMat4("CVM", camera->getOVM());
}

// getters

// setters
void OGL_Renderer::setProgram(OGL_Program* program) { this->program = program; }

// rendering
void OGL_Renderer::render(OGL_Renderable& toRender) {
  program->use();

  // setting matrices
  camera->calculateOVM();
  program->setMat4("CVM", camera->getOVM());

  glBindVertexArray(toRender.vertexArray);
  glDrawElements(GL_TRIANGLES, toRender.indexBufferlength, GL_UNSIGNED_INT,
                 (void*)0);

  glBindVertexArray(0);
}

// utility
OGL_Renderable OGL_Renderer::genRenderable(Mesh genFrom) {
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

  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)(10 * sizeof(float)));
  glEnableVertexAttribArray(3);

  glBindVertexArray(0);

  return temp;
}