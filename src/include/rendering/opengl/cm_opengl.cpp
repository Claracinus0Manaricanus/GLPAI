#include "cm_opengl.hpp"
#include <vector>

// constructors
OGL_Renderer::OGL_Renderer() {}

// getters

// setters

// rendering
void OGL_Renderer::renderOGL_Renderable(OGL_Program& program,
                                        OGL_Renderable& toRender) {
  program.use();
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