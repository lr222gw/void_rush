#include "Model.h"
#include "ReadObjFile.h"

ModelObj::ModelObj(const std::string& ModelFile, Graphics*& gfx, ID3D11ShaderResourceView** def)
{
	getMatrialFromFileObj(ModelFile, matrial, gfx, def);
	if (!readObjFile(mMeshes, ModelFile, matrial, gfx, boxSize)) {
		std::cout << "couldn't load " << ModelFile << std::endl;
		MessageBox(nullptr, L"sorry couldn't load file look in terminal for more info", L"ERROR", MB_ICONWARNING | MB_OK);
	}
}

ModelObj::ModelObj()
{
}

void ModelObj::init(const std::string& ModelFile, Graphics*& gfx, ID3D11ShaderResourceView** def)
{
	std::cout << ModelFile.substr(ModelFile.length() - 3,3) << std::endl;
	if (ModelFile.substr(ModelFile.length() - 3, 3) == "obj") {
		getMatrialFromFileObj(ModelFile, matrial, gfx, def);
	}
	else {
		getMatrialFromFile(ModelFile, matrial, gfx, def);
	}
	if (!readObjFile(mMeshes, ModelFile, matrial, gfx, boxSize)) {
		std::cout << "couldn't load " << ModelFile << std::endl;
		MessageBox(nullptr, L"sorry couldn't load file look in terminal for more info", L"ERROR", MB_ICONWARNING | MB_OK);
	}
}
void ModelObj::init(aiScene* scene, Graphics*& gfx, ID3D11ShaderResourceView** def)
{

	bool oneTex = false;
	matrial.resize(scene->mNumMaterials);	
	for (int i = 0; i < (int)scene->mNumMaterials; i++) {
		matrial[i] = new Material(def);
		const aiMaterial* pMaterial = scene->mMaterials[i];
		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString Path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string thePath = getPathfrom(Path.data, "textures");
				matrial[i]->loadTexture(thePath, gfx, 0, def);
			}
		}
		if (pMaterial->GetTextureCount(aiTextureType_NORMALS) > 0) {
			aiString Path;
			if (pMaterial->GetTexture(aiTextureType_NORMALS, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
				std::string thePath(Path.data);
				matrial[i]->loadTexture(thePath, gfx, 3, def);
			}
		}
		//load kd, ka, ks, ns
		aiColor3D kd,ka,ks;
		float ns;
		pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ka);
		pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, ks);
		pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, kd);
		pMaterial->Get(AI_MATKEY_SHININESS, ns);

		matrial[i]->Ka[0] = ka.r;
		matrial[i]->Ka[1] = ka.g;
		matrial[i]->Ka[2] = ka.b;
		matrial[i]->Kd[0] = kd.r;
		matrial[i]->Kd[1] = kd.g;
		matrial[i]->Kd[2] = kd.b;
		matrial[i]->Ks[0] = ks.r;
		matrial[i]->Ks[1] = ks.g;
		matrial[i]->Ks[2] = ks.b;
		matrial[i]->Ns = 0;

	}

	if (!readObjFromScene(scene, mMeshes, matrial, gfx, boxSize)) {
		std::cout << "couldn't load the scene" << std::endl;
		MessageBox(nullptr, L"sorry couldn't load assimp scene look in terminal for more info", L"ERROR", MB_ICONWARNING | MB_OK);
	}
}

ModelObj::~ModelObj()
{
	for (int i = 0; i < mMeshes.size(); i++) {
		mMeshes[i].begone();
	}
}

void ModelObj::draw(Graphics*& gfx, bool shadowmap)
{
	for (int i = 0; i < mMeshes.size(); i++) {
		if (!shadowmap) {
			this->mMeshes[i].SetShader(gfx->get_IMctx());
			gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		}
		else{
			gfx->get_IMctx()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			gfx->get_IMctx()->IASetInputLayout(gfx->getInputLayout()[1]);
			gfx->get_IMctx()->VSSetShader(gfx->getVS()[3], nullptr, 0);
		}
		mMeshes[i].draw(gfx->get_IMctx());
	}
}

void ModelObj::drawRaw(Graphics*& gfx)
{
	for (int i = 0; i < mMeshes.size(); i++) {
		mMeshes[i].drawRaw(gfx->get_IMctx());
	}
}

std::vector<MeshObj> &ModelObj::getMehses()
{
	return this->mMeshes;
}

std::vector<Material*>& ModelObj::getMatrial()
{
	return this->matrial;
}

vec3* ModelObj::getBoundingBox()
{
	return this->boxSize;
}

bool ModelObj::loadModel(const std::string& ModelFile)
{
	return false;
}