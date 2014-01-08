#include "react/Collision/Shapes/reSphere.h"

#include "react/Entities/reEnt.h"

reSphere::reSphere(reFloat radius) : reShape() {
  _shell = radius;
}

reSphere::reSphere(const reSphere& sphere) : reShape() {
  _shell = sphere.radius();
}

reSphere::~reSphere() {
  // do nothing
}

bool reSphere::intersectsRay(const reVector& origin, const reVector& dir, reVector* intersect, reVector* normal) const {
  // solve for the points of intersection
  const reFloat a = dir.lengthSq();
  const reFloat b = 2 * origin.dot(dir);
  const reFloat c = origin.lengthSq() - radius() * radius();
  const reFloat discriminant = b*b - 4*a*c;
  
  if (discriminant < RE_FP_TOLERANCE) {
    return false;
  }
  
  // consider only the smaller of the two solutions
  const reFloat solA = (-b - reSqrt(discriminant)) / (2.0 * a);
  
  // invalid solutions
  if (solA < RE_FP_TOLERANCE) {
    return false;
  }
  
  if (intersect != nullptr) {
    *intersect = origin + dir * solA;
  }
  
  if (normal != nullptr) {
    *normal = (origin + dir * solA).normalized();
  }
  
  return true;
}

