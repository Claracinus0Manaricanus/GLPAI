#include "mesh.hpp"
#include <cassert>

// constructor
Mesh::Mesh() {}

Mesh::Mesh(MeshData& data) {
  this->vertices = data.vertices;
  this->indexBuffer = data.indexBuffer;
}

// setters
int Mesh::addVertex(Vertex& toInsert) {
  vertices.insert(vertices.cend(), toInsert);

  return 0; // needs check
}

int Mesh::addMultipleVertex(std::vector<Vertex>& toInsert) {
  vertices.insert(vertices.cend(), toInsert.cbegin(), toInsert.cend());

  return 0; // needs check
}

int Mesh::addFace(uint32_t index1, uint32_t index2, uint32_t index3) {
  if (index1 < vertices.size() && index2 < vertices.size() &&
      index3 < vertices.size())
    return -1;

  indexBuffer.push_back(index1);
  indexBuffer.push_back(index2);
  indexBuffer.push_back(index3);

  return 0;
}

void Mesh::addTriangle(Triangle& toAdd) {
  uint32_t tempSize = vertices.size();

  vertices.push_back(toAdd.vertices[0]);
  vertices.push_back(toAdd.vertices[1]);
  vertices.push_back(toAdd.vertices[2]);

  indexBuffer.push_back(tempSize);
  indexBuffer.push_back(tempSize + 1);
  indexBuffer.push_back(tempSize + 2);
}

// getters
Vertex Mesh::getVertex(int index) {
  assert(index < vertices.size());
  return vertices[index];
}

std::vector<Vertex>& Mesh::getAllVertices() { return vertices; }

std::vector<uint32_t>& Mesh::getIndexBuffer() { return indexBuffer; }