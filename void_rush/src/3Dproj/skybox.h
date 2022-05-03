#pragma once
#include "GameObject.h"
#include "plHelper.h"

class SkyBox : public GameObject{
public:
	SkyBox(ModelObj* file, Graphics*& gfx, vec3 pos, ID3D11ShaderResourceView* textureCube);
	virtual ~SkyBox();
	void draw(Graphics*& gfx);
	void update(vec3 position);
private:
};

