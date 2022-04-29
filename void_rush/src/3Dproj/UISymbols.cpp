#include "UISymbols.h"

UISymbols::UISymbols(Graphics*& gfx, vec2 position, vec2 size):
	UIElements(position, size, gfx)
{
}

void UISymbols::symbol_to_uv(char symbol)
{
	//32 = space
	//then go from there
	int intSymbol = (int)symbol - 32;
	if (intSymbol < 0) {
		std::cout << "error unknown characater" << std::endl;
	}
	//15
	int xSymbol, ySymbol;
	xSymbol = intSymbol % 8;
	ySymbol = intSymbol / 8;
	this->setUVPosition(xSymbol, ySymbol);
	this->setUVSize(1.f / 8.f, 1.f / 16.f);
}

void UISymbols::replaceSprite(ID3D11ShaderResourceView* _sprite)
{
	//Do nothing
}

void UISymbols::draw(Graphics*& gfx)
{
	this->updateConstBuffer(gfx);
	gfx->get_IMctx()->VSSetConstantBuffers(0, 1, &this->constBuffer);
	gfx->get_IMctx()->Draw(4, 0);
}
