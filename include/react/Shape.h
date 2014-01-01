#ifndef RE_SHAPE_H
#define RE_SHAPE_H

#include "react/common.h"

namespace re {
  
  /**
   * @ingroup shapes
   * Represents an shape with defined geometric qualities
   */
  
  class Shape {
  public:
    
    /** The types of shapes supported by the engine */
    enum Type {
      /** A sphere @see Sphere */
      SPHERE,
      /** A rectangle @see Rectangle */
      RECTANGLE,
      /** A compound shape type */
      COMPOUND
    };
    
    Shape();
    virtual ~Shape();
    
    virtual Type type() const = 0;
    virtual reFloat volume() const = 0;
    virtual const mat computeInertia() const = 0;
  };
  
  /**
   * @fn Type Shape::type() const
   * @brief Returns the Shape's type
   * 
   * @return The Shape's type
   */
  
  /**
   * @fn reFloat Shape::volume() const
   * @brief Returns the volume of the shape
   * 
   * @return The calculated volume in user-defined units
   */
  
  /**
   * @fn const mat computeInertia() const
   * @brief Returns the inertia tensor assuming unit mass
   * 
   * @return The inertia tensor in user-defined units
   */
  
  inline Shape::Shape() { }
  inline Shape::~Shape() { }
}

#endif