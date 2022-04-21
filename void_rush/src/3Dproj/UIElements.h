#pragma once
#include "Vec.h"
#include "Graphics.h"

struct UIVCB : public CB {
	struct {
		float element[2];
	}size;
	struct {
		float element[2];
	}position;
	struct {
		float element[4];
	}UVSizeWXY;
};

class UIElements {
public:
	UIElements(vec2 _pos, vec2 _size, Graphics*& gfx);
	void updateConstBuffer(Graphics*& gfx);
	void setSize(float x, float y);
	void setPosition(float x, float y);
	void setUVSize(float x, float y);
	void setUVPosition(float x, float y);

	void moveSize(float x, float y);
	void movePosition(float x, float y);
	void moveUVSize(float x, float y);
	void moveUVPosition(float x, float y);

	vec2 getSize();
	vec2 getPosition();
	vec2 getUVSize();
	vec2 getUVPosition();

	virtual ~UIElements();
	virtual void draw(Graphics*& gfx) = 0;
protected:
	ID3D11Buffer* constBuffer;
private:
	UIVCB constBufferData;
};
