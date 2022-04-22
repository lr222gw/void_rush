#include "UIButton.h"
#include "Mouse.h"

UIButton::UIButton(ID3D11ShaderResourceView*& _sprite, Graphics*& gfx, Mouse* mouse, vec2 pos, vec2 size):
    UISprite(_sprite,pos,size,gfx)
{
    CWS = gfx->getClientWH();
    this->mouse = mouse;
    hovered = false;
}

void UIButton::update()
{
    //change space from 0 to 1 -> -1 to 1
    hovered = false;
    float mNDCx = (mouse->getPosX() / CWS.x) * 2 - 1;
    float mNDCy = ((CWS.y - mouse->getPosY()) / CWS.y) * 2 - 1;//flip the Y
    if (mNDCx > getPosition().x && mNDCx < getPosition().x + getSize().x &&
        mNDCy > getPosition().y && mNDCy < getPosition().y + getSize().y) {
        //change quality?
        hovered = true;
    }
}

bool UIButton::clicked()
{
    return mouse->IsLeftDown() && hovered;
}
