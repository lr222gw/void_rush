#pragma once
#include "UIElements.h"
#include "vec.h"
#include <d3d11.h>

class UISymbols : public UIElements {
public: 
	UISymbols(Graphics*& gfx, vec2 position, vec2 size);
	void symbol_to_uv(char symbol);

	void draw(Graphics*& gfx) override;
private:
};