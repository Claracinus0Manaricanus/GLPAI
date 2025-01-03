#ifndef CM_TRANSFORM
#define CM_TRANSFORM

#include <types/vectors.hpp>

// data structs
struct TransformData {
  Vec3 position = {0, 0, 0};
  Vec3 rotation = {0, 0, 0};
  Vec3 scale = {1, 1, 1};
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
  Mat4 rotMat;

public:
  // constructors
  Transform();
  Transform(TransformData data);

  // destructors
  ~Transform();

  // setters
  void setPosition(Vec3 position);
  void move(Vec3 movement);
  void localMove(Vec3 movement);
  void setRotation(Vec3 rotation);
  void rotate(Vec3 rotation);
  void setScale(Vec3 scale);

  // getters
  Vec3& getPosition();
  Vec3& getRotation();
  Vec3& getScale();

  Vec3& getForward();
  Vec3& getRight();
  Vec3& getUp();

  Mat4& getOVM();
  Mat4& getRotMat();

public:
  // utility

  /* calculates forward, right and up vectors
   * for the transform. Uses quaternions to rotate
   * around x, y and z axis (i, j, k)
   * generates a rotation matrix named rotMat according to
   * the transforms direction vectors.
   */
  virtual void calculateDirections();

  /* generates Object View Matrix (OVM) using
   * matrix multiplication.
   */
  virtual void calculateOVM(Mat4* relativeTo = nullptr);
};

#endif
