#include "interact.hpp"

bool CanInteract (vec3 camPos, vec3 camDir, vec3 itemPos,
                  float itemRad, float maxDistance)
{

   /* if(get_distance(camPos, itemPos) > maxDistance){
        return false;
    }
    Vector3 dir = camPos - itemPos;
    dir.normalize();

    if(dot(dir, camDir) > itemRad){
        return false;
    }
    return true;*/

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
    else if (s > maxDistance)
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

void TestIntersection(){
    vec3 camPos = vec3(0.0f, 6.0f, -10.0f);
    vec3 camDir = vec3(0.0f, 0.0f, 1.0f);
    vec3 itemPos = vec3(0.0f, 6.0f, 0.0f);
    float itemRad = 1.0f;
    float maxDistance = 10.0f;

    if(CanInteract(camPos,  camDir,  itemPos, itemRad, maxDistance)){
        std::cout<<"Intersect!.\n";
    }
}