#pragma once
#include "UISprite.h"

class Mouse;

class UIButton : public UISprite{
public:
	UIButton(ID3D11ShaderResourceView*& _sprite, Graphics*& gfx, Mouse* mouse, vec2 pos = vec2(0,0), vec2 size = vec2(0,0));
	void update();
	bool clicked();
	bool hover();

private:
	bool hovered;
	vec2 CWS;
	Mouse* mouse;
	float a = 2;
};