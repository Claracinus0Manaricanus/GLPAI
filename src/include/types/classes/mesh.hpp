#ifndef CM_MESH
#define CM_MESH

#include "../primitives.hpp"
#include <vector>
#include <cstdint>

class Mesh {
protected:
  // instance vars
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indexBuffer;

public:
  // constructor
  Mesh();
  Mesh(std::vector<Vertex>& toInsert);
  Mesh(std::vector<Vertex>& toInsert, std::vector<uint32_t>& toIndex);

  // setters
  int addVertex(Vertex& toInsert);
  int addMultipleVertex(std::vector<Vertex>& toInsert);

  int addFace(uint32_t index1, uint32_t index2, uint32_t index3);
  void addTriangle(Triangle& toAdd);

  // getters
  Vertex getVertex(int index);
  std::vector<Vertex>& getAllVertices(); // unsafe and WIP
  std::vector<uint32_t>& getIndexBuffer();
  //add get*Size() functions
};

#endif