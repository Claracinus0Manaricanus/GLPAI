#ifndef CM_COLLISIONS
#define CM_COLLISIONS

#include "../types/classes/mesh.hpp"
#include "../types/classes/transform.hpp"
#include "../types/physics.hpp"
#include "../types/primitives.hpp"

namespace Physics {

int checkCollisionRaySurface(Ray& ray, Surface surface, RayHit* out);
int checkCollisionRayTriangle(Ray& ray, Triangle triangle, RayHit* out);
int checkCollisionRayMesh(Ray& ray, Mesh& mesh, Transform& transform,
                          RayHit* out);

} // namespace Physics

#endif
