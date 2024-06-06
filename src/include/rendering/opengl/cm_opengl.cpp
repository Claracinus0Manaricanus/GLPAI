#include "cm_opengl.hpp"
#include <cstdio>
#include <vector>

const char* vertSrc = "#version 450 core\n"
                      "layout(location=0) in vec3 pos;\n"
                      "layout(location=2) in vec4 col;\n"
                      "out vec4 fragCol;\n"
                      "void main(){\n"
                      "fragCol = col;\n"
                      "gl_Position = vec4(pos.xyz, 1);\n"
                      "}\n";

const char* fragSrc = "#version 450 core\n"
                      "in vec4 fragCol;"
                      "out vec4 col;"
                      "void main(){\n"
                      "col = fragCol;\n"
                      "}";

// constructors
OGL_Renderer::OGL_Renderer() {
  // this part is for testing so it is temporary
  tempPRG = glCreateProgram();

  uint32_t vertSha = 0, fragSha = 0;
  vertSha = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertSha, 1, &vertSrc, NULL);
  glCompileShader(vertSha);

  fragSha = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragSha, 1, &fragSrc, NULL);
  glCompileShader(fragSha);

  glAttachShader(tempPRG, vertSha);
  glAttachShader(tempPRG, fragSha);
  glLinkProgram(tempPRG);

  glDeleteShader(vertSha);
  glDeleteShader(fragSha);
}

// getters

// setters

// rendering
void OGL_Renderer::renderOGL_Renderable(uint32_t programID,
                                        OGL_Renderable& toRender) {
  glUseProgram(tempPRG);
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