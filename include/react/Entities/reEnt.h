/**
 * @file
 * Contains the definition of the reEnt abstract class
 */
#ifndef RE_ENT_H
#define RE_ENT_H

#include "react/math.h"
#include "react/Collision/reAABB.h"
#include "react/Collision/Shapes/reShape.h"
#include "react/Collision/reSpatialQueries.h"

/**
 * @ingroup entities
 * Represents a physical entity, which exists and interacts in the reWorld.
 * 
 * @see reWorld
 */

class reEnt {
public:
  
  /** Defines unique types of entities which exists in the reWorld */
  enum Type {
    /** A rigid body entity with finite mass @see RigidBody */
    RIGID,
    /** A rigid body entity with infinite mass */
    STATIC,
    /** A physical entity with no volume */
    PARTICLE,
    /** A massless physical entity */
    FIELD
  };
  
  /** Default constructor does nothing */
  reEnt();
  /** Default destructor does nothing */
  virtual ~reEnt();
  
  // must be implemented by the subclass
  virtual Type type() const = 0;
  virtual const reVector vel() const = 0;
  virtual const reMatrix rotVel() const = 0;
  
  void update();
  
  const reVector getAABBLowerCorner() const;
  const reVector getAABBUpperCorner() const;
  
  // getter methods which are defined inline
  reShape* shape();
  const reShape* shape() const;
  const reVector pos() const;
  const reMatrix rot() const;
  const reTMatrix transform() const;
  virtual const reVector center() const;
  
  // setter methods which are defined inline
  void setPos(const reVector& position);
  void setPos(reFloat x, reFloat y, reFloat z);
  virtual void setShape(const reShape& shape);
  virtual reEnt& at(const reVector& position) = 0;
  virtual reEnt& at(reFloat x, reFloat y, reFloat z) = 0;
  virtual reEnt& withShape(const reShape& shape) = 0;
  virtual reEnt& withMass(reFloat mass) = 0;
  virtual reEnt& withDensity(reFloat mass) = 0;
  
  // collision queries
  bool intersectsRay(const reRayQuery& query, reRayQueryResult& result);
  
  bool intersectsHyperplane(const reHyperplaneQuery& query);
  bool intersectsHyperplane(const reVector& point, const reVector& dir) const;
  
  /** a pointer to arbitrary data, defined by the user */
  void* userdata;
  
  static reUInt queriesMade;
  
protected:
  /** The reEnt's reShape */
  reShape* _shape;
  /** The reEnt's position vector */
  reVector _vPos;
  /** The reEnt's rotation matrix */
  reMatrix _mRot;
  /** Used internally to avoid repeating queries */
  reUInt _queryID;
};

/**
 * @fn Type reEnt::type() const
 * @brief Returns the reEnt's type, it is unique for each base class
 * 
 * @return The entity type
 */

/**
 * @fn const reVector reEnt::vel() const
 * @brief Returns the reEnt's current velocity
 * 
 * @return The current velocity in 3D space in user-defined units
 */

/**
 * @fn const reMatrix reEnt::rotVel() const
 * @brief Returns the reEnt's rotational velocity matrix
 * 
 * @return The rotational velocity in matrix form
 */

/**
 * @fn reEnt& reEnt::at(const reVector& pos)
 * Set the reEnt's position, this method can be chained
 * 
 * @param position The new position vector
 * @return A reference to the reEnt
 */

/**
 * @fn reEnt& reEnt::at(reFloat x, reFloat y, reFloat z)
 * Set the reEnt's position, this method can be chained
 * 
 * @param x The new position x-coordinates
 * @param y The new position y-coordinates
 * @param z The new position z-coordinates
 * @return A reference to the reEnt
 */

/**
 * @fn reEnt& reEnt::withShape(const reShape& shape)
 * Set the reEnt's reShape, this method can be chained
 * 
 * @param shape The new reShape
 * @return A reference to the reEnt
 */

inline reEnt::reEnt() : userdata(nullptr), _shape(nullptr), _vPos(), _mRot(), _queryID(0) { }
inline reEnt::~reEnt() { }

