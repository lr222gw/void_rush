#include "Collision3D.h"


bool RayRect(vec3& rayOrigin, vec3 rayDir, ColCube& cube, vec3& cp, vec3& cn, float& t)
{
	vec3 t_near = (cube.lowPoint - rayOrigin) / rayDir;
	vec3 t_far = (cube.highPoint - rayOrigin) / rayDir;

	float txMin = (cube.lowPoint.x - rayOrigin.x) / rayDir.x;
	float txMax = (cube.highPoint.x - rayOrigin.x) / rayDir.x;
	float tyMin = (cube.lowPoint.y - rayOrigin.y) / rayDir.y;
	float tyMax = (cube.highPoint.y - rayOrigin.y) / rayDir.y;
	float tzMin = (cube.lowPoint.z - rayOrigin.z) / rayDir.z;
	float tzMax = (cube.highPoint.z - rayOrigin.z) / rayDir.z;
	if (txMax < txMin) { float tmp = txMax; txMax = txMin; txMin = tmp; }
	if (tyMax < tyMin) { float tmp = tyMax; tyMax = tyMin; tyMin = tmp; }
	if (tzMax < tzMin) { float tmp = tzMax; tzMax = tzMin; tzMin = tmp; }

	float tMin = txMin;
	if (tMin < tyMin) { tMin = tyMin; }
	if (tMin < tzMin) { tMin = tzMin; }
	float tMax = txMax;
	if (tMax > tyMax) { tMax = tyMax; }
	if (tMax > tzMax) { tMax = tzMax; }

	if (!(txMin > tyMax || tyMin > txMax)) {
		if (!(tMin > tzMax || tzMin > tMax)) {
			if (tzMin > tMin) {
				tMin = tzMin;
			}
			if (tzMax > tMax) {
				tMax = tzMax;
			}
			float distance = tMin;
			if (distance < 0) {
				distance = tMax;
			}
			if (distance > 0) {
				t = distance;
				cp = rayOrigin + rayDir * t;
				//check normal
				if (t_near.x > t_near.y && t_near.x > t_near.z) {
					if (rayDir.x < 0) {
						cp = vec3(1, 0, 0);
					}
					else {
						cp = vec3(-1, 0, 0);
					}
				}
				else if (t_near.y > t_near.x && t_near.y > t_near.z) {
					if (rayDir.y < 0) {
						cp = vec3(0, 1, 0);
					}
					else {
						cp = vec3(0, -1, 0);
					}
				}
				else if (t_near.z > t_near.y && t_near.z > t_near.x) {
					if (rayDir.z < 0) {
						cp = vec3(1, 0, 1);
					}
					else {
						cp = vec3(1, 0, 0);
					}
				}

			}
			//returnVar.distance = distance;  
		}
	}

	return false;
}

bool RectRect(GameObject*& objectA, ColCube& in, ColCube& target, vec3& cp, vec3& cn, float& contact_t, float elapsed_t)
{
	vec3 rayDir = objectA->getPos() - objectA->getlastPosition();
	if (rayDir.x == 0 && rayDir.y == 0 && rayDir.z == 0) {
		return false;
	}
	ColCube expandedTarget(
		target.highPoint + (in.highPoint - in.lowPoint) * 0.5,
		target.lowPoint - (in.highPoint - in.lowPoint) * 0.5
	);
	vec3 pos = objectA->getPos();
	if (RayRect(pos, rayDir * elapsed_t, expandedTarget, cp, cn, contact_t)) {
		if (contact_t > 0 && contact_t < 1.f) {
			return true;
		}
	}

	return false;
}

void moveObject(GameObject*& objectA, GameObject*& objectB, ColCube colCubeA, ColCube colCubeB)
{
	vec3 lowdir(colCubeA.highPoint - colCubeB.lowPoint);
	vec3 highdir(colCubeB.highPoint - colCubeA.lowPoint);
	float dirs[6]{ lowdir.x,lowdir.y ,lowdir.z ,highdir.x ,highdir.y ,highdir.z };
	float lowest = dirs[0];
	int index = 0;
	for (int i = 1; i < 6; i++) {
		if (lowest > dirs[i]) {
			index = i;
			lowest = dirs[i];
		}
	}
	float newxyzPos;
	vec3 dir;
	switch (index)
	{
	case 0:
		newxyzPos = (objectA->getPos().x - colCubeA.highPoint.x) + colCubeB.lowPoint.x;
		objectA->setPos(vec3(newxyzPos, objectA->getPos().y, objectA->getPos().z));
		break;
	case 1:
		newxyzPos = (objectA->getPos().y - colCubeA.highPoint.y) + colCubeB.lowPoint.y;
		objectA->setPos(vec3(objectA->getPos().x, newxyzPos, objectA->getPos().z));
		break;
	case 2:
		newxyzPos = (objectA->getPos().z - colCubeA.highPoint.z) + colCubeB.lowPoint.z;
		objectA->setPos(vec3(objectA->getPos().x, objectA->getPos().y, newxyzPos));
		break;
	case 3:
		newxyzPos = (objectA->getPos().x - colCubeA.lowPoint.x) + colCubeB.highPoint.x;
		objectA->setPos(vec3(newxyzPos, objectA->getPos().y, objectA->getPos().z));
		break;
	case 4:
		newxyzPos = (objectA->getPos().y - colCubeA.lowPoint.y) + colCubeB.highPoint.y;
		objectA->setPos(vec3(objectA->getPos().x, newxyzPos, objectA->getPos().z));
		break;
	case 5:
		newxyzPos = (objectA->getPos().z - colCubeA.lowPoint.z) + colCubeB.highPoint.z;
		objectA->setPos(vec3(objectA->getPos().x, objectA->getPos().y, newxyzPos));
		break;
	}
}

