#ifndef CM_CAMERA
#define CM_CAMERA

#include <types/classes/transform.hpp>

// data structures
struct CameraData {
  TransformData trData = {{0, 0, 0}, {0, 0, 0}, {1, 1, 1}};
  float fov = 60;
  float aspectRatio = 1;
  float near = 0.1f;
  float far = 100.0f;
};

class Camera : public Transform {
protected:
  float fov; // half of field of view for optimization
  float aspectRatio;
  float near;
  float far;

public:
  // constructor
  Camera();
  Camera(CameraData caData);

  // setters
  void setAspectRatio(float aspectRatio);

public:
  // CVM generation
  void calculateOVM(Transform* relativeTo = nullptr) override;
  void calculateDirections() override;
};

#endif
