#include "Graphics.h"
#include "imguiManager.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

ID3D11Buffer*& Graphics::getConstBuffers(int i)
{
	switch (i)
	{
	case 0:
		return this->Pg_pConstantBuffer;
		break;
	case 1:
		return this->camConstBuffer;
		break;
	default:
		return this->Pg_pConstantBuffer;
		break;
	}
}


void Graphics::setProjection(int flag, float fov)
{
	//setting projection matrix
	switch (flag)
	{
	case 0://normal
		vcbd.projection.element = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(this->fov), ratio, nearPlane, farPlane);
		break;
	case 1://orthographic
		vcbd.projection.element = DirectX::XMMatrixOrthographicLH(50, 50, nearPlane, farPlane);
		break;
	case 2://6:6
		vcbd.projection.element = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(this->fov), 1, nearPlane, farPlane);
		break;
	case 3://6:6
		vcbd.projection.element = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), ratio, nearPlane, farPlane);
		break;
	default:
		vcbd.projection.element = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(fov), ratio, nearPlane, farPlane);
		break;
	}
}

void Graphics::setFov(float fov)
{
	this->fov = fov;
	setProjection(0, fov);
}

void Graphics::CreateBlendState(int wBlend, bool transparance) {
	D3D11_BLEND_DESC bd = {};
	if (transparance) {
		bd.RenderTarget[0].BlendEnable = TRUE;
		bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	else {
		bd.RenderTarget[0].BlendEnable = FALSE;
		bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	device->CreateBlendState(&bd, &bs[wBlend]);
}

Graphics::Graphics(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow, Mouse*& mouse) :
	speed(1.5f)
{
	WIDTH = resolutions[settingsSingleTon::GetInst().getSettings().resolution][0];
	HEIGHT = resolutions[settingsSingleTon::GetInst().getSettings().resolution][1];
	fov = 45.f;
	ratio = 16.f / 9.f;
	farPlane = 2000.f;
	nearPlane = 0.1f;
	nrOfObject = 0;
	Pg_pConstantBuffer = nullptr;
	
	inputLayout = new ID3D11InputLayout * [2]{nullptr, nullptr};
	
	vShader = new ID3D11VertexShader * [5]{ nullptr, nullptr, nullptr };//4 is used
	gShader = new ID3D11GeometryShader * [5]{ nullptr, nullptr };//2 is used
	pShader = new ID3D11PixelShader * [5] { nullptr, nullptr,nullptr, nullptr };//4 is used
	
	//setting normal value for pcbd
	this->LCBG.lightColor = { 1,1,1,0 };
	this->LCBG.cameraPos = { 0,0,1,1 };
	for (int i = 0; i < 4; i++) {
		for (int o = 0; o < 6; o++) {
			this->LCBG.lightPos.element[o][i] = 255;
		}
	}
	
	this->pcbd.ka = { 0.5f,0.5f,0.5f,1 };
	this->pcbd.kd = { 1.f,1.f,1.f,0 };
	this->pcbd.ks = {1.f,1.f,1.f,0};
	
	//setting matrixes
	setProjection();
	//if delete this happens it will get an error and program will stop working(I want this to happen when I debug)

	windowClass.Initialize(hInstance, "a", "a", WIDTH, HEIGHT);
	
	ImGui_ImplWin32_Init(windowClass.getRenderWindow().getHandle());
	if (!SetupD3D11((UINT)this->getClientWH().x, (UINT)this->getClientWH().y, windowClass.getRenderWindow().getHandle(), device, immediateContext, swapChain, renderTarget, dsTexture, dsView, viewPort, pRS))
	{
		//std::cerr << "cant set up" << std::endl;
		delete this;
	}
	if (!SetupPipeline(device, vShader, pShader, gShader, inputLayout, tex, sampler))
	{
		std::cerr << "cant set up" << std::endl;
		delete this;
	}
	bs = new ID3D11BlendState * [2];
	this->CreateBlendState(0, false);
	this->CreateBlendState(1, true);
	UINT a = 0xFFFFFFFFu;
	immediateContext->OMSetBlendState(bs[0], nullptr, 0xFFFFFFFFu);
	
	//set settings up
	immediateContext->PSSetSamplers(0, 1, &sampler);
	immediateContext->VSSetSamplers(0, 1, &sampler);
	immediateContext->DSSetSamplers(0, 1, &sampler);
	immediateContext->CSSetSamplers(0, 1, &sampler);
	
	immediateContext->PSSetShader(getPS()[0], nullptr, 0);
	immediateContext->RSSetViewports(1, &viewPort);
	immediateContext->OMSetRenderTargets(1, &renderTarget, dsView);
	immediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	immediateContext->RSSetState(pRS);
	ImGui_ImplDX11_Init(device, immediateContext);
}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
	swapChain->SetFullscreenState(FALSE, NULL);
	inputLayout[0]->Release();
	inputLayout[1]->Release();
	delete[] inputLayout;
	
	for (int i = 0; i < 5; i++) {
		if (vShader[i] != nullptr) {
			vShader[i]->Release();
		}
		if (pShader[i] != nullptr) {
			pShader[i]->Release();
		}
		if (gShader[i] != nullptr) {
			gShader[i]->Release();
		}
	}
	delete[] vShader;
	delete[] pShader;
	delete[] gShader;
	
	if (device != nullptr) {
		device->Release();
	}
	if (immediateContext != nullptr) {
		immediateContext->Release();
	}
	if (swapChain != nullptr) {
		swapChain->Release();
	}
	if (renderTarget != nullptr) {
		renderTarget->Release();
	}
	if (dsTexture != nullptr) {
		dsTexture->Release();
	}
	if (dsView != nullptr) {
		dsView->Release();
	}
	if (Pg_pConstantBuffer != nullptr) {
		Pg_pConstantBuffer->Release();
	}
	if (camConstBuffer != nullptr) {
		camConstBuffer->Release();
	}
	if (pRS != nullptr) {
		pRS->Release();
	}
	bs[0]->Release();
	bs[1]->Release();
	delete[] bs;
	
	if (sampler != nullptr) {
		sampler->Release();
	}
}

void Graphics::RsetViewPort()
{
	immediateContext->RSSetViewports(1, &viewPort);
}

void Graphics::Update(vec3 camPos)
{


	LCBG.cameraPos.element[3] = 0;

	for (int i = 0; i < nrOfLights; i++) {
		LCBG.lightPos.element[i][0] = light[i]->getPos().x;
		LCBG.lightPos.element[i][1] = light[i]->getPos().y;
		LCBG.lightPos.element[i][2] = light[i]->getPos().z;

		//TODO
		//om statiska ljus nej annars ja
		LCBG.lightView.element[i] = light[i]->getLightViewProj();

		LCBG.lightColor.element[i][0] = light[i]->getColor().x;
		LCBG.lightColor.element[i][1] = light[i]->getColor().y;
		LCBG.lightColor.element[i][2] = light[i]->getColor().z;
		
	
	}
	D3D11_MAPPED_SUBRESOURCE resource;
	immediateContext->Map(this->Pg_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &LCBG, sizeof(LCBGS));
	immediateContext->Unmap(this->Pg_pConstantBuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	this->immediateContext->PSSetConstantBuffers(6, 1, &this->Pg_pConstantBuffer);
	 
	this->CPCB.cameraPos.element[0] = camPos.x;
	this->CPCB.cameraPos.element[1] = camPos.y;
	this->CPCB.cameraPos.element[2] = camPos.z;
	this->CPCB.cameraPos.element[3] = 0;

	immediateContext->Map(camConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &CPCB, sizeof(CamPosCB));
	immediateContext->Unmap(camConstBuffer, 0);
	ZeroMemory(&resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	immediateContext->PSSetConstantBuffers(5, 1, &camConstBuffer);
}


void Graphics::UpdateFPSCounter(float dt)
{
	//fps
	static int a = 0;
	static float nextFpsUpdate = 0;

	nextFpsUpdate += (float)dt;
	a++;
	if (nextFpsUpdate >= 0.5f) {
		nextFpsUpdate = 0;
		float fps = (float)(a * 2);
		a = 0;
		SetWindowTextA(windowClass.getRenderWindow().getHandle(), ("Void Rush || fps: " + std::to_string(static_cast<int>(fps))).c_str());
	}
}

Vcb *Graphics::getVertexconstbuffer()
{
	return &vcbd;
}
Pcb *Graphics::getPixelconstbuffer()
{
	return &pcbd;
}
Gcb* Graphics::getGeometryconstbuffer()
{
	return &gcbd;
}
LCBGS* Graphics::getLightconstbufferforCS()
{
	return &LCBG;
}
ID3D11Device* Graphics::getDevice()
{
	return this->device;
}
ID3D11DeviceContext*& Graphics::get_IMctx()
{
	return this->immediateContext;
}
ID3D11Texture2D*& Graphics::getTexture()
{
	return tex;
}
HWND& Graphics::getWindow()
{
	return this->windowClass.getRenderWindow().getHandle();
}
ID3D11VertexShader** Graphics::getVS()
{
	return this->vShader;
}
ID3D11PixelShader** Graphics::getPS()
{
	return this->pShader;
}
ID3D11GeometryShader** Graphics::getGS()
{
	return this->gShader;
}
IDXGISwapChain*& Graphics::getSwapChain()
{
	return this->swapChain;
}

ID3D11InputLayout** Graphics::getInputLayout()
{
	return this->inputLayout;
}
ID3D11RenderTargetView*& Graphics::getRenderTarget()
{
	return this->renderTarget;
}
CamPosCB* Graphics::getCamPosconstbuffer()
{
	return &this->CPCB;
}
ID3D11DepthStencilView* Graphics::getDepthStencil()
{
	return this->dsView;
}
Light **Graphics::getLight()
{
	return this->light;
}

vec2 Graphics::getWH()
{
	return vec2((float)WIDTH, (float)HEIGHT);
}

vec2 Graphics::getClientWH()
{
	RECT rect;
	int width = 0;
	int height = 0;
	if (GetClientRect(windowClass.getRenderWindow().getHandle(), &rect))
	{
		width = rect.right - rect.left;
		height = rect.bottom - rect.top;
	}
	return vec2((float)width, (float)height);
}

void Graphics::setTransparant(bool transparance)
{
	if (transparance) {
		immediateContext->OMSetBlendState(bs[1], nullptr, 0xFFFFFFFFu);
	}
	else {
		immediateContext->OMSetBlendState(bs[0], nullptr, 0xFFFFFFFFu);
	}
}

void Graphics::takeLight(Light** light, int nrOfLights)
{
	this->nrOfLights = nrOfLights;
	this->light = light;
	/*set constant buffer*/
	for (int i = 0; i < nrOfLights; i++) {
		LCBG.lightView.element[i] = this->light[i]->getLightViewProj();
		LCBG.lightPos.element[i][3] = (float)light[i]->getType();
		LCBG.lightColor.element[i][3] = light[i]->getFallOff();
	}
}

void Graphics::takeIM(ImguiManager* manager)
{
	this->imguimanager = manager;
}

Window& Graphics::getWindosClass()
{
	return windowClass;
}

void Graphics::clearScreen()
{
	float clearColor[4] = { 0.1f,0.1f,0.1f,0 };
	immediateContext->ClearRenderTargetView(renderTarget, clearColor);
	immediateContext->ClearDepthStencilView(dsView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
}

void Graphics::setRenderTarget()
{
	immediateContext->OMSetRenderTargets(1, &renderTarget, dsView);
}

void Graphics::present(int lightNr)
{
	this->imguimanager->updateRender();	
	swapChain->Present(0, 0);
}
