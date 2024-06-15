#ifndef CM_CAMERA
#define CM_CAMERA

#include "transform.hpp"

// data structures
struct CameraData {
  TransformData trData={{0,0,0}, {0,0,0}, {0,0,0}};
  float fov = 60;
};

class Camera : public Transform {
protected:
  float fov;

public:
  // constructor
  Camera();
  Camera(CameraData caData);


protected:
  //CVM generation
  void calculateOVM() override;
};

#endif