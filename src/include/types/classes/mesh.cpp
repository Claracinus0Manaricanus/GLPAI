#include "mesh.hpp"
#include <cassert>

// constructor
Mesh::Mesh() {}

Mesh::Mesh(std::vector<Vertex>& toInsert) { vertices = toInsert; }

Mesh::Mesh(std::vector<Vertex>& toInsert, std::vector<int>& toIndex) {
  vertices = toInsert;
  indexBuffer = toIndex;
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

// getters
Vertex Mesh::getVertex(int index) {
  assert(index < vertices.size());
  return vertices[index];
}

std::vector<Vertex>& Mesh::getAllVertices() { return vertices; }

std::vector<int>& Mesh::getIndexBuffer() { return indexBuffer; }