#include "UISprite.h"

UISprite::UISprite(ID3D11ShaderResourceView*& _sprite, vec2 pos, vec2 size, Graphics *& gfx)
	: UIElements(pos, size, gfx), sprite(_sprite)
{

}

void UISprite::replaceSprite(ID3D11ShaderResourceView* _sprite)
{
	this->sprite = _sprite;
}

void UISprite::draw(Graphics*& gfx)
{
	gfx->get_IMctx()->VSSetConstantBuffers(0, 1, &this->constBuffer);
	gfx->get_IMctx()->PSSetShaderResources(0,1,&sprite);
	gfx->get_IMctx()->Draw(4, 0);
}
