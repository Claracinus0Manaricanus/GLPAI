#ifndef CM_CAMERA
#define CM_CAMERA

#include "transform.hpp"

// data structures
struct CameraData {
  TransformData trData = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
  float fov = 60;
  float aspectRatio = 1;
};

class Camera : public Transform {
protected:
  float fov; // half of field of view for optimization
  float aspectRatio;

public:
  // constructor
  Camera();
  Camera(CameraData caData);

  // setters
  void setAspectRatio(float aspectRatio);

public:
  // CVM generation
  void calculateOVM() override;
  void calculateDirections() override;
};

#endif