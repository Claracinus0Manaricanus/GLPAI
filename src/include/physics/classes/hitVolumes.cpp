#include "hitVolumes.hpp"
#include "physics/collisions.hpp"

//    HitVolume class    //

// constructors
HitVolume::HitVolume() {}

// getters

// setters

// collision detection
int HitVolume::checkCollisionWithRay(Ray& ray, RayHit* out, Mat4& OVM) {
  return -1;
}

int HitVolume::checkCollisionWithSphere(Sphere& sphere, SphereHit* out,
                                        Mat4& OVM) {
  return -1;
}

//    HitSphere class    //

// constructors
HitSphere::HitSphere() {
  hitSphere.position = {0, 0, 0};
  hitSphere.radius = 1;
}

HitSphere::HitSphere(Sphere initSphere) { hitSphere = initSphere; }

// getters
Sphere HitSphere::getSphere() { return hitSphere; }

// setters
void HitSphere::setSpherePosition(Vec3 position) {
  hitSphere.position = position;
}

// collision detection
int HitSphere::checkCollisionWithRay(Ray& ray, RayHit* out, Mat4& OVM) {
  Sphere processedSphere = {{0, 0, 0}, hitSphere.radius};

  Vec4 newPos = OVM * (Vec4){hitSphere.position.x, hitSphere.position.y,
                             hitSphere.position.z, 1};
  processedSphere.position = (Vec3){newPos.x, newPos.y, newPos.z};

  return Physics::checkCollisionRaySphere(ray, processedSphere, out);
}

int HitSphere::checkCollisionWithSphere(Sphere& sphere, SphereHit* out,
                                        Mat4& OVM) {
  Sphere processedSphere = {{0, 0, 0}, hitSphere.radius};

  Vec4 newPos = OVM * (Vec4){hitSphere.position.x, hitSphere.position.y,
                             hitSphere.position.z, 1};
  processedSphere.position = (Vec3){newPos.x, newPos.y, newPos.z};

  return Physics::checkCollisionSphereSphere(sphere, processedSphere, out);
}

//    HitMesh class    //

// constructors
HitMesh::HitMesh() {};
HitMesh::HitMesh(Mesh& initMesh) { hitMesh = Mesh(initMesh); }

// getters
Mesh& HitMesh::getMesh() { return hitMesh; }

// setters
void HitMesh::setMesh(Mesh& mesh) { hitMesh = Mesh(mesh); }

// collision detection
int HitMesh::checkCollisionWithRay(Ray& ray, RayHit* out, Mat4& OVM) {
  return Physics::checkCollisionRayMesh(ray, hitMesh, OVM, out);
}

int HitMesh::checkCollisionWithSphere(Sphere& sphere, SphereHit* out,
                                      Mat4& OVM) {
  return -1;
}
