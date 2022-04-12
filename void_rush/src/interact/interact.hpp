#include "../common/Vector.hpp"
#include "../common/Helper.hpp"
#include <math.h>
#include <stdlib.h>
#include <iostream>

bool CanInteract (Vector3 camPos, Vector3 camDir, Vector3 itemPos,
                  float itemRad = 10.0f, float maxDistance = 10.0f);

void TestIntersection();