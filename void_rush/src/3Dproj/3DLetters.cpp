#include "3DLetters.h"

Letters3D::Letters3D(std::string str, vec3 pos, ResourceManager* rm, Graphics*& gfx, vec2 size)
{
	this->pos = pos;
	for (size_t i = 0; i < str.size(); i++) {
		letters.push_back(new GameObject(rm->get_Models("3DText.obj", gfx), gfx, pos, vec3(0, 0, 0), vec3(1, size.y, size.x)));
		float a = str.size() * size.x - str.size() / 2;
		letters[i]->setPoint(vec3(0, 0,
			//i * 2.f * size.x - (str.size() / 2.f)

			 (i*2) + 1 - (float)str.size()
		 ));
		symbol_to_uv(str[i]);
	}

	bufferData.UV.element[0] = 1;
	bufferData.UV.element[1] = 1;
	CreateConstBuffer(gfx, uvBuffer, sizeof(LetCB), &bufferData);
	rotateToPlayer = true;
}

Letters3D::~Letters3D()
{
	for (int i = 0; i < letters.size(); i++) {
		delete letters[i];
	}
	uvBuffer->Release();
}

void Letters3D::draw(Graphics*& gfx)
{
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
	vec3 forward(0,0,1);    
	vec3 objectToCam = camPos - pos;
	float rot = vec3(objectToCam.x, 0, objectToCam.z).angle(forward);
	for (size_t i = 0; i < letters.size(); i++) {
		letters[i]->Updateshaders(gfx);
		if (rotateToPlayer) {
			//letters[i]->addRot(vec3(0, 0.01,0));
			letters[i]->setRot(vec3(0, -rot,0)+ vec3(0,-1.57,0));
		}
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

void Letters3D::setRotateWithPlayer(bool rotate)
{
	this->rotateToPlayer = rotate;
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
