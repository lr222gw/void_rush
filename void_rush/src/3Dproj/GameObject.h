#pragma once
#include <vector>
#include <string>
#include "vertex.h"
#include "object.h"
#include <DirectXCollision.h>
#include "Model.h"

class GameObject : public object{
public:
	GameObject(ModelObj*file, Graphics*& gfx, vec3 pos = vec3(0, 0, 0), vec3 rot = vec3(0, 0, 0), vec3 scale = vec3(1, 1, 1), std::string otherTex = "");
	virtual ~GameObject();
	
	void draw(Graphics*& gfx, bool sm = false);

	vec3 getlastPosition();

	//update vertexConstBuffer on object
	void Updateshaders(Graphics*& gfx, bool vertex = true);

	//gives 2 points 0 = the lowest in x,y and z while 1 is the highest in x,y and zdimensions
	void getBoundingBox(DirectX::XMFLOAT4 theReturn[]);
	void getBoundingBoxFromObject(DirectX::XMFLOAT4 theReturn[]);

	DirectX::BoundingBox getDirectXBoundingBoxFromModel();
	
	void setBoundingBox(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 sizes);

	//see if this object has beened drawned before(used for quad tree)
	const bool isDrawed();

	//clear that the object has not been drawned before
	void clearDrawed();

	void setWeight(float newWeight);
	float getWeight();

	//DEBUG
	vec3 getWidthHeightDepth();
protected:
	ModelObj* model;
private:
	friend class ImguiManager;
	bool tess;
	bool drawed;
	DirectX::XMFLOAT3 BBpos;
	DirectX::XMFLOAT3 BBsizes;
	void setHeightWidthDepth();
	vec3 WHD;

	/*Interaction Start*/
	bool used;
public:
	void Use();
	bool isUsed();
	/*Interaction End*/
	float weight;//see what object should move in collision
};