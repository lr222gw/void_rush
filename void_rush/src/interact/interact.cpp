#include "interact.hpp"

bool CanInteract (vec3 camPos, vec3 camDir, vec3 itemPos,
                  float itemRad, float maxDistance, float& rayDist)
{
    camDir.Normalize();

    // l = distance from camPos to itemPos
    vec3 l = itemPos - camPos;

    // s = distance from rays orig to center of sphere in the direction of the
    // ray
    //double s = dot(l, camDir);
    //double l2 = dot(l, l);
    float s = l * camDir;
    float l2 = l * l;

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

    rayDist = sqrt(m2);

    return true;
}

vec3 GetMidPos(GameObject* object, float& size)
{
    DirectX::XMFLOAT4 bb[2];
    DirectX::XMFLOAT3 objMidPos;

    object->getBoundingBox(bb);
    float xSize = fabs(bb[1].x - bb[0].x);
    float ySize = fabs(bb[1].y - bb[0].y);
    float zSize = fabs(bb[1].z - bb[0].z);
    if (xSize >= ySize && xSize >= zSize)
        size = xSize;
    else if (ySize >= xSize && ySize >= zSize)
        size = ySize;
    else
        size = zSize;

    objMidPos = DirectX::XMFLOAT3(bb[0].x + xSize / 2, bb[0].y + ySize / 2, bb[0].z + zSize / 2);

    vec3 ret = vec3(objMidPos.x, objMidPos.y, objMidPos.z);
    return ret;
}

vec3 GetMidPos(GameObject* object)
{
    DirectX::XMFLOAT4 bb[2];
    DirectX::XMFLOAT3 objMidPos;

    object->getBoundingBox(bb);
    float size;
    float xSize = fabs(bb[1].x - bb[0].x);
    float ySize = fabs(bb[1].y - bb[0].y);
    float zSize = fabs(bb[1].z - bb[0].z);
    if (xSize >= ySize && xSize >= zSize)
        size = xSize;
    else if (ySize >= xSize && ySize >= zSize)
        size = ySize;
    else
        size = zSize;

    objMidPos = DirectX::XMFLOAT3(bb[0].x + xSize / 2, bb[0].y + ySize / 2, bb[0].z + zSize / 2);

    vec3 ret = vec3(objMidPos.x, objMidPos.y, objMidPos.z);
    return ret;
}