/**
 * Notifies the reEnt that it should update its AABB
 */

inline void reEnt::update() {
  if (shape() != nullptr) {
    shape()->updateAABB(_mRot);
  }
}

inline const reVector reEnt::getAABBLowerCorner() const {
  if (shape() != nullptr) {
    return center() - shape()->aabb().dimens();
  } else {
    return _vPos;
  }
}

inline const reVector reEnt::getAABBUpperCorner() const {
  if (shape() != nullptr) {
    return center() + shape()->aabb().dimens();
  } else {
    return _vPos;
  }
}

/**
 * Returns the reShape of the object
 * 
 * @return The reShape of the object
 */

inline reShape* reEnt::shape() {
  return _shape;
}

/**
 * Returns the reShape of the object
 * 
 * const version
 * 
 * @return The reShape of the object
 */

inline const reShape* reEnt::shape() const {
  return _shape;
}

/**
 * Returns the position of the reEnt.
 * 
 * @return The position in 3D space in user defined units
 */

inline const reVector reEnt::pos() const {
  return _vPos;
}

/**
 * Returns the reEnt's rotation matrix
 * 
 * @return The rotation in matrix form
 */

inline const reMatrix reEnt::rot() const {
  return _mRot;
}

/**
 * Returns the reEnt's reTMatrix
 * 
 * @return The transformation in matrix form
 */

inline const reTMatrix reEnt::transform() const {
  return reTMatrix(rot(), pos());
}

/**
 * Returns the position of the center of the reShape associated with the reEnt.
 * This differs from reEnt::pos() when the shape has an offset
 * 
 * @return The centroid of the reShape in user-defined units
 */

inline const reVector reEnt::center() const {
  if (shape() != nullptr) {
    return _vPos + shape()->offset();
  } else {
    return _vPos;
  }
}

/**
 * Set the reEnt's position
 * 
 * @param position The new position vector
 */

inline void reEnt::setPos(const reVector& position) {
  _vPos = position;
}

/**
 * Set the reEnt's position
 * 
 * @param x The new position x-coordinates
 * @param y The new position y-coordinates
 * @param z The new position z-coordinates
 */

inline void reEnt::setPos(reFloat x, reFloat y, reFloat z) {
  _vPos.set(x, y, z);
}

/**
 * Set the reEnt's reShape
 * 
 * @param shape The new reShape
 */

inline void reEnt::setShape(const reShape& shape) {
  _shape = re::copyOf(shape);
}

/**
 * Returns true if the reEnt intersects the ray specified
 * 
 * @param origin The ray origin
 * @param dir The ray direction
 * @param intersect An optional argument which is filled with the intersect
 *                  point
 * @param normal An optional argument which is filled with the intersect norm
 * @param queryID Used internally to avoid redundant queries
 * @return True if the ray intersects
 */

inline bool reEnt::intersectsRay(const reRayQuery& query, reRayQueryResult& result) {
  if (_queryID == query.ID) {
    return false; // avoid redundant queries
  }
  reEnt::queriesMade++;
  _queryID = query.ID;
  if (_shape != nullptr) {
    return _shape->intersectsRay(this->transform(), query, result);
  }
  return false;
}

inline bool reEnt::intersectsHyperplane(const reHyperplaneQuery& query) {
  if (_queryID == query.ID) {
    return false; // avoid redundant queries
  }
  _queryID = query.ID;
  if (_shape != nullptr) {
    return _shape->intersectsHyperplane(this->transform(), query);
  } else {
    return (_vPos - query.point).dot(query.dir) > 0.0;
  }
}

inline bool reEnt::intersectsHyperplane(const reVector& point, const reVector& dir) const {
  if (_shape != nullptr) {
    reHyperplaneQuery query;
    query.point = point;
    query.dir = dir;
    return _shape->intersectsHyperplane(this->transform(), query);
  } else {
    return (_vPos - point).dot(dir) > 0.0;
  }
}

#endif
