#include "skybox.h"

SkyBox::SkyBox(ModelObj* file, Graphics*& gfx, vec3 pos, ID3D11ShaderResourceView* textureCube) :
	GameObject(file, gfx, pos, vec3(0,0,0), vec3(10,10,10))
{
	for (int i = 0; i < model->getMehses().size(); i++) {
		model->getMehses()[i].getMatrial()->texSRVPS[0] = textureCube;
	}
}

SkyBox::~SkyBox()
{
}

void SkyBox::draw(Graphics*& gfx)
{
	gfx->get_IMctx()->OMSetRenderTargets(1, &gfx->getRenderTarget(), nullptr);
	//set pixelshader and vertexshader
	gfx->get_IMctx()->VSSetShader(gfx->getVS()[2], nullptr ,0);
	gfx->get_IMctx()->PSSetShader(gfx->getPS()[3], nullptr ,0);

	gfx->get_IMctx()->VSSetConstantBuffers(0, 1, &this->getVertexConstBuffer());
	//draw
	for (int i = 0; i < model->getMehses().size(); i++) {
		model->getMehses()[i].draw(gfx->get_IMctx());
	}
	gfx->get_IMctx()->OMSetRenderTargets(1, &gfx->getRenderTarget(), gfx->getDepthStencil());
}

void SkyBox::update(vec3 position)
{
	GameObject::setPos(position);
}


