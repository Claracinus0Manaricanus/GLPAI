#ifndef CM_CAMERA
#define CM_CAMERA

#include "transform.hpp"

// data structures
struct CameraData {
  float fov;
};

class Camera : public Transform {
protected:
  float fov;

public:
  // constructor
  Camera(CameraData caData);
  Camera(CameraData caData, TransformData trData);


protected:
  //CVM generation
  void calculateOVM() override;
};

#endif