#ifndef CM_MATERIAL
#define CM_MATERIAL

#include "../vectors.hpp"

struct MaterialData {
  Vec4 color = {1, 1, 1, 1};
  float metalness = 0;
};

class Material {
protected:
  // holds data in an array
  // format: CCCCM or RGBAM
  // RGBA is the color format, CCCC means color, M is metalness
  // if the data held changes with an update the
  // new data types will be added after the existing ones
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

  // getters
  const float* getDataAsArray();
  Vec4& getColor();
  float& getMetalness();
};

#endif