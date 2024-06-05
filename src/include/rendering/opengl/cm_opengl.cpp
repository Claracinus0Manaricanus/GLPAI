#include "cm_opengl.hpp"

// constructors
OGL_Renderer::OGL_Renderer() {}

OGL_Renderer::OGL_Renderer(std::vector<uint32_t>& programs) {
  this->programs = programs;
}

// getters

// setters
int OGL_Renderer::addProgram(uint32_t& programID) {
  programs.push_back(programID);

  return 0;
}

// utility
OGL_Renderable OGL_Renderer::genRenderable(Mesh genFrom) {
  OGL_Renderable temp = {0, 0, 0};
  std::vector<Vertex> vertexData = genFrom.getAllVertices();
  std::vector<int> indexData = genFrom.getIndexBuffer();

  glGenBuffers(1, &temp.vertexDataBuffer);
  glGenBuffers(1, &temp.indexbuffer);
  glGenVertexArrays(1, &temp.vertexArray);

  // vertex data
  glBindVertexArray(temp.vertexArray);
  glBindBuffer(GL_ARRAY_BUFFER, temp.vertexDataBuffer);
  glBufferData(GL_VERTEX_ARRAY, vertexData.size() * sizeof(Vertex),
               vertexData.data(), GL_STATIC_DRAW);

  // index data
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, temp.indexbuffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof(int),
               indexData.data(), GL_STATIC_DRAW);

  // vertex data structure
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(6*sizeof(float)));
  glEnableVertexAttribArray(2);

  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(10*sizeof(float)));
  glEnableVertexAttribArray(3);

  glBindVertexArray(0);

  return temp;
}