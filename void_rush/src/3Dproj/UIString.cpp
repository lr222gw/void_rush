#include "UIString.h"

UIString::UIString(Graphics*& gfx, std::string str, vec2 position, vec2 size)
{
	for (int i = 0; i < str.size(); i++) {
		symbols.push_back(new UISymbols(gfx,vec2(position.x + size.x * i , position.y), size));
		symbols[i]->symbol_to_uv(str[i]);
	}
}

UIString::~UIString()
{
	for (size_t i = 0; i < symbols.size(); i++) {
		delete symbols[i];
	}
}

void UIString::move(vec2 m)
{
	for (int i = 0; i < this->symbols.size(); i++) {
		symbols[i]->movePosition(m.x, m.y);
	}
}

void UIString::setPosition(vec2 m)
{
	for (int i = 0; i < this->symbols.size(); i++) {
		symbols[i]->setPosition(m.x + symbols[i]->getSize().x * i, m.y);
	}
}

void UIString::hide(bool h)
{
	this->hiding = h;
}

void UIString::draw(Graphics*& gfx)
{
	for (size_t i = 0; i < symbols.size(); i++) {
		symbols[i]->draw(gfx);
	}
}
