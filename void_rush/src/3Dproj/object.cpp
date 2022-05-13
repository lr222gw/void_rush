#include "object.h"
#include "Graphics.h"
#include <array>
#include "otherHelps.h"

object::object():
    scaleMat(
        scale.x, 0.0f, 0.0f, 0.0f,
        0.0f, scale.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    ),
    transMat(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        pos.x, pos.y, pos.z, 1.0f
    ), pointMat(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        rPoint.x, rPoint.y, rPoint.z, 1.0f
    )
{
    model = nullptr;
    Vg_pConstantBuffer = nullptr;
}

object::object(vec3 pos):
    rot(0,0,0),
    scale(1,1,1),
    rPoint(0,0,0),
    scaleMat(
        scale.x, 0.0f, 0.0f, 0.0f,
        0.0f, scale.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    ),
    transMat(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        pos.x, pos.y, pos.z, 1.0f
    ), pointMat(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        rPoint.x, rPoint.y, rPoint.z, 1.0f
    )
{
    this->pos = pos;
    Vg_pConstantBuffer = nullptr;
}

object::~object()
{
    if (Vg_pConstantBuffer != nullptr) {
        Vg_pConstantBuffer->Release();
    };
}

vec3 object::getPos()
{
    return this->pos;
}

const vec3 object::getRot()
{
    return this->rot;
}

vec3 object::getScale()
{
    return this->scale;
}

const vec3 object::getPoint()
{
    return this->rPoint;
}

void object::setPos(vec3 pos)
{
    this->pos = pos;
}

void object::setRot(vec3 rot)
{
    this->rot = rot;
}

void object::setScale(vec3 scale)
{
    this->scale = scale;
}

void object::setPoint(vec3 point)
{
    this->rPoint = point;
}

void object::movePoint(vec3 point)
{
    this->rPoint = this->rPoint + point;
}

void object::movePos(vec3 pos)
{
    this->pos = this->pos + pos;
}

void object::addRot(vec3 rot)
{
    this->rot = this->rot + rot;
}

void object::addScale(vec3 scale)
{
    this->scale = this->scale + scale;
}

void object::update(float dt)
{
    _lastPosition = this->pos;
}

ID3D11Buffer*& object::getVertexConstBuffer()
{
    return this->Vg_pConstantBuffer;
}

void object::getKdKa(float kd[4], float ka[4])
{
    for (int i = 0; i < 4; i++) {
    }
}

float& object::getxRot()
{
    return this->rot.y;
}

float& object::getyRot()
{
    return this->rot.x;
}

float& object::getzRot()
{
    return this->rot.z;
}

void object::setModel(ModelObj* m)
{
    this->model = m;
}

float& object::getxPos() {
    return this->pos.x;
}
float& object::getyPos()
{
    return this->pos.y;
}
float& object::getzPos() {
    return this->pos.z;
}

void object::updateVertexShader(Graphics*& gfx)
{
    DirectX::XMMATRIX rot(DirectX::XMMatrixRotationRollPitchYaw(this->getRot().x, this->getRot().y, this->getRot().z));

    transMat = DirectX::XMMatrixSet(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        pos.x, pos.y, pos.z, 1.0f
    );

    scaleMat = DirectX::XMMatrixSet(
        scale.x, 0.0f, 0.0f, 0.0f,
        0.0f, scale.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    pointMat = DirectX::XMMatrixSet(1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        rPoint.x, rPoint.y, rPoint.z, 1.0f
    );

    gfx->getVertexconstbuffer()->transform.element = pointMat * (scaleMat * rot * transMat);

    //changing vertex Shader cBuffer
    D3D11_MAPPED_SUBRESOURCE resource;

    gfx->get_IMctx()->Map(this->getVertexConstBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
    memcpy(resource.pData, gfx->getVertexconstbuffer(), sizeof(Vcb));
    gfx->get_IMctx()->Unmap(this->getVertexConstBuffer(), 0);
    ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
}

void object::updatePixelShader(Graphics*& gfx)
{
    for (int i = 0; i < model->getMehses().size(); i++) {
        model->getMehses()[i].updatePS(gfx);
    }
}

void object::updateMatrix()
{
    transMat = DirectX::XMMatrixSet(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        pos.x, pos.y, pos.z, 1.0f
    );

    scaleMat = DirectX::XMMatrixSet(
        scale.x, 0.0f, 0.0f, 0.0f,
        0.0f, scale.y, 0.0f, 0.0f,
        0.0f, 0.0f, scale.z, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );

    pointMat = DirectX::XMMatrixSet(1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        rPoint.x, rPoint.y, rPoint.z, 1.0f
    );
}

