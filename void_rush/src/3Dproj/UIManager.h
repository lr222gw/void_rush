#pragma once
#include "ResourceManager.h"
#include "Graphics.h"
#include <vector>
#include "UIString.h"
#include "UIButton.h"
#include <string>
#include "plHelper.h"

class UIManager {
public:
	UIManager(ResourceManager*& rm, Graphics*& gfx);
	virtual ~UIManager();
	void createUIString(std::string str, vec2 pos, vec2 size, std::string name = "");
	void createUISprite(std::string rmsprite, vec2 pos, vec2 size, std::string name = "");
	void createUIButton(std::string rmsprite, Mouse* mouse, vec2 pos = vec2(0, 0), vec2 size = vec2(0, 0), std::string name = "");
	UIElements* getElements(int index);
	UIString* getStringElement(int index);
	UIElements* getElements(std::string key);
	UIString* getStringElement(std::string key);
	UIButton* getButton(std::string key);
	UIButton* getButton(int index);
	void deleteElement(int index);
	void deleteString(int index);
	void deleteButton(int index);
	void deleteElement(std::string name);
	void deleteString(std::string name);
	void deleteButton(std::string name);
	void draw();
private:
	std::vector<UIElements*> elements;
	std::vector<UIString*> strings;
	std::vector<UIButton*> buttons;
	std::map<std::string, UIString*> mapOfString;
	std::map<std::string, UIElements*> mapOfSprites;
	std::map<std::string, UIButton*> mapOfButtons;
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