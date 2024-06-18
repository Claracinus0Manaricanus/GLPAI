#ifndef CM_COLLISIONS
#define CM_COLLISIONS

#include "../types/classes/gameObject.hpp"
#include "../types/physics.hpp"
#include "../types/primitives.hpp"

namespace Physics {

int checkCollisionRaySurface(Ray& ray, Surface surface, RayHit* out);
int checkCollisionRayTriangle(Ray& ray, Triangle triangle, RayHit* out);
int checkCollisionRayGameObject(Ray& ray, GameObject& mesh, RayHit* out);

} // namespace Physics

#endif
