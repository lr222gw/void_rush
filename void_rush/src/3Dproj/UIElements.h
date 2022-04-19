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
	void setUVPosition(int x, int y);
	virtual ~UIElements();
	virtual void draw(Graphics*& gfx) = 0;
protected:
	ID3D11Buffer* constBuffer;
private:
	UIVCB constBufferData;
};
