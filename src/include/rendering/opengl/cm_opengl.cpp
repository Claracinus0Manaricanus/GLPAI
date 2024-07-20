#include "cm_opengl.hpp"
#include <vector>

// constructors
OGL_Renderer::OGL_Renderer() {}

OGL_Renderer::OGL_Renderer(OGL_RendererData data) {}

// getters

// setters
void OGL_Renderer::register_mesh(Mesh& mesh) {
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
}

void OGL_Renderer::register_material(Material& material) {
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
    glTextureParameteri(tmpMat.texture.target, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(tmpMat.texture.target, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(tmpMat.texture.target, 0);

  } else {
    tmpMat.texture.texID = 0;
    tmpMat.texture.target = 0;
    tmpMat.texture.format = 0;
    tmpMat.texture.width = 0;
    tmpMat.texture.height = 0;
  }

  tmpMat.color = material.getColor();

  materials.push_back(tmpMat);
}

// rendering
void OGL_Renderer::render(int mesh, int material, OGL_Program& program,
                          Transform& transform, Camera& camera,
                          PointLight& light) {
  program.use();

  // setting matrices
  // camera
  camera.calculateOVM();
  program.setMat4("CVM", camera.getOVM());

  // object
  // OVM
  transform.calculateOVM();
  program.setMat4("OVM", transform.getOVM());

  program.setVec4("color", materials[material].color);

  // light
  program.setVec3("lPos", light.getPosition());
  program.setVec3("lCol", light.getColor());
  program.setFloat("lStrength", light.getStrength());

  // texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(materials[material].texture.target,
                materials[material].texture.texID);
  program.setUnsignedInt("tex", 0);

  // vertex array
  glBindVertexArray(meshes[mesh].vertexArray);
  glDrawElements(GL_TRIANGLES, meshes[mesh].indexBufferlength, GL_UNSIGNED_INT,
                 (void*)0);

  glBindTexture(materials[material].texture.target, 0);
  glBindVertexArray(0);
}

void OGL_Renderer::render(Scene& scene, OGL_Program& program, Camera& camera,
                          PointLight& light) {
  std::vector<GameObject>& objs = scene.getGameObjects();

  program.use();

  // setting matrices
  // camera
  camera.calculateOVM();
  program.setMat4("CVM", camera.getOVM());

  for (GameObject obj : objs) {
    // object
    // OVM
    obj.calculateOVM();
    program.setMat4("OVM", obj.getOVM());

    program.setVec4("color", materials[obj.getMaterial()].color);

    // light
    program.setVec3("lPos", light.getPosition());
    program.setVec3("lCol", light.getColor());
    program.setFloat("lStrength", light.getStrength());

    // texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(materials[obj.getMaterial()].texture.target,
                  materials[obj.getMaterial()].texture.texID);
    program.setUnsignedInt("tex", 0);

    // vertex array
    glBindVertexArray(meshes[obj.getMesh()].vertexArray);
    glDrawElements(GL_TRIANGLES, meshes[obj.getMesh()].indexBufferlength,
                   GL_UNSIGNED_INT, (void*)0);

    glBindTexture(materials[obj.getMaterial()].texture.target, 0);
  }

  glBindVertexArray(0);
}