#include "UIManager.h"

UIManager::UIManager(ResourceManager*& rm, Graphics*& gfx)
{
	this->gfx = gfx;
	this->rm = rm;
	init(gfx);
}

UIManager::~UIManager()
{
	for (size_t i = 0; i < elements.size(); i++) {
		delete elements[i];
	}
	for (size_t i = 0; i < strings.size(); i++) {
		delete strings[i];
	}
	for (size_t i = 0; i < buttons.size(); i++) {
		delete buttons[i];
	}
	vertexBuffer->Release();
	pShader->Release();
	vShader->Release();
	inputLayout->Release();
	font->Release();
	
}

void UIManager::createUIString(std::string str, vec2 pos, vec2 size, std::string name)
{
	strings.push_back(new UIString(gfx, str,pos, size));
	if (name != "") {
		mapOfString.insert(std::make_pair(name, strings[strings.size() - 1]));
	}
}

void UIManager::createUISprite(std::string rmsprite, vec2 pos, vec2 size, std::string name)
{
	elements.push_back(new UISprite(rm->getSprite(rmsprite, gfx), pos, size, gfx));
	if (name != "") {
		mapOfSprites.insert(std::make_pair(name, elements[elements.size() - 1]));
	}
}

void UIManager::createUIButton(std::string rmsprite, Mouse* mouse, vec2 pos, vec2 size, std::string name)
{
	buttons.push_back(new UIButton(rm->getSprite(rmsprite, gfx), gfx, mouse, pos, size));
	if (name != "") {
		mapOfButtons.insert(std::make_pair(name, buttons[buttons.size() - 1]));
	}
}

void UIManager::createUIButton(std::string rmsprite, std::string str, Mouse* mouse, vec2 pos, vec2 size, std::string name, vec2 pos_offset, vec2 size_offset)
{
	buttons.push_back(new UIButton(rm->getSprite(rmsprite, gfx), gfx, mouse, pos, size));
	if (name != "") {
		mapOfButtons.insert(std::make_pair(name, buttons[buttons.size() - 1]));
	}
	this->strings.push_back(new UIString(gfx, str, pos + pos_offset, (size / str.size()) + size_offset));
	if (name != "") {
		mapOfString.insert(std::make_pair(name, strings[strings.size() - 1]));
	}
}

UIElements* UIManager::getElements(int index)
{
	return elements[index];
}

UIString* UIManager::getStringElement(int index)
{
	return strings[index];
}

UIElements* UIManager::getElements(std::string key)
{
	return mapOfSprites.find(key)->second;
}

UIString* UIManager::getStringElement(std::string key)
{
	return mapOfString.find(key)->second;
}

UIButton* UIManager::getButton(std::string key)
{
	return mapOfButtons.find(key)->second;
}

UIButton* UIManager::getButton(int index)
{
	return buttons[index];
}

void UIManager::deleteElement(int index)
{
	std::map<std::string, UIElements*>::iterator it;
	bool done = false;
	for (it = mapOfSprites.begin(); it != mapOfSprites.end() && !done; it++)
	{
		if (elements[index] == it->second) {
			done = true;
			//delete both
			delete elements[index];
			elements.erase(std::next(elements.begin(), index));
			mapOfSprites.erase(it);
		}
	}
}

void UIManager::deleteString(int index)
{
	std::map<std::string, UIString*>::iterator it;
	bool done = false;
	for (it = mapOfString.begin(); it != mapOfString.end() && !done; it++)
	{
		if (strings[index] == it->second) {
			done = true;
			//delete both
			delete strings[index];
			strings.erase(std::next(strings.begin(), index));
			mapOfString.erase(it);
		}
	}
}

void UIManager::deleteButton(int index)
{
	std::map<std::string, UIButton*>::iterator it;
	bool done = false;
	for (it = mapOfButtons.begin(); it != mapOfButtons.end() && !done; it++)
	{
		if (buttons[index] == it->second) {
			done = true;
			//delete both
			delete buttons[index];
			buttons.erase(std::next(buttons.begin(), index));
			mapOfButtons.erase(it);
		}
	}
}

void UIManager::deleteElement(std::string name)
{
	std::map<std::string, UIElements*>::iterator it;
	it = mapOfSprites.find(name);
	for (int i = 0; i < elements.size(); i++) {
		if (elements[i] == it->second) {
			elements.erase(std::next(elements.begin(), i));
		}
	}
	delete it->second;
	mapOfSprites.erase(it);
}

void UIManager::deleteString(std::string name)
{
	std::map<std::string, UIString*>::iterator it;
	it = mapOfString.find(name);
	for (int i = 0; i < strings.size(); i++) {
		if (strings[i] == it->second) {
			strings.erase(std::next(strings.begin(), i));
		}
	}
	delete it->second;
	mapOfString.erase(it);
}

void UIManager::deleteButton(std::string name)
{
	std::map<std::string, UIButton*>::iterator it;
	it = mapOfButtons.find(name);
	for (int i = 0; i < strings.size(); i++) {
		if (buttons[i] == it->second) {
			buttons.erase(std::next(buttons.begin(), i));
		}
	}
	delete it->second;
	mapOfButtons.erase(it);
}

void UIManager::update()
{
	for (size_t i = 0; i < buttons.size(); i++) {
		buttons[i]->update();
	}
}

void UIManager::draw()
{

	UINT offset = 0;
	static UINT strid = sizeof(UIVertex);
	gfx->get_IMctx()->IASetVertexBuffers(0, 1, &this->vertexBuffer, &strid, &offset);
	gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	gfx->get_IMctx()->IASetInputLayout(inputLayout);
	gfx->get_IMctx()->VSSetShader(vShader, nullptr, 0);
	gfx->get_IMctx()->PSSetShader(pShader, nullptr, 0);

	for (int i = 0; i < elements.size(); i++) {
		elements[i]->updateConstBuffer(gfx);
		elements[i]->draw(gfx);
	}
	for (int i = 0; i < buttons.size(); i++) {
		buttons[i]->updateConstBuffer(gfx);
		buttons[i]->draw(gfx);
	}
	gfx->get_IMctx()->PSSetShaderResources(0, 1, &font);
	for (int i = 0; i < strings.size(); i++) {
		strings[i]->draw(gfx);
	}
}

void UIManager::init(Graphics*& gfx)
{
	UIVertex vertices[4]{
		UIVertex(0,0,0,1),
		UIVertex(0,1,0,0),
		UIVertex(1,0,1,1),
		UIVertex(1,1,1,0)
	};
	ID3D11Texture2D* tex;
	//if (!CreateTexture("assets/textures/Fonts/ExportedFont.bmp", gfx->getDevice(), tex, font)) {
	if (!CreateTexture("assets/textures/Fonts/TheFont.png", gfx->getDevice(), tex, font)) {
		std::cout << "error cannot load font" << std::endl;
	}

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(vertices) * 3;//TODO : 3 or 4?
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the vertex buffer.
	HRESULT hr = gfx->getDevice()->CreateBuffer(&bufferDesc, &InitData, &vertexBuffer);

	std::string vShaderByteCode[1];
	//load shader
	if (loadVShader("UIVertexShader.cso", gfx->getDevice(), vShader, vShaderByteCode[0]) &&
		loadPShader("UIPixelShader.cso", gfx->getDevice(), pShader)) 
	{
		//continue
	}
	else {
		std::cout << "error cannot load UI shader" << std::endl;
	}


	//create input layout
	D3D11_INPUT_ELEMENT_DESC inputDesc[2] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	CreateInputLayoutOwn(gfx->getDevice(), inputLayout, vShaderByteCode[0], inputDesc, 2);
}
