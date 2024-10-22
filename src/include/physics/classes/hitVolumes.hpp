#ifndef CM_HIT_VOLUMES
#define CM_HIT_VOLUMES

#include <physics/collisions.hpp>

/**
 * HitVolume class is the base class for other hitvolumes, it doesn't implement
 * anything but gives a baseline for other classes to use with a few failsafes
 * for missing implementations
 * */
class HitVolume {
public:
  // constructors
  HitVolume();

  // getters

  // setters

  // collision detection
  /**
   * OVM is for the position and orientation of the hitVolume in space
   */
  virtual int checkCollisionWithRay(Ray& ray, RayHit* out, Mat4& OVM);
  virtual int checkCollisionWithSphere(Sphere& sphere, SphereHit* out,
                                       Mat4& OVM);
};

class HitSphere : public HitVolume {
private:
  /**
   * its position value is relative to origin so the position also gets
   * processed with OVM
   */
  Sphere hitSphere;

public:
  // constructors
  HitSphere();
  HitSphere(Sphere initSphere);

  // getters
  Sphere getSphere();

  // setters
  void setSpherePosition(Vec3 position);

  // collision detection
  int checkCollisionWithRay(Ray& ray, RayHit* out, Mat4& OVM) override;
  int checkCollisionWithSphere(Sphere& sphere, SphereHit* out,
                               Mat4& OVM) override;
};

class HitMesh : public HitVolume {
private:
  Mesh hitMesh;

public:
  // constructors
  HitMesh();
  HitMesh(Mesh& initMesh);

  // getters
  Mesh& getMesh();

  // setters
  void setMesh(Mesh& mesh);

  // collision detection
  int checkCollisionWithRay(Ray& ray, RayHit* out, Mat4& OVM) override;

  /**
   * not implemented yet
   */
  int checkCollisionWithSphere(Sphere& sphere, SphereHit* out,
                               Mat4& OVM) override;
};

#endif
