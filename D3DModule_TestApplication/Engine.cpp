#include"pch.h"
#include "Engine.h"

shared_ptr<Engine> Engine::EngineInstance = nullptr;

bool Engine::OnInit()
{
	FBXLoader Loader;
	Camera = make_shared<D3DACamera>(D3DACamera());
	shared_ptr<D3DAModel> SampleModel = Loader.Load("Resources/Stylized_box.FBX");
	shared_ptr<D3DAMaterial> Material = make_shared<D3DAMaterial>();
	CompilePass("SampleVS.hlsl", "Sample", PASSTYPE::VER | PASSTYPE::PIX, Material);

	Renderer->GetSwapChainBuffer(0, SwapChainTexture);
	Renderer->AddRenderViewport(1.0f, 1.0f, BIWidth, BIHeight, 1.0f);

	
	
	static ComPtr<ID3D11RenderTargetView> SwapChainRTV = *SwapChainTexture->GetResource();
	static shared_ptr<D3DRS::D3DDepthStencilTexture> DepthStencilTexture;
	Renderer->GetSwapChainDepthBuffer(DepthStencilTexture);

	Renderer->SetSwapChainTexture(SwapChainTexture);
	Renderer->SetSwapChainDepthStencil(DepthStencilTexture);

	
	// 보류 코드 //
	Context->RSSetViewports(0, &Renderer->GetViewport(0));
	Context->OMSetRenderTargets(1, SwapChainRTV.GetAddressOf(), *DepthStencilTexture->GetResource());
	// ******** //


	SelectedScene = make_shared<Scene>();

	SelectedScene->AddMaterial(Material);
	SelectedScene->AddCamera(Camera);

	
	auto Inst = SelectedScene->MakeInstance("Sample Instance");
	Inst->SetMesh(SampleModel);
	Inst->AddMaterial(Material);
	
	return true;
}

void Engine::OnUpdate(float Delta)
{	
	static ComPtr<ID3D11RenderTargetView> SwapChainRTV = *SwapChainTexture->GetResource();
	static shared_ptr<D3DRS::D3DDepthStencilTexture> DepthStencilTexture = Renderer->GetSwapChainDepthStencil();

	static D3DViewProject ViewProject;

	XMStoreFloat4x4(&ViewProject.View, Camera->GetView());
	XMStoreFloat4x4(&ViewProject.Projection, Camera->GetProjection());
	
	Context->UpdateSubresource(Camera->GetBuffer().Get(), 0, nullptr, &ViewProject, 0, 0);

	Renderer->ClearTexture2D(SwapChainTexture, Colors::Red);
	Renderer->ClearDepthStencil(DepthStencilTexture);

	static auto Inst = SelectedScene->GetInstance(0);
	//Renderer->SelectRenderTarget(SwapChainRTV.GetAddressOf(), 1, )
	Inst->DrawMesh();


}

void Engine::OnRender(float Delta)
{
	Renderer->Execute();
	SwapChain->Present(0, 0);
}

void Engine::OnRelease()
{}
