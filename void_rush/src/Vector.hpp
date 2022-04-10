#pragma once
#include <math.h>

struct Vector2 {
	float x;
	float y;

	Vector2(){
		x = 0.0f;
		y = 0.0f;
	}
	Vector2(float xPos, float yPos) {
		x = xPos;
		y = yPos;
	}

	void normalizeXY(){
		x = x/length();
		y = y/length();
	}

	float length() {
		return sqrtf(x * x + y * y);
	}
};

struct Vector3 {
	float x;
	float y;
	float z;

	Vector3() {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}
	Vector3(float xPos, float yPos, float zPos) {
		x = xPos;
		y = yPos;
		z = zPos;
	}
	
	void normalizeXY(){
		x = x/magnitude();
		y = y/magnitude();
	}

	float magnitude() {
		return sqrtf(x * x + y * y);
	}

	float length() {
		return sqrtf(x * x + y * y + z * z);
	}

	Vector3& operator -=(const Vector3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
};