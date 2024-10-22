#ifndef CM_COLLISIONS
#define CM_COLLISIONS

#include "../types/classes/mesh.hpp"
#include "../types/physics.hpp"
#include "../types/primitives.hpp"

namespace Physics {

// Ray collisions
int checkCollisionRaySurface(Ray& ray, Surface surface, RayHit* out);
int checkCollisionRayTriangle(Ray& ray, Triangle triangle, RayHit* out);
int checkCollisionRayMesh(Ray& ray, Mesh& mesh, Mat4& OVM, RayHit* out);
/**
 * returns normals in RayHit* out according to the surface of the sphere
 */
int checkCollisionRaySphere(Ray& ray, Sphere sphere, RayHit* out);

// sphere collisions
/**
 * return normals according to sphere0
 */
int checkCollisionSphereSphere(Sphere& sphere0, Sphere& sphere1,
                               SphereHit* out);

} // namespace Physics

#endif
