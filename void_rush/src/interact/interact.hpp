//#include "../common/Vector.hpp"
//#include "../common/Helper.hpp"
#include "3Dproj/Vec.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "3Dproj/GameObject.h"

bool CanInteract (vec3 camPos, vec3 camDir, vec3 itemPos, float itemRad, float maxDistance, float& rayDist);

vec3 GetMidPos(GameObject* object, float& size); 
vec3 GetMidPos(GameObject* object);