#include "3DLettersHandler.h"

Letters3DHandler::Letters3DHandler(ResourceManager*& rm, Graphics*& gfx):
	rm(rm),gfx(gfx)
{
	ID3D11Texture2D* tex;
	if (!CreateTexture("assets/textures/Fonts/TheFont.png", gfx->getDevice(), tex, font)) {
		std::cout << "error cannot load font" << std::endl;
	}
	std::string a;
	loadVShader("3DTextVS.cso", gfx->getDevice(), vShader, a);
	loadPShader("3DTextPS.cso", gfx->getDevice(), pShader);
}

Letters3DHandler::~Letters3DHandler()
{
	vShader->Release();
	pShader->Release();
	font->Release();
	for (size_t i = 0; i < strings.size(); i++) {
		delete strings[i];
	}
}

void Letters3DHandler::draw()
{
	gfx->get_IMctx()->PSSetShaderResources(0, 1, &font);
	gfx->get_IMctx()->PSSetShader(pShader, nullptr, 0);
	gfx->get_IMctx()->VSSetShader(vShader, nullptr, 0);
	for (size_t i = 0; i < strings.size(); i++) {
		strings[i]->draw(this->gfx);
	}
}

void Letters3DHandler::update(vec3 camPos)
{
	for (size_t i = 0; i < strings.size(); i++) {
		strings[i]->update(gfx, camPos);
	}
}

void Letters3DHandler::createText(std::string str, vec3 pos, vec2 size, std::string name)
{
	Letters3D* string = new Letters3D(str, pos, rm, gfx, size);
	strings.push_back(string);
	if (name != "") {
		mapOfLetters.insert(std::make_pair(name, string));
	}

}

void Letters3DHandler::removeText(std::string key)
{
	std::map<std::string, Letters3D*>::iterator it;
	it = mapOfLetters.find(key);
	for (int i = 0; i < strings.size(); i++) {
		if (strings[i] == it->second) {
			strings.erase(std::next(strings.begin(), i));
		}
	}
	delete it->second;
	mapOfLetters.erase(it);
}

void Letters3DHandler::removeText(int index)
{
	std::map<std::string, Letters3D*>::iterator it;
	bool done = false;
	for (it = mapOfLetters.begin(); it != mapOfLetters.end() && !done; it++)
	{
		if (strings[index] == it->second) {
			done = true;
			//delete both
			delete strings[index];
			strings.erase(std::next(strings.begin(), index));
			mapOfLetters.erase(it);
		}
	}
}

void Letters3DHandler::removeAllText()
{
	for (size_t i = 0; i < strings.size(); i++)
	{
		delete strings[i];
	}
	strings.clear();

	mapOfLetters.clear();
}

Letters3D* Letters3DHandler::getText(std::string key)
{
	return mapOfLetters.find(key)->second;
}

Letters3D* Letters3DHandler::getText(int index)
{
	return strings[index];
}
