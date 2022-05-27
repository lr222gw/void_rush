#include "Camera.h"
#include <iostream>
//git

Camera::Camera(Graphics *&gfx, Mouse* mouse, vec3 pos, vec3 rot)
{
	this->Lcbd = gfx->getLightconstbufferforCS();
	this->Vcbd = gfx->getVertexconstbuffer();
	this->mouse = mouse;
	this->mouseSensitivity = 1.0f;
	this->xCamPos = pos.x;
	this->yCamPos = pos.y;
	this->zCamPos = pos.z;

	this->xCamRot = rot.x;
	this->yCamRot = rot.y;
	this->zCamRot = rot.z;
	this->calcFURVectors();

	this->shakeTimerOrig = 0.01f;
	resetShake();
}

Camera::~Camera()
{
}

void Camera::updateCamera() {
	DirectX::XMMATRIX viewMatrix = DirectX::XMMATRIX(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-xCamPos, -yCamPos, -zCamPos, 1.0f
	);
	rotaiton(viewMatrix);


	//just add it to the pixel shader
	movement();
	Vcbd->view.element = viewMatrix;
}
void Camera::updateCamera(float dt)
{	
	DirectX::XMMATRIX viewMatrix = DirectX::XMMATRIX(
		1.0f,0.0f,0.0f,0.0f,
		0.0f,1.0f,0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		-xCamPos,-yCamPos,-zCamPos,1.0f
	);
	rotaiton(viewMatrix);

	
	//just add it to the pixel shader
	movement();
	Vcbd->view.element = viewMatrix;

	//ScreenShake
	if (shake) {
		shakeScreen(dt);
	}

}

vec3 Camera::getPos()
{
	return vec3(xCamPos, yCamPos, zCamPos);
}

vec3 Camera::getRot()
{
	return vec3(xCamRot, yCamRot, 0);
}

void Camera::calcFURVectors()
{
	DirectX::XMMATRIX viewMatrix = DirectX::XMMATRIX(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		-xCamPos, -yCamPos, -zCamPos, 1.0f
	);
	rotaiton(viewMatrix);


	//just add it to the pixel shader
	movement();
	DirectX::XMFLOAT4X4 theViewMatrix;
	DirectX::XMStoreFloat4x4(&theViewMatrix, viewMatrix);
	FUL[0] = vec3(theViewMatrix._13, theViewMatrix._23, theViewMatrix._33);
	FUL[1] = vec3(theViewMatrix._12, theViewMatrix._22, theViewMatrix._32);
	FUL[2] = vec3(theViewMatrix._11, theViewMatrix._21, theViewMatrix._31);
}

vec3 Camera::getForwardVec()
{
	return FUL[0];
}

vec3 Camera::getUpVector()
{
	return FUL[1];
}

vec3 Camera::getRightVector()
{
	return FUL[2];
}

void Camera::getViewFrustoms(vec3 frustoms[])
{
	vec3 nearCenter = getForwardVec() * nearDist;
	vec3 farCenter = getPos() - getForwardVec() * farDist;

	float nearHeight = 2 * tan(fov / 2) * nearDist;
	float farHeight = 2 * tan(fov / 2) * farDist;
	float nearWidth = nearHeight * ratio;
	float farWidth = farHeight * ratio;

	//left, right, up, down 
	frustoms[0] = vec3(nearCenter - getRightVector() * ((float)nearWidth * 0.5f)).Normalize();
	frustoms[1] = vec3(nearCenter + getRightVector() * ((float)nearWidth * 0.5f)).Normalize();
	frustoms[2] = vec3(nearCenter + getUpVector() * ((float)nearHeight * 0.5f)).Normalize();
	frustoms[3] = vec3(nearCenter - getUpVector() * ((float)nearHeight * 0.5f)).Normalize();
}

void Camera::setRotation(vec3 newRot)
{
	this->xCamRot = newRot.x;
	this->yCamRot = newRot.y;
}

void Camera::setPosition(vec3 newpos)
{
	this->xCamPos = newpos.x;
	this->yCamPos = newpos.y;
	this->zCamPos = newpos.z;
}

void Camera::addRotation(vec3 addRot)
{
	xCamRot += addRot.x;
	yCamRot += addRot.y;
}

void Camera::rotateCameraMouse(vec3 Rot, float dt)
{
	xCamRot += Rot.x * (float)dt * mouseSensitivity;
	yCamRot -= Rot.y * (float)dt * mouseSensitivity;
}

void Camera::movePos(vec3 move)
{
	xCamPos += move.x;
	yCamPos += move.y;
	zCamPos += move.z;
}

void Camera::screenShake(float magnitude)
{
	this->shake = true;
	this->magnitude = magnitude/100;
	this->randomShake = (float)(rand() % 2);
	if (this->randomShake == 0)
		this->randomShake = -1;
}

void Camera::setData(float FOVRadians, float viewRatio, float nearDist, float farDist)
{
	this->ratio = viewRatio;
	this->fov = FOVRadians;
	this->nearDist = nearDist;
	this->farDist = farDist;
}

void Camera::rotaiton(DirectX::XMMATRIX& matrix, float xRot, float yRot) {
	XRotation(matrix, xRot);
	YRotation(matrix, yRot);
}

void Camera::rotaiton(DirectX::XMMATRIX &matrix)
{
	XRotation(matrix, xCamRot);
	YRotation(matrix, yCamRot);
}

void Camera::movement()
{
	Lcbd->cameraPos.element[0] = xCamPos;
	Lcbd->cameraPos.element[1] = yCamPos;
	Lcbd->cameraPos.element[2] = zCamPos;
}

void Camera::handleEvent(float dt)
{

}

void Camera::shakeScreen(float dt)
{
	if (shakeTimer > 0) {
		shakeTimer -= dt;
		return;
	}
	resetShakeTimer();
	if (this->shakeCounter > 0) {
		switch (shakeCounter--)
		{
		case 1://Left
			this->xCamRot -= this->magnitude * randomShake;
			break;
		case 2://Right
			this->xCamRot += this->magnitude * randomShake;
			break;
		case 3://Up
			this->yCamRot += this->magnitude * randomShake;
			break;
		case 4://Down
			this->yCamRot -= this->magnitude * randomShake;
			break;
		}
	}
	else {
		resetShake();
	}
}

void Camera::resetShake()
{
	this->shakeCounter = 4;
	resetShakeTimer();
	this->magnitude = 1.0f;
	this->shake = false;
}

void Camera::resetShakeTimer()
{
	this->shakeTimer = this->shakeTimerOrig;
}

void Camera::Translate(float dt)
{
	DirectX::XMStoreFloat3(&translation, DirectX::XMVector3Transform(
		DirectX::XMLoadFloat3(&translation),
		DirectX::XMMatrixRotationRollPitchYaw(yCamRot, xCamRot, 0) *
		DirectX::XMMatrixScaling(1, 1, 1)//this line is not neccessary but I am afraid to break things
	));
	vec2 trans(translation.x, translation.z);
	trans.Normalize();
	xCamPos -= trans.x * movementspeed * dt;
	zCamPos -= trans.y * movementspeed * dt;
}
