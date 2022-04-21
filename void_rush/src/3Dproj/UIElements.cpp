#include "UIElements.h"
#include "CreateBuffer.h"

UIElements::UIElements(vec2 _pos, vec2 _size, Graphics*& gfx)
{
    constBufferData.size.element[0] = _size.x;
    //constBufferData.size.element[0] = 1;
    constBufferData.size.element[1] = _size.y;
    //constBufferData.size.element[1] = 1;
    constBufferData.position.element[0] = _pos.x;
    //constBufferData.position.element[0] = 0;
    constBufferData.position.element[1] = _pos.y;
    //constBufferData.position.element[1] = 0;
    constBufferData.UVSizeWXY.element[0] = 1;
    constBufferData.UVSizeWXY.element[1] = 1;
    constBufferData.UVSizeWXY.element[2] = 0;
    constBufferData.UVSizeWXY.element[3] = 0;
    CreateConstBuffer(gfx, constBuffer, sizeof(UIVCB), &constBufferData);
}

void UIElements::updateConstBuffer(Graphics*& gfx)
{
    D3D11_MAPPED_SUBRESOURCE resource;

    gfx->get_IMctx()->Map(this->constBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
    memcpy(resource.pData, &constBufferData, sizeof(UIVCB));
    gfx->get_IMctx()->Unmap(constBuffer, 0);
    ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
}

void UIElements::setSize(float x, float y)
{
    constBufferData.size.element[0] = x;
    constBufferData.size.element[1] = y;
}

void UIElements::setPosition(float x, float y)
{
    constBufferData.position.element[0] = x;
    constBufferData.position.element[1] = y;
}

void UIElements::setUVSize(float x, float y)
{
    constBufferData.UVSizeWXY.element[0] = x;
    constBufferData.UVSizeWXY.element[1] = y;
}

void UIElements::setUVPosition(float x, float y)
{
    constBufferData.UVSizeWXY.element[2] = x;
    constBufferData.UVSizeWXY.element[3] = y;
}

void UIElements::moveSize(float x, float y)
{
    constBufferData.size.element[0] += x;
    constBufferData.size.element[1] += y;
}

void UIElements::movePosition(float x, float y)
{
    constBufferData.position.element[0] += x;
    constBufferData.position.element[1] += y;
}

void UIElements::moveUVSize(float x, float y)
{
    constBufferData.UVSizeWXY.element[0] += x;
    constBufferData.UVSizeWXY.element[1] += y;
}

void UIElements::moveUVPosition(float x, float y)
{
    constBufferData.UVSizeWXY.element[2] += x;
    constBufferData.UVSizeWXY.element[3] += y;
}

vec2 UIElements::getSize()
{
    return vec2(constBufferData.size.element[0],
                constBufferData.size.element[1]
                );
}

vec2 UIElements::getPosition()
{
    return vec2(
        constBufferData.position.element[0],
        constBufferData.position.element[1]
    );
}

vec2 UIElements::getUVSize()
{
    return vec2(constBufferData.UVSizeWXY.element[0],
                constBufferData.UVSizeWXY.element[1]
    );
}

vec2 UIElements::getUVPosition()
{
    return vec2(
                constBufferData.UVSizeWXY.element[2],
                constBufferData.UVSizeWXY.element[3]
    );
}

UIElements::~UIElements()
{
    constBuffer->Release();
}
