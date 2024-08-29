#ifndef CM_MATERIAL
#define CM_MATERIAL

#include <types/classes/image.hpp>
#include <types/vectors.hpp>

struct MaterialData {
  Vec4 color = {1, 1, 1, 1};
  float metallic = 0;
};

class Material {
protected:
  // holds data in an array
  // format: CCCCM or RGBAM
  // RGBA is the color format, CCCC means color, M is metalness
  // if the data held changes with an update the
  // new data types will be added after the existing ones
  Image image;
  float dataArray[5];

public:
  // constructors
  Material();
  Material(MaterialData& data);

  // destructors
  ~Material();

  // setters
  void setData(MaterialData& data);
  void setColor(Vec4 color);
  void setMetalness(float metalness);
  int loadTexture(const char* path);
  void resetTexture();

  // getters
  const float* getDataAsArray();
  Vec4& getColor();
  float& getMetallic();
  char hasTexture();
  int getTextureWidth();
  int getTextureHeight();
  int getTextureChannel();
  unsigned char* getTextureData();
};

#endif
