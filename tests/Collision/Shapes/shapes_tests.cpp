#include "helpers.h"

// volume-less shape types
#include "Ray.h"
#include "Plane.h"

// complex shape types
#include "reSphere.h"
#include "reProxyShape.h"

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
