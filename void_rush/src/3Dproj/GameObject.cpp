#include "GameObject.h"

GameObject::GameObject(ModelObj*file, Graphics*& gfx, vec3 pos, vec3 rot, vec3 scale, std::string otherTex)
{
	this->setPos(pos);
	this->setScale(scale);
	this->setRot(rot);
	this->model = file;
	CreateVertexConstBuffer(gfx, this->getVertexConstBuffer());
	object::setModel(model);
	drawed = false;
	this->setHeightWidthDepth();

	/*Interact*/
	used = false;
	this->weight = 50.f;
}

GameObject::~GameObject()
{
	
}

void GameObject::draw(Graphics *&gfx, bool sm)
{
	drawed = true;
	gfx->get_IMctx()->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	model->draw(gfx, sm);
}

void GameObject::drawRaw(Graphics*& gfx)
{
	drawed = true;
	gfx->get_IMctx()->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	model->drawRaw(gfx);
}

vec3 GameObject::getlastPosition()
{
	return this->_lastPosition;
}


void GameObject::getBoundingBox(DirectX::XMFLOAT4 theReturn[])
{
	//rotations
	DirectX::XMMATRIX rot(DirectX::XMMatrixRotationRollPitchYaw(this->getRot().x, this->getRot().y, this->getRot().z));

	vec3 high(model->getBoundingBox()[1].x, model->getBoundingBox()[1].y, model->getBoundingBox()[1].z);
	vec3 low(model->getBoundingBox()[0].x, model->getBoundingBox()[0].y, model->getBoundingBox()[0].z);
	
	DirectX::XMMATRIX modelMatrix = (scaleMat * rot * transMat);

	DirectX::XMVECTOR bbPoints[8] = {
		{high.x,high.y,high.z,1},
		{high.x,high.y,low.z,1},
		{high.x,low.y,high.z,1},
		{high.x,low.y,low.z,1},
		{low.x,high.y,high.z,1},
		{low.x,high.y,low.z,1},
		{low.x,low.y,high.z,1},
		{low.x,low.y,low.z,1}
	};
	
	bbPoints[0] = DirectX::XMVector4Transform(bbPoints[0], modelMatrix);
	DirectX::XMStoreFloat4(&theReturn[0], bbPoints[0]);
	DirectX::XMStoreFloat4(&theReturn[1], bbPoints[0]);

	for (int i = 1; i < 8; i++) {
		bbPoints[i] = DirectX::XMVector4Transform(bbPoints[i], modelMatrix);
		DirectX::XMFLOAT4 tester;
		DirectX::XMStoreFloat4(&tester, bbPoints[i]);

		if (tester.x > theReturn[1].x) {
			theReturn[1].x = tester.x;
		}
		else if(tester.x < theReturn[0].x) {
			theReturn[0].x = tester.x;
		}
		if (tester.y > theReturn[1].y) {
			theReturn[1].y = tester.y;
		}
		else if (tester.y < theReturn[0].y) {
			theReturn[0].y = tester.y;
		}
		if (tester.z > theReturn[1].z) {
			theReturn[1].z = tester.z;
		}
		else if (tester.z < theReturn[0].z) {
			theReturn[0].z = tester.z;
		}
	}
}

void GameObject::getBoundingBoxFromObject(DirectX::XMFLOAT4 theReturn[])
{
	theReturn[0].x = BBpos.x - BBsizes.x + this->getPos().x;
	theReturn[0].y = BBpos.y - BBsizes.y + this->getPos().y;
	theReturn[0].z = BBpos.z - BBsizes.z + this->getPos().z;

	theReturn[1].x = BBpos.x + BBsizes.x + this->getPos().x;
	theReturn[1].y = BBpos.y + BBsizes.y + this->getPos().y;
	theReturn[1].z = BBpos.z + BBsizes.z + this->getPos().z;
}

DirectX::BoundingBox GameObject::getDirectXBoundingBoxFromModel()
{
	//rotations
	DirectX::XMMATRIX rot(DirectX::XMMatrixRotationRollPitchYaw(this->getRot().x, this->getRot().y, this->getRot().z));

	DirectX::XMVECTOR low = { model->getBoundingBox()[1].x ,model->getBoundingBox()[1].y ,model->getBoundingBox()[1].z , 1 };
	DirectX::XMVECTOR high = { model->getBoundingBox()[0].x ,model->getBoundingBox()[0].y ,model->getBoundingBox()[0].z , 1 };

	vec3 themid = this->getPos() + model->getBoundingBox()[0];
	DirectX::XMFLOAT3 midpoint(
		themid.toXMFloat3()
		);//mid pos
	DirectX::XMFLOAT3 sizes(
		model->getBoundingBox()[1].toXMFloat3()
	);//sizes pos
	return DirectX::BoundingBox(midpoint, sizes);
}

void GameObject::setBoundingBox(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 sizes)
{
	this->BBpos = pos;
	this->BBsizes = sizes;
}

const bool GameObject::isDrawed()
{
	return this->drawed;
}

void GameObject::clearDrawed()
{
	this->drawed = false;
}

void GameObject::setWeight(float newWeight)
{
	this->weight = newWeight;
}

float GameObject::getWeight()
{
	return this->weight;
}

void GameObject::update(float dt)
{
	object::update(dt);
}

vec3 GameObject::getWidthHeightDepth()
{
	return this->WHD;
}

void GameObject::setHeightWidthDepth()
{
	DirectX::XMFLOAT4 theReturn[2];
	//rotations
	this->updateMatrix();
	getBoundingBox(theReturn);

	WHD.x = theReturn[1].x - theReturn[0].x;
	WHD.y = theReturn[1].y - theReturn[0].y;
	WHD.z = theReturn[1].z - theReturn[0].z;

}


void GameObject::Updateshaders(Graphics*& gfx, bool vertex)
{
	if (vertex) {
		this->updateVertexShader(gfx);
	}

	D3D11_MAPPED_SUBRESOURCE resource;

	gfx->get_IMctx()->Map(this->getVertexConstBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, gfx->getVertexconstbuffer(), sizeof(Vcb));
	gfx->get_IMctx()->Unmap(this->getVertexConstBuffer(), 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
}

/*Interaction*/
void GameObject::Use()
{
	if (!used)
		used = true;
	else {
		used = false;
	}
}


bool GameObject::isUsed()
{
	return used;
}
