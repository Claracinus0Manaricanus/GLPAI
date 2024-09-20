#ifndef CM_LIGHT
#define CM_LIGHT

#include <types/vectors.hpp>

struct PointLightData {
  Vec3 position;
  Vec3 color;
  float strength;
};

class PointLight {
protected:
  Vec3 position;
  Vec3 color;
  float strength;

public:
  // cosntructors
  PointLight();
  PointLight(PointLightData data);

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

struct DirectLightData {
  Vec3 direction; // direction to light for example 1,1,1 means the light is
                  // towards the direction 1,1,1
  Vec3 color;
  float strength;
};

class DirectLight {
protected:
  Vec3 direction; // direction to light
  Vec3 color;
  float strength;

public:
  // constructors
  DirectLight();
  DirectLight(DirectLightData data);

  // setters
  void setDirection(Vec3 Direction);
  void setColor(Vec3 color);
  void setStrength(float strength);

  // getters
  Vec3& getDirection();
  Vec3& getColor();
  float getStrength();
};

#endif
