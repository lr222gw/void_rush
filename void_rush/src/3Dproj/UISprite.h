#pragma once
#include "UIElements.h"
#include "ResourceManager.h"

class UISprite : public UIElements {
public:
	UISprite(ID3D11ShaderResourceView*& _sprite, vec2 pos, vec2 size, Graphics*& gfx);
	ID3D11ShaderResourceView* sprite;	

	void replaceSprite(ID3D11ShaderResourceView* _sprite) override;

	void draw(Graphics*& gfx) override;
private:
};