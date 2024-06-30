#include "material.hpp"

extern "C" {
#define STB_IMAGE_IMPLEMENTATION
#include "../../osDependent/linux/filesystem/stb_image.h"
}

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

int Material::loadTexture(const char* path) {
  texData.data =
      stbi_load(path, &texData.width, &texData.height, &texData.channel, 0);

  if (texData.data == NULL) {
    texData.data = nullptr;
    return -1;
  }

  return 0;
}

void Material::resetTexture() {
  stbi_image_free(texData.data);
  texData.data = nullptr;
}

// getters
const float* Material::getDataAsArray() { return dataArray; }
Vec4& Material::getColor() { return (*(Vec4*)dataArray); }
float& Material::getMetalness() { return dataArray[4]; }
char Material::hasTexture() { return texData.data != nullptr; }
int Material::getTextureWidth() { return texData.width; }
int Material::getTextureHeight() { return texData.height; }
int Material::getTextureChannel() { return texData.channel; }
unsigned char* Material::getTextureData() { return texData.data; }