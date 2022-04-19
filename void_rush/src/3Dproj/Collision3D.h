#pragma once
#include "Vec.h"
#include "GameObject.h"
#include <DirectXCollision.h>
#include "otherHelps.h"
#include "rect.h"

struct ColCube {
	vec3 highPoint;
	vec3 lowPoint;
	ColCube(vec3 highPoint, vec3 lowPoint) {
		this->highPoint = highPoint;
		this->lowPoint = lowPoint;
	};
	ColCube(DirectX::XMFLOAT4 boundingBox[]) {
		lowPoint = vec3(boundingBox[0]);
		highPoint = vec3(boundingBox[1]);
	};
	ColCube(vec3 highPoint, float width, float height, float depth) {
		this->highPoint = highPoint;
		this->lowPoint = highPoint - vec3(width, height, depth);
	};
	ColCube() {

	};
	
};
struct ColSphere {
	ColSphere(vec3 position, float size) {
		this->position = position;
		this->size = size;
	};
	vec3 position;
	float size;
};

 

bool RayRect(vec3 &rayOrigin, vec3 rayDir, ColCube &cube, vec3 &cp, vec3 &cn, float &t);
bool RectRect(GameObject*& objectA, ColCube& in, ColCube& target, vec3& cp, vec3& cn, float& contact_t, float elapsed_t);

//only moves object A!!!
void moveObject(GameObject*& objectA, GameObject*& objectB, ColCube colCubeA, ColCube colCubeB);
//void collisionWithBlocking(GameObject*& objectA, GameObject*& objectB);
//void collisionWithBlocking(GameObject*& objectA, GameObject*& objectB);
void collisionWithBlocking(GameObject* objectA, GameObject* objectB);


//two spheres
bool collision3D(ColSphere objectA, ColSphere objectB);
bool collision3D(ColSphere objectA, ColCube objectB);
bool collision3D(DirectX::XMFLOAT4 objectA[], DirectX::XMFLOAT4 objectB[]);
bool collision3D(GameObject*& objectA, GameObject*& objectB, bool fromobjA = false, bool fromobjB = false);

//sphere and a cube
bool collision3D(ColCube objectA, ColCube objectB);

//collision with two aligned axis cubes
bool collision3D(DirectX::BoundingBox objectA, DirectX::BoundingBox objectB);