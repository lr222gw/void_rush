#pragma once

#include "3DLetters.h"

class Letters3DHandler {
public:
	Letters3DHandler(ResourceManager*& rm, Graphics*& gfx);
	virtual ~Letters3DHandler();
	void draw();
	void update(vec3 camPos = vec3(0,0,0));
	void createText(std::string str, vec3 pos, vec2 size, std::string name = "");
	void removeText(std::string key);
	void removeText(int index);
	void editText(std::string key, std::string text);
	void removeAllText();
	Letters3D* getText(std::string key);
	Letters3D* getText(int index);
private:
	std::vector<Letters3D*> strings;
	std::map<std::string, Letters3D*> mapOfLetters;
	ResourceManager* rm;
	Graphics* gfx;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	ID3D11ShaderResourceView* font;
};