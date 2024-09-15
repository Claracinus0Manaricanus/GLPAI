#include "material.hpp"
#include <cstdlib>

// constructors
Material::Material() {
  (*(Vec4*)dataArray) = {1, 1, 1, 1};
  dataArray[4] = 0;
  prg_ID = 0;
  tex_ID = -1;
}

Material::Material(MaterialData& data) {
  (*(Vec4*)dataArray) = data.color;
  dataArray[4] = data.metallic;
  prg_ID = data.prg_ID;
  tex_ID = data.tex_ID;
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

int Material::loadTexture(LoadedImageData data, int clearPrevious) {
  return image.loadImage(data, clearPrevious);
}

void Material::resetTexture() { image.clear(); }

void Material::setPrgID(int ID) { prg_ID = ID; }
void Material::setTexID(int ID) { tex_ID = ID; }
void Material::addUniformFloat(const char* name, float value) {
  names.push_back(name);
  values.push_back(value);
}

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
int Material::getPrgID() { return prg_ID; }
int& Material::getTexID() { return tex_ID; }
int Material::getUniformCount() { return names.size(); }
const char* Material::getUnfiromName(int index) { return names[index]; }
float Material::getUniformValue(int index) { return values[index]; }
