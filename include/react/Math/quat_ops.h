/**
 * @file
 * Contains the definition of mathematical operators for the re::quat class
 */
#ifndef RE_QUAT_OPS_H
#define RE_QUAT_OPS_H

#include "react/Math/quat.h"

using namespace re;

namespace re {
  
  /**
   * @ingroup maths
   * Calculates the squared length of the quaternion. It is considerably faster
   * than calculating the length.
   * 
   * @param q The input quaternion
   * @return The squared length of the quaternion
   */

  inline reFloat lengthSq(const quat& q) {
    return q.r*q.r + q.i*q.i + q.j*q.j + q.k*q.k;
  }
  
  /**
   * @ingroup maths
   * Calculates the length of the quaternion
   * 
   * @param q The input quaternion
   * @return The length of the quaternion
   */

  inline reFloat length(const quat& q) {
    return re::sqrt(lengthSq(q));
  }
  
  /**
   * @ingroup maths
   * Normalizes the given quaternion
   * 
   * @param q The quaternion to normalize
   * @return The normalized quaternion
   */

  inline const quat normalize(const quat& q) {
    return quat(q) /= length(q);
  }
  
  /**
   * Generates a random unit quaternion
   * 
   * @return A random unit quaternion
   */
  
  inline const quat quat::unit() {
    return re::normalize(quat::rand());
  }
  
  /**
   * @ingroup maths
   * Returns true if the two quaternions are equal within reasonable tolerance.
   * Difference is measured using the Euclidean distance metric.
   * 
   * @param a The first quaternion
   * @param b The second quaternion
   * @return True if the Euclidean distance between the quaternions is within
   * tolerance
   */
  
  inline bool similar(const quat& a, const quat& b) {
    return re::lengthSq(a - b) < re::sq(RE_FP_TOLERANCE);
  }
}

#endif
