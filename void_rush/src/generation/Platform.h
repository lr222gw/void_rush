#pragma once
#include <vector>
#include <math.h>
#include <memory>
#include "../common/Vector.hpp"

class platform {
private:
	Vector3 pos;
	int obstacles;
	int difficluty;
public:
	platform();
	platform(std::vector<float> pos, int obstacles, int difficluty);
	platform(Vector3 pos, int obstacles, int difficluty);
	~platform();
	void setPosition(float x, float y, float z);
	void setPosition(Vector3 position);
	void move(float xOfset, float yOfset, float zOfset);
	Vector3 getPos();
	float distance(Vector3 position);
	float distance(std::vector<float> &position);
	platform* next;
};