/**
 * @file
 * Contains various definitions for spatial query structs
 */
#ifndef RE_SPATIALQUERIES_H
#define RE_SPATIALQUERIES_H

#include "react/common.h"
#include "react/math.h"

namespace re {
  class Entity;

  struct Intersect {
    Intersect() : depth(RE_INFINITY), point(), normal() { }
    reFloat depth;
    re::vec3 point;
    re::vec3 normal;
  };

  struct RayQuery : public Intersect {
    RayQuery() : Intersect(), entity(nullptr) { }
    RayQuery& operator=(const RayQuery& q) {
      depth = q.depth;
      point = q.point;
      normal = q.normal;
      entity = q.entity;
      return *this;
    }
    Entity* entity;
  };

  extern reUInt globalQueryID;  // defined in common.h
};

/**
 * Base class for all query structs
 */

struct reSpatialQuery {
  /** A unique query ID to avoid redundant queries */
  const reUInt ID;
protected:
  // ensures the ID is never zero
  reSpatialQuery() : ID(
    (++re::globalQueryID == 0) ? ++re::globalQueryID : re::globalQueryID
  ) { }
};

/**
 * The struct which defines spatial queries using rays
 */

struct reRayQuery : reSpatialQuery {
  reRayQuery() : reSpatialQuery(), origin(), dir() { }
  /** The ray origin */
  re::vec3 origin;
  /** The ray direction */
  re::vec3 dir;
};

struct reRayQueryResult {
  reRayQueryResult() : intersect(), normal(), distSq(RE_INFINITY) { }
  re::vec3 intersect;
  re::vec3 normal;
  reFloat distSq;
};

namespace re {
  struct RayResult {
    RayResult() : distSq(RE_INFINITY), entity(nullptr), surfaceIntersect(), surfaceNormal() { }
    reFloat distSq;
    Entity* entity;
    re::vec3 surfaceIntersect;
    re::vec3 surfaceNormal;
  };
  
  struct SingleResult {
    SingleResult() : distSq(RE_INFINITY), entity(nullptr) { }
    reFloat distSq;
    Entity* entity;
  };
}

#endif
