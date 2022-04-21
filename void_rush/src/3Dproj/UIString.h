#pragma once
#include <vector>
#include "UISymbols.h"
#include "vec.h"
class UIString {
public:
	UIString(Graphics*& gfx, std::string str, vec2 position, vec2 size);
	virtual ~UIString();
	void move(vec2 m);
	void setPosition(vec2 m);
	void hide(bool h);
	void draw(Graphics*& gfx);
private:
	bool hiding;
	std::vector<UISymbols*> symbols;
	std::string dataString;
};