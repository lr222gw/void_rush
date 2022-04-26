#include "3DLetters.h"

Letters3D::Letters3D(std::string str, vec3 pos, ResourceManager* rm, Graphics*& gfx, vec2 size)
{
	this->pos = pos;
	for (size_t i = 0; i < str.size(); i++) {
		letters.push_back(new GameObject(rm->get_Models("3DText.obj", gfx), gfx, pos, vec3(0, 0, 0), vec3(1, size.y, size.x)));
		letters[i]->setPoint(vec3(0, 0, i * 2.f * size.x - (str.size() / 2.f)));
		symbol_to_uv(str[i]);
	}
	ID3D11Texture2D* tex;
	if (!CreateTexture("assets/textures/Fonts/TheFont.png", gfx->getDevice(), tex, font)) {
		std::cout << "error cannot load font" << std::endl;
	}
	std::string a;
	loadVShader("3DTextVS.cso",gfx->getDevice(), vShader, a);
	loadPShader("3DTextPS.cso", gfx->getDevice(), pShader);
	bufferData.UV.element[0] = 1;
	bufferData.UV.element[1] = 1;
	CreateConstBuffer(gfx, uvBuffer, sizeof(LetCB), &bufferData);
}

Letters3D::~Letters3D()
{
	for (int i = 0; i < letters.size(); i++) {
		delete letters[i];
	}
	vShader->Release();
	pShader->Release();
	uvBuffer->Release();
}

void Letters3D::draw(Graphics*& gfx)
{
	gfx->get_IMctx()->PSSetShaderResources(0, 1, &font);
	gfx->get_IMctx()->PSSetShader(pShader, nullptr, 0);
	gfx->get_IMctx()->VSSetShader(vShader, nullptr, 0);
	gfx->get_IMctx()->VSSetConstantBuffers(1, 1, &uvBuffer);
	D3D11_MAPPED_SUBRESOURCE resource;

	for (size_t i = 0; i < letters.size(); i++) {
		bufferData.UV.element[0] = u[i];
		bufferData.UV.element[1] = v[i];

		gfx->get_IMctx()->Map(uvBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &bufferData, sizeof(LetCB));
		gfx->get_IMctx()->Unmap(uvBuffer, 0);
		ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

		letters[i]->drawRaw(gfx);
	}
}

void Letters3D::update(Graphics*& gfx, vec3 camPos)
{
	//update shaders
	for (size_t i = 0; i < letters.size(); i++) {
		letters[i]->Updateshaders(gfx);
		letters[i]->addRot(vec3(0, 0, 0.01));
	}
}

void Letters3D::move(vec3 pos)
{
	this->pos = this->pos + pos;
	for (size_t i = 0; i < letters.size(); i++) {
		letters[i]->movePos(pos);
	}
}

void Letters3D::setPos(vec3 pos)
{
	this->pos = pos;
	for (size_t i = 0; i < letters.size(); i++) {
		letters[i]->setPos(pos);
	}
}

void Letters3D::symbol_to_uv(char symbol)
{
	int intSymbol = (int)symbol - 32;
	if (intSymbol < 0) {
		std::cout << "error unknown characater" << std::endl;
	}
	int xSymbol, ySymbol;
	xSymbol = intSymbol % 8;
	ySymbol = intSymbol / 8;
	u.push_back(xSymbol);
	v.push_back(ySymbol);
}
