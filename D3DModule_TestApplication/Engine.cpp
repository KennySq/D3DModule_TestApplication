#include"pch.h"
#include "Engine.h"

shared_ptr<Engine> Engine::EngineInstance = nullptr;

bool Engine::OnInit()
{
	FBXLoader Loader;
	Camera = make_shared<D3DACamera>(D3DACamera());
	shared_ptr<D3DAModel> SampleModel = Loader.Load("Resources/Stylized_box.FBX");
		
	Renderer->GetSwapChainBuffer(0, SwapChainTexture);
	Renderer->AddRenderViewport(1.0f, 1.0f, BIWidth, BIHeight, 1.0f);

	
	
	static ComPtr<ID3D11RenderTargetView> SwapChainRTV = *SwapChainTexture->GetResource();
	static shared_ptr<D3DRS::D3DDepthStencilTexture> DepthStencilTexture;
	Renderer->GetSwapChainDepthBuffer(DepthStencilTexture);

	Renderer->SetSwapChainTexture(SwapChainTexture);
	Renderer->SetSwapChainDepthStencil(DepthStencilTexture);


	Context->RSSetViewports(0, &Renderer->GetViewport(0));
	Context->OMSetRenderTargets(1, SwapChainRTV.GetAddressOf(), *DepthStencilTexture->GetResource());
	return true;
}

void Engine::OnUpdate(float Delta)
{	
	static ComPtr<ID3D11RenderTargetView> SwapChainRTV = *SwapChainTexture->GetResource();
	static shared_ptr<D3DRS::D3DDepthStencilTexture> DepthStencilTexture = Renderer->GetSwapChainDepthStencil();

	Renderer->ClearTexture2D(SwapChainTexture, Colors::Red);
	Renderer->ClearDepthStencil(DepthStencilTexture);
}

void Engine::OnRender(float Delta)
{
	Renderer->Execute();
	SwapChain->Present(0, 0);
}

void Engine::OnRelease()
{}
