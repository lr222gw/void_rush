#include "ResourceManager.h"

ResourceManager::ResourceManager(Graphics*& gfx)
{
	loadThings(gfx);
}

ResourceManager::~ResourceManager()
{
	for (int i = 0; i < 4; i++) {
		if (def[i] != nullptr) {
			def[i]->Release();
		}
	}
	delete[] def;
	
	if (Fire != nullptr) {
		Fire->Release();
	}
	
	std::map<std::string, ModelObj*>::iterator it;
	for (it = Models.begin(); it != Models.end(); it++) {
		delete it->second;
	}
	//delete sprites TODO
	std::map<std::string, ID3D11ShaderResourceView*>::iterator sit;
	for (sit = Sprites.begin(); sit != Sprites.end(); sit++) {
		sit->second->Release();
	}
	std::map<std::string, ID3D11ShaderResourceView*>::iterator csit;
	for (csit = CubeSprites.begin(); csit != CubeSprites.end(); csit++) {
		csit->second->Release();
	}
	delete defMatrial;
}

void loadWithThread(threadInfo thredData)
{
	thredData.model->init(thredData.name, thredData.gfx, thredData.def);
}

void ResourceManager::loadThings(Graphics*& gfx)
{
	//default textures
	def = new ID3D11ShaderResourceView * [4];
	//diffuse
	if (!CreateTexture("assets/textures/Default/KdDef.png", gfx->getDevice(), gfx->getTexture(), def[0])) {
		cantLoad(L"kddef cant load");
	}
	//normal
	if (!CreateTexture("assets/textures/Default/NormalDef2.png", gfx->getDevice(), gfx->getTexture(), def[1])) {
		cantLoad(L"NDef cant load");
	}
	//ambient
	if (!CreateTexture("assets/textures/Default/KaDef2.png", gfx->getDevice(), gfx->getTexture(), def[2])) {
		cantLoad(L"AmientDef cant load");
	}
	//specular//orkar inte
	def[3] = def[2];
	
	if (!CreateTexture("assets/textures/Fire.png", gfx->getDevice(), gfx->getTexture(), Fire)) {
		cantLoad(L"Fire cant load");
	}
	def[3] = nullptr;
	//mesh

	std::string names[] = {
		"Camera.obj"
	};
	for (int i = 0; i < _countof(names); i++) {
		ModelObj* model = new ModelObj();
		model->init("assets/obj/" + names[i], gfx, def);
		Models.insert(std::make_pair(names[i], model));
	}
	for (int i = 0; i < _countof(names); i++) {
		int sizeOfMatrials = (int)Models.find(names[i])->second->getMatrial().size();
		ModelObj* model = Models.find(names[i])->second;
		for (int p = 0; p < sizeOfMatrials; p++) {
			TC::GetInst().add(model->getMatrial()[p]);
		}
	}
	
	std::cout << "resource manager is done loading" << std::endl;
}

ModelObj* ResourceManager::get_Models(std::string key)
{
	return Models.find(key)->second;
}

ModelObj* ResourceManager::get_Models(std::string key, Graphics*& gfx)
{
	if (Models.find(key) == Models.end()) {
		//its not found try to add it to the library
		ModelObj* model = new ModelObj();
		model->init("assets/obj/" + key, gfx, def);
		Models.insert(std::make_pair(key, model));

		
		for (int p = 0; p < model->getMatrial().size(); p++) {
			TC::GetInst().add(model->getMatrial()[p]);
		}
	}
	//else we return it
	return Models.find(key)->second;
}

ModelObj* ResourceManager::load_map_scene(aiScene* scene,std::string key, Graphics*& gfx)
{
	//its not found try to add it to the library
	ModelObj* model = new ModelObj();
	//model->init("assets/obj/" + key, gfx, def);
	
	model->init(scene, gfx, def);

	auto iterator = Models.find(key);
	if ( iterator != Models.end() ) {
		delete iterator->second;
		Models.erase(iterator);		
	}
	Models.insert(std::make_pair(key, model));


	for (int p = 0; p < model->getMatrial().size(); p++) {
		TC::GetInst().add(model->getMatrial()[p]);
	}
	return Models.find(key)->second;
}

ID3D11ShaderResourceView** ResourceManager::getDef()
{
	return this->def;
}

ID3D11ShaderResourceView* ResourceManager::getFire()
{
	return this->Fire;
}

ID3D11ShaderResourceView*& ResourceManager::getSprite(std::string textureFile, Graphics*& gfx)
{
	ID3D11Texture2D* tex;
	if (Sprites.find(textureFile) == Sprites.end()) {
		//its not found try to add it to the library
		ID3D11ShaderResourceView* SRVptr;
		if (!CreateTexture(textureFile, gfx->getDevice(), tex, SRVptr)) {
			std::cout << "stop" << std::endl;
		}
		Sprites.insert(std::make_pair(textureFile, SRVptr));
	}
	//else we return it
	return Sprites.find(textureFile)->second;
}

ID3D11ShaderResourceView*& ResourceManager::getSpriteCube(std::string textureFiles[], Graphics*& gfx)
{
	ID3D11Texture2D* tex;
	if (CubeSprites.find(textureFiles[0]) == CubeSprites.end()) {
		//its not found try to add it to the library
		ID3D11ShaderResourceView* SRVptr;
		if (!CreateTextureCube(textureFiles, gfx->getDevice(), tex, SRVptr)) {
			std::cout << "stop" << std::endl;
		}
		CubeSprites.insert(std::make_pair(textureFiles[0], SRVptr));
	}
	//else we return it
	return CubeSprites.find(textureFiles[0])->second;
}

void ResourceManager::addMaterialToTrashCollector(ModelObj* model)
{
	for (int i = 0; i < model->getMatrial().size(); i++) {
		TC::GetInst().add(model->getMatrial()[i]);
	}
}

void ResourceManager::cantLoad(LPCWSTR theerror)
{
	MessageBox(nullptr, theerror, L"ERROR", MB_ICONWARNING | MB_OK);
}
