#include "mesh.hpp"
#include <cassert>
#include <cm_math/operations.hpp>
#include <math.h>

#define PI 3.14159265f
#define TAU (PI * 2)

// constructor
Mesh::Mesh() {}

Mesh::Mesh(MeshData& data) {
  this->vertices = data.vertices;
  this->indexBuffer = data.indexBuffer;
}

Mesh::Mesh(Box data, int invert) {
  for (int i = -1; i < 1; i++) {
    for (int j = -1; j < 1; j++) {
      for (int k = -1; k < 1; k++) {
        Vertex tempvert;
        tempvert.position =
            data.position +
            (data.dimensions * (Vec3){(float)(1 + 2 * i), (float)(1 + 2 * j),
                                      (float)(1 + 2 * k)});
        tempvert.normal = {0, 0, 0};
        tempvert.uv = {1.0f + k, -1.0f - j};

        vertices.push_back(tempvert);
      }
    }
  }
  if (invert) {
    // X
    addFace(0, 2, 1);
    addFace(1, 2, 3);
    addFace(4, 5, 6);
    addFace(6, 5, 7);

    // Y
    addFace(4, 0, 1);
    addFace(4, 1, 5);
    addFace(2, 6, 3);
    addFace(3, 6, 7);

    // Z
    addFace(0, 4, 2);
    addFace(2, 4, 6);
    addFace(5, 1, 3);
    addFace(5, 3, 7);
  } else {
    // X
    addFace(0, 1, 2);
    addFace(1, 3, 2);
    addFace(4, 6, 5);
    addFace(6, 7, 5);

    // Y
    addFace(4, 1, 0);
    addFace(4, 5, 1);
    addFace(2, 3, 6);
    addFace(3, 7, 6);

    // Z
    addFace(0, 2, 4);
    addFace(2, 6, 4);
    addFace(5, 3, 1);
    addFace(5, 7, 3);
  }
}

Mesh::Mesh(Sphere data, int resolution, int invert) {
  // UV sphere algorithm (by me so don't expect much) (it works though)
  Vertex tempVert;
  for (int i = -resolution; i <= resolution; i += 1) {
    for (int k = 0; k <= resolution; k += 1) {
      float radian = k * TAU / resolution;
      float y = (i * data.radius / resolution);

      float x = cos(radian), z = sin(radian);

      if (i != -resolution && i != resolution) {
        float division_const =
            sqrt(((x * x) + (z * z)) / ((data.radius * data.radius) - (y * y)));
        x /= division_const;
        z /= division_const;
      } else {
        x = 0;
        z = 0;
      }

      tempVert.position = {x, y, z};
      if (invert)
        tempVert.uv = {(float)k / resolution,
                       -(y + data.radius) / (2.0f * data.radius)};
      else
        tempVert.uv = {1.0f - k / resolution,
                       -(y + data.radius) / (2.0f * data.radius)};

      vertices.push_back(tempVert);
    }
  }

  for (int i = 0; i < vertices.size() - (resolution + 1);
       i += (resolution + 1)) {
    for (int k = 0; k < resolution; k++) {
      if (invert) {
        addFace(i + k, i + k + (resolution + 2), i + k + (resolution + 1));
        addFace(i + k, i + k + 1, i + k + (resolution + 2));
      } else {
        addFace(i + k, i + k + (resolution + 1), i + k + (resolution + 2));
        addFace(i + k, i + k + (resolution + 2), i + k + 1);
      }
    }
  }
}

// setters
int Mesh::addVertex(Vertex& toInsert) {
  vertices.push_back(toInsert);

  return 0; // needs check
}

int Mesh::addMultipleVertex(std::vector<Vertex>& toInsert) {
  vertices.insert(vertices.cend(), toInsert.cbegin(), toInsert.cend());

  return 0; // needs check
}

int Mesh::addFace(uint32_t index1, uint32_t index2, uint32_t index3) {
  if (index1 >= vertices.size() && index2 >= vertices.size() &&
      index3 >= vertices.size())
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

// utility
int Mesh::calculateNormals() {

  Vertex v1, v2, v3;
  Vec3 normal;

  std::vector<Vertex> newVertices;
  std::vector<uint32_t> newIndexBuffer;

  for (int i = 0; i < indexBuffer.size(); i += 3) {
    v1 = vertices[indexBuffer[i]];
    v2 = vertices[indexBuffer[i + 1]];
    v3 = vertices[indexBuffer[i + 2]];

    normal = Vector::Normalize(
        Vector::Cross(v2.position - v1.position, v3.position - v1.position));

    v1.normal = normal;
    v2.normal = normal;
    v3.normal = normal;

    newVertices.push_back(v1);
    newVertices.push_back(v2);
    newVertices.push_back(v3);
    newIndexBuffer.push_back(i);
    newIndexBuffer.push_back(i + 1);
    newIndexBuffer.push_back(i + 2);
  }

  vertices = newVertices;
  indexBuffer = newIndexBuffer;

  return 0;
}
