#ifndef CM_MESH
#define CM_MESH

#include "../../types/primitives.hpp"
#include <cassert>
#include <vector>

class Mesh {
private:
  // instance vars
  std::vector<Vertex> vertices;
  std::vector<int> indexBuffer;

public:
  // constructor
  Mesh();
  Mesh(std::vector<Vertex>& toInsert);
  Mesh(std::vector<Vertex>& toInsert, std::vector<int>& toIndex);

  // setters
  int addVertex(Vertex& toInsert);
  int addMultipleVertex(std::vector<Vertex>& toInsert);

  // getters
  Vertex getVertex(int index);
  std::vector<Vertex>& getAllVertices(); // unsafe and WIP
};

#endif