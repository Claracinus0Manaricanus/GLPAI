#include "mesh.hpp"

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
int Mesh::getVertex(int index, Vertex& toStore) {
  if (index < vertices.size()) {
    toStore = vertices[index];
    return 0;
  }

  return -1;
}

std::vector<Vertex>& Mesh::getAllVertices() { return vertices; }