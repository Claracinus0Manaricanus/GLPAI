#ifndef CM_LIGHT
#define CM_LIGHT

#include "../vectors.hpp"

struct PointLightData {
  Vec3 position;
  Vec3 color;
  float strength;
};

class PointLight {
private:
  Vec3 position;
  Vec3 color;
  float strength;

public:
  // cosntructors
  PointLight();
  PointLight(PointLightData& data);

  // setters
  void move(Vec3 movement);
  void setPosition(Vec3 position);
  void setColor(Vec3 color);
  void setStrength(float strength);

  // getters
  Vec3& getPosition();
  Vec3& getColor();
  float getStrength();
};

#endif