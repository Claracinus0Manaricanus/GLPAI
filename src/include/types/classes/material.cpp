#include "material.hpp"
#include "../../osDependent/linux/filesystem/stb_image.h"

// constructors
Material::Material() {
  (*(Vec4*)dataArray) = {1, 1, 1, 1};
  dataArray[4] = 0;
}

Material::Material(MaterialData& data) {
  (*(Vec4*)dataArray) = data.color;
  dataArray[4] = data.metalness;
}

// destructors
Material::~Material() {}

// setters
void Material::setData(MaterialData& data) {
  (*(Vec4*)dataArray) = data.color;
  dataArray[4] = data.metalness;
}

void Material::setColor(Vec4 color) { (*(Vec4*)dataArray) = color; }
void Material::setMetalness(float metalness) { dataArray[4] = metalness; }

// getters
const float* Material::getDataAsArray() { return dataArray; }
Vec4& Material::getColor() { return (*(Vec4*)dataArray); }
float& Material::getMetalness() { return dataArray[4]; }