void collisionWithBlocking(GameObject* objectA, GameObject* objectB)
{
	DirectX::XMFLOAT4 a[2];
	DirectX::XMFLOAT4 b[2];
	objectA->getBoundingBox(a);
	objectB->getBoundingBox(b);
	ColCube colCubeA(a);
	ColCube colCubeB(b);
	if (collision3D(colCubeA, colCubeB))
	{
		//check later if object can move
		if (objectA->getWeight() < objectB->getWeight()) {
			moveObject(objectA, objectB, colCubeA, colCubeB);
		}
		else {
			moveObject(objectB, objectB, colCubeB, colCubeA);
		}
	}
}

bool collision3D(ColSphere objectA, ColSphere objectB)
{
	return (objectA.position - objectB.position).length() < objectA.size + objectB.size;
}

bool collision3D(ColSphere objectA, ColCube objectB)
{
	float sqDist = 0.0f;
	for (int i = 0; i < 3; i++) {
		float v = objectA.position.getWithNumber(i);
		if (v < objectB.lowPoint.getWithNumber(i)) {
			sqDist += (objectB.lowPoint.getWithNumber(i) - v) * (objectB.lowPoint.getWithNumber(i) - v);
		}
		if (v > objectB.highPoint.getWithNumber(i)) {
			sqDist += (v - objectB.highPoint.getWithNumber(i)) * (v - objectB.highPoint.getWithNumber(i));
		}
	}
	return sqDist <= objectA.size * objectA.size;
}

bool collision3D(DirectX::XMFLOAT4 objectA[], DirectX::XMFLOAT4 objectB[])
{
	ColCube ColCubeA(objectA);
	ColCube ColCubeB(objectB);
	return collision3D(ColCubeA, ColCubeB);
}


bool collision3D(GameObject* objectA, GameObject* objectB, bool fromobjA, bool fromobjB)
{
	ColCube ColCubeA;
	ColCube ColCubeB;
	DirectX::XMFLOAT4 a[2];
	DirectX::XMFLOAT4 b[2];
	if (!fromobjA) {
		objectA->getBoundingBox(a);
	}
	else {
		objectA->getBoundingBoxFromObject(a);
	}
	if (!fromobjB) {
		objectB->getBoundingBox(b);
	}
	else {
		objectB->getBoundingBoxFromObject(b);
	}
	return collision3D(a,b);
}

bool collision3D(ColCube objectA, ColCube objectB)
{
	return (
		//check x
		(
			objectA.lowPoint.x < objectB.highPoint.x&&
			objectA.highPoint.x > objectB.lowPoint.x
			) &&
		//check y
		(
			objectA.lowPoint.y < objectB.highPoint.y&&
			objectA.highPoint.y > objectB.lowPoint.y
			)
		&&
		//check z
		(
			objectA.lowPoint.z < objectB.highPoint.z&&
			objectA.highPoint.z > objectB.lowPoint.z
			)
		);
}



bool collision3D(DirectX::BoundingBox objectA, DirectX::BoundingBox objectB)
{
	return objectA.Intersects(objectB);
}



void moveObject(GameObject*& player, ColCube colCubeA, ColCube colCubeB)
{
	vec3 lowdir(colCubeA.highPoint - colCubeB.lowPoint);
	vec3 highdir(colCubeB.highPoint - colCubeA.lowPoint);
	float dirs[6]{ lowdir.x,lowdir.y ,lowdir.z ,highdir.x ,highdir.y ,highdir.z };
	float lowest = dirs[0];
	int index = 0;
	for (int i = 1; i < 6; i++) {
		if (lowest > dirs[i]) {
			index = i;
			lowest = dirs[i];
		}
	}
	float newxyzPos;
	vec3 dir;
	switch (index)
	{
	case 0:
		newxyzPos = (player->getPos().x - colCubeA.highPoint.x) + colCubeB.lowPoint.x;
		player->setPos(vec3(newxyzPos, player->getPos().y, player->getPos().z));
		break;
	case 1:
		newxyzPos = (player->getPos().y - colCubeA.highPoint.y) + colCubeB.lowPoint.y;
		player->setPos(vec3(player->getPos().x, newxyzPos, player->getPos().z));
		break;
	case 2:
		newxyzPos = (player->getPos().z - colCubeA.highPoint.z) + colCubeB.lowPoint.z;
		player->setPos(vec3(player->getPos().x, player->getPos().y, newxyzPos));
		break;
	case 3:
		newxyzPos = (player->getPos().x - colCubeA.lowPoint.x) + colCubeB.highPoint.x;
		player->setPos(vec3(newxyzPos, player->getPos().y, player->getPos().z));
		break;
	case 4:
		newxyzPos = (player->getPos().y - colCubeA.lowPoint.y) + colCubeB.highPoint.y;
		player->setPos(vec3(player->getPos().x, newxyzPos, player->getPos().z));
		break;
	case 5:
		newxyzPos = (player->getPos().z - colCubeA.lowPoint.z) + colCubeB.highPoint.z;
		player->setPos(vec3(player->getPos().x, player->getPos().y, newxyzPos));
		break;
	}
}


void collisionWithBlocking(DirectX::XMFLOAT4 platform_min_max_bounds[], GameObject* player)
{
	DirectX::XMFLOAT4 b[2];
	player->getBoundingBox(b);
	ColCube colCubeA(platform_min_max_bounds);
	ColCube colCubeB(b);
	if (collision3D(colCubeA, colCubeB))
	{
		moveObject(player, colCubeB, colCubeA);
	}
}
