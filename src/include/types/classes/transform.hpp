#ifndef CM_TRANSFORM
#define CM_TRANSFORM

#include "../vectors.hpp"

// data structs
struct TransformData {
  Vec3 position;
  Vec3 rotation; // ? might change
  Vec3 scale;
};

// OVM function is to be overridden by the extending
// class to accommodate for View Matrix calculation differences
// There is a default calculation installed however for rotation and
// translation

class Transform {
protected:
  Vec3 position;
  Vec3 rotation;
  Vec3 scale;
  // orientation
  Vec3 forward;
  Vec3 right;
  Vec3 up;

  Mat4 OVM;

public:
  // constructors
  Transform();
  Transform(TransformData data);

  // setters
  void setPosition(Vec3 position);
  void move(Vec3 movement);
  void setRotation(Vec3 rotation);
  void rotate(Vec3 rotation);

  // getters
  Vec3& getPosition();
  Vec3& getRotation();
  Vec3& getScale();

  Vec3& getForward();
  Vec3& getRight();
  Vec3& getUp();

  Mat4& getOVM();

protected:
  // utility
  virtual void calculateOVM();
};

#endif