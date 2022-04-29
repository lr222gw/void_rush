#pragma once
#include "Mesh.h"

class Graphics;

class ModelObj {
public:
	ModelObj(const std::string& ModelFile, Graphics*& gfx, ID3D11ShaderResourceView** def);
	ModelObj();
	void init(const std::string& ModelFile, Graphics*& gfx, ID3D11ShaderResourceView** def);
	void init(aiScene* scene, Graphics*& gfx, ID3D11ShaderResourceView** def);
	virtual ~ModelObj();
	void draw(Graphics*& gfx, bool shadowmap = false);
	std::vector<MeshObj> &getMehses();
	std::vector<Material*> &getMatrial();
	vec3* getBoundingBox();
private:
	bool loadModel(const std::string& ModelFile);
	std::vector<MeshObj> mMeshes;
	std::vector<Material*> matrial;
	vec3 boxSize[2];
};