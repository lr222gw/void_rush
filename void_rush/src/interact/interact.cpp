#include "interact.hpp"

bool CanInteract (vec3 camPos, vec3 camDir, vec3 itemPos,
                  float itemRad, float maxDistance)
{
    camDir.Normalize();

    // l = distance from camPos to itemPos
    vec3 l = itemPos - camPos;

    // s = distance from rays orig to center of sphere in the direction of the
    // ray
    //double s = dot(l, camDir);
    //double l2 = dot(l, l);
    double s = l * camDir;
    double l2 = l * l;

    // Item is behine camera, and not inside the items radius
    if (s < 0 && l2 > powf (itemRad, 2))
    {
        return false;
    }
    // Item is too far away
    else if (s > maxDistance+itemRad)
    {
        return false;
    }

    //m becomes the nerest point on the ray from the center of the sphere
    double m2 = l2 - powf (s, 2);
    if (m2> powf (itemRad, 2)){
        return false;
    }

    return true;
}