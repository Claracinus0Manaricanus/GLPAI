#ifndef CM_MATERIAL
#define CM_MATERIAL

#include <types/classes/image.hpp>
#include <types/vectors.hpp>
#include <vector>

// definitions

struct MaterialData {
  Vec4 color = {1, 1, 1, 1};
  float metallic = 0;
  int prg_ID = 0;
  int tex_ID = -1;
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

  // shader program intended to be used with the material
  int prg_ID;
  // texture ID is assigned after this material is registered with the renderer
  // if texture ID is set pre registeration it uses the existing texture at that
  // ID even if the material has an image loaded
  int tex_ID;
  // data to be sent to the shader program
  std::vector<const char*> names;
  std::vector<float> values;

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
  int loadTexture(LoadedImageData data, int clearPrevious = 1);
  void resetTexture();
  void setPrgID(int ID);
  void setTexID(int ID);
  void addUniformFloat(const char* name, float value);

  // getters
  const float* getDataAsArray();
  Vec4& getColor();
  float& getMetallic();
  char hasTexture();
  int getTextureWidth();
  int getTextureHeight();
  int getTextureChannel();
  unsigned char* getTextureData();
  Image& getImageObject();
  int getPrgID();
  int& getTexID();
  int getUniformCount();
  const char* getUnfiromName(int index);
  float getUniformValue(int index);
};

#endif
