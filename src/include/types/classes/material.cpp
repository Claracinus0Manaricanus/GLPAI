#include "material.hpp"
#include <cstdlib>

// constructors
Material::Material() {
  (*(Vec4*)dataArray) = {1, 1, 1, 1};
  dataArray[4] = 0;
}

Material::Material(MaterialData& data) {
  (*(Vec4*)dataArray) = data.color;
  dataArray[4] = data.metallic;
}

// destructors
Material::~Material() {}

// setters
void Material::setData(MaterialData& data) {
  (*(Vec4*)dataArray) = data.color;
  dataArray[4] = data.metallic;
}

void Material::setColor(Vec4 color) { (*(Vec4*)dataArray) = color; }
void Material::setMetalness(float metallic) { dataArray[4] = metallic; }

int Material::loadTexture(const char* path) {
  return image.loadImage({path, 0});
}

void Material::resetTexture() { image.clear(); }

// getters
const float* Material::getDataAsArray() { return dataArray; }
Vec4& Material::getColor() { return (*(Vec4*)dataArray); }
float& Material::getMetallic() { return dataArray[4]; }
char Material::hasTexture() { return image.getDataPointer() != nullptr; }
int Material::getTextureWidth() { return image.getWidth(); }
int Material::getTextureHeight() { return image.getHeight(); }
int Material::getTextureChannel() { return image.getChannels(); }
unsigned char* Material::getTextureData() { return image.getDataPointer(); }
Image& Material::getImageObject() { return image; }
