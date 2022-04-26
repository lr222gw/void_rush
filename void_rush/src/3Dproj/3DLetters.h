#pragma once
#include "GameObject.h"
#include <string>
#include "ResourceManager.h"

class Letters3D {
public:
	Letters3D(std::string str, vec3 pos, ResourceManager* rm, Graphics*& gfx, vec2 size);
	virtual ~Letters3D();
	void draw(Graphics*& gfx);
	void update(Graphics*& gfx, vec3 camPos);
	void move(vec3 pos);
	void setPos(vec3 pos);
private:
	struct LetCB : public CB {
		struct {
			float element[4];
		}UV;
	};
	ID3D11ShaderResourceView* font;
	std::vector<float> u;
	std::vector<float> v;
	void symbol_to_uv(char symbol);
	std::vector<GameObject*> letters;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	LetCB bufferData;
	ID3D11Buffer* uvBuffer;
	vec3 pos;
	
};