#ifndef CM_MESH
#define CM_MESH

#include <cstdint>
#include <types/primitives.hpp>
#include <vector>

struct MeshData {
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indexBuffer;
};

class Mesh {
protected:
  // instance vars
  std::vector<Vertex> vertices;
  std::vector<uint32_t> indexBuffer;

public:
  // constructor
  Mesh();
  Mesh(const Mesh& mesh);
  Mesh(MeshData& data);
  Mesh(Box data, int invert = 0);
  Mesh(Sphere data, int resolution = 10, int invert = 0);

  // operators
  Mesh& operator=(const Mesh& mesh);

  // setters
  int addVertex(Vertex& toInsert);
  int addMultipleVertex(std::vector<Vertex>& toInsert);

  int addFace(uint32_t index1, uint32_t index2, uint32_t index3);
  void addTriangle(Triangle& toAdd);

  // getters
  Vertex getVertex(int index);
  std::vector<Vertex>& getAllVertices(); // unsafe and WIP
  std::vector<uint32_t>& getIndexBuffer();
  // add get*Size() functions

  // utility
  int calculateNormals(); // calculates normals based on faces and vertex
                          // positions, assumes triangles
};

#endif
