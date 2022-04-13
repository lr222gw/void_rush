#include "UIManager.h"

UIManager::UIManager(ResourceManager*& rm, Graphics*& gfx)
{
	this->gfx = gfx;
	this->rm = rm;
	init(gfx);
}

UIManager::~UIManager()
{
	for (int i = 0; i < elements.size(); i++) {
		delete elements[i];
	}
}

void UIManager::createUIString(std::string str, vec2 pos, vec2 size)
{
	strings.push_back(UIString());
}

void UIManager::createUISprite(std::string rmsprite, vec2 pos, vec2 size)
{
	elements.push_back(new UISprite(rm->getSprite(rmsprite, gfx), pos, size));
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
		elements[i]->draw(gfx);
	}
	for (int i = 0; i < strings.size(); i++) {
		strings[i].draw(gfx);
	}
}

void UIManager::init(Graphics*& gfx)
{
	UIVertex vertices[4]{
		UIVertex(0,0,0,0),
		UIVertex(0,1,0,1),
		UIVertex(1,0,1,0),
		UIVertex(1,1,1,1)
	};

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
