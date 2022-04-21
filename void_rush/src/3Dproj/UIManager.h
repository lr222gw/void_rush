#pragma once
#include "ResourceManager.h"
#include "Graphics.h"
#include <vector>
#include "UIString.h"
#include "UISprite.h"
#include <string>
#include "plHelper.h"

class UIManager {
public:
	UIManager(ResourceManager*& rm, Graphics*& gfx);
	virtual ~UIManager();
	void createUIString(std::string str, vec2 pos, vec2 size, std::string name = "");
	void createUISprite(std::string rmsprite, vec2 pos, vec2 size, std::string name = "");
	UIElements* getElements(int index);
	UIString* getStringElement(int index);
	UIElements* getElements(std::string key);
	UIString* getStringElement(std::string key);
	void draw();
private:
	std::vector<UIElements*> elements;
	std::vector<UIString*> strings;
	std::map<std::string, UIString*> mapOfString;
	std::map<std::string, UIElements*> mapOfSprites;
	ResourceManager* rm;
	Graphics* gfx;
	void init(Graphics*& gfx);
private://d3dshit
	ID3D11InputLayout* inputLayout;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3D11ShaderResourceView* font;

	ID3D11Buffer* vertexBuffer;
};