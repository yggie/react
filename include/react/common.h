/**
 * @file
 * Contains global definitions used throughout the engine, including logging
 * and debugging functions
 */
#ifndef RE_COMMON_H
#define RE_COMMON_H

/** a customizable base floating point type */
typedef float reFloat;

typedef int reInt;

typedef unsigned int reUInt;

/** If RE_ZERO_MEMORY is defined, this value will be used to set unused memory */
#ifndef RE_ZERO_MEM_VAL
#define RE_ZERO_MEM_VAL   0
#endif

#include <cstdio>
#include <string.h>

namespace re {
  /**
   * All log outputs will be forwarded to this file
   */
  
  extern FILE* logFile;
}

#define RE_FUNC             __func__

/**
 * A convenient define to obtain short file names
 */

#define RE_FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

/**
 *  Marks features which have not yet been implemented.
 */

#define RE_NOT_IMPLEMENTED \
  fprintf(re::logFile, \
    "[ALERT] %s:%d: (%s)(%s %s) %s\n", RE_FILE, __LINE__, \
    RE_FUNC, __DATE__, __TIME__, "Feature not implemented");

/**
 * Marks areas of the code which should be impossible to react.
 */

#define RE_IMPOSSIBLE \
  fprintf(re::logFile, "[ALERT] %s:%d: Impossible program flow!\n", RE_FILE, __LINE__);

/**
 * Behaves like a printf statement, but includes additional information such
 * as line number, file name and an info tag.
 */

#define RE_INFO(...) \
  fprintf(re::logFile, "[INFO]  %s:%d: ", RE_FILE, __LINE__), \
  fprintf(re::logFile, __VA_ARGS__);

/**
 * Behaves like a printf statement, but includes additional information such
 * as line number, file name and a warning tag.
 */

#define RE_WARN(...) \
  fprintf(re::logFile, "[WARN]  %s:%d: ", RE_FILE, __LINE__), \
  fprintf(re::logFile, __VA_ARGS__);

#ifdef NDEBUG
  #define RE_ZERO_MEMORY
  
  #ifndef RE_SOFT_STOP
    #define _RE_STOP_PROGRAM    __builtin_trap();
  #else
    #define _RE_STOP_PROGRAM
  #endif

  #define RE_DEBUG(...) \
    fprintf(re::logFile, "[DEBUG] %s:%d: ", RE_FILE, __LINE__), \
    fprintf(re::logFile, __VA_ARGS__);
  
  #define RE_EXPECT_MSG(expr, msg) \
  do { \
    if(!(expr)) { \
      RE_WARN("Expected (%s) to be true but got false! %s\n", #expr, msg) \
    } \
  } while(0);
  
  #define RE_EXPECT(expr)   RE_EXPECT_MSG(expr, "")
  
  #define RE_ASSERT_MSG(expr, msg) \
  do { \
    if(!(expr)) { \
      fprintf(re::logFile, "[FATAL] %s:%d: Assertion in expression (%s) failed! %s\n", RE_FILE, __LINE__, #expr, msg); \
      _RE_STOP_PROGRAM \
    } \
  } while(0);
  
  #define RE_ASSERT(expr)   RE_ASSERT_MSG(expr, "")
  
#else
  /**
   * Asserts a given condition, halting the program and showing a message if it
   * fails. Does nothing if NDEBUG is not defined
   * 
   * @param expr The assert condition
   * @param msg The message shown on failure
   */
  #define RE_ASSERT_MSG(expr, msg)
  
  #define RE_ASSERT(expr)
  
  /**
   * Expects a given condition to be true, displays a warning message if it
   * fails. Does nothing if NDEBUG is not defined
   * 
   * @param expr The condition expected to be true
   * @param msg The message shown on failure
   */
  #define RE_EXPECT_MSG(expr, msg)
  
  #define RE_EXPECT

  /**
   * Behaves like a printf statement, but includes additional information such
   * as line number, file name and a debug tag. Does nothing if NDEBUG is not
   * defined
   */
  #define RE_DEBUG(...)
#endif

#include "react/Memory/reBaseAllocator.h"

class reShape;
class reEnt;

namespace re {
  
  /** A single shared allocator instance to allocate memory for the engine */
  extern reAllocator* globalAllocator;

  /**
   * Uses the global allocator to allocate memory for the object using the
   * default constructor
   * 
   * @return The allocated instance
   */
  
  template <class T> T* alloc_new() {
    return new T();
//  return new (globalAllocator->alloc(sizeof(T), __alignof(T))) T();
  }

  /**
   * Uses the global allocator to allocate memory for the object using a
   * constructor with a single argument
   * 
   * @param arg A single argument
   * @return The allocated instance
   */
  
  template <class T, class Y> T* alloc_new(Y arg) {
    return new T(arg);
//    return globalAllocator->alloc_new<T, Y>(arg);
  }
  
  template <class T> void alloc_delete(T* ptr) {
    delete ptr;
//    globalAllocator->alloc_delete<T>(ptr);
  }
  
  reShape* copyOf(const reShape& shape);
  template <> void alloc_delete<reShape>(reShape* shape);
};

#endif
