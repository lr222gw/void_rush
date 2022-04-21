#pragma once
#include <vector>
#include "UISymbols.h"
#include "vec.h"
class UIString {
public:
	UIString(Graphics*& gfx, std::string str, vec2 position, vec2 size);
	virtual ~UIString();
	void draw(Graphics*& gfx);
private:
	std::vector<UISymbols*> symbols;
	std::string dataString;
};