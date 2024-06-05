#ifndef CM_MESH
#define CM_MESH

#include "../../types/primitives.hpp"
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
  int getVertex(int index, Vertex& toStore);
  std::vector<Vertex>& getAllVertices();//unsafe and WIP
};

#endif