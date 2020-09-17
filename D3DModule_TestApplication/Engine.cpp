#include"pch.h"
#include "Engine.h"

shared_ptr<Engine> Engine::EngineInstance = nullptr;

bool Engine::OnInit()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	auto IO = ImGui::GetIO();
	ImGui_ImplWin32_Init(BIAppWindowHandle);

	ImGui_ImplDX11_Init(Device.Get(), Context.Get());
	ImGui::StyleColorsDark();

	FBXLoader Loader;
	Camera = make_shared<D3DACamera>(D3DACamera());
	shared_ptr<D3DAModel> SampleModel = Loader.Load("Resources/Stylized_box.FBX");
	shared_ptr<D3DAMaterial> Material = make_shared<D3DAMaterial>();
	CompilePass("Sample.hlsl", "Sample", PASSTYPE::VER | PASSTYPE::PIX, Material);

	Renderer->GetSwapChainBuffer(0, SwapChainTexture);
	Renderer->AddRenderViewport(1.0f, 1.0f, BIWidth, BIHeight, 1.0f);



	static ComPtr<ID3D11RenderTargetView> SwapChainRTV = *SwapChainTexture->GetResource();
	static shared_ptr<D3DRS::D3DDepthStencilTexture> DepthStencilTexture;
	Renderer->GetSwapChainDepthBuffer(DepthStencilTexture);

	Renderer->SetSwapChainTexture(SwapChainTexture);
	Renderer->SetSwapChainDepthStencil(DepthStencilTexture);
	Renderer->CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, false);

	// ���� �ڵ� //
	Context->RSSetViewports(0, &Renderer->GetViewport(0));
	Context->OMSetRenderTargets(1, SwapChainRTV.GetAddressOf(), *DepthStencilTexture->GetResource());
	// ******** //



	SelectedScene = make_shared<Scene>();

	SelectedScene->AddMaterial(Material);
	SelectedScene->AddCamera(Camera);


	auto Inst = SelectedScene->MakeInstance("Sample Instance");
	Inst->SetMesh(SampleModel);
	Inst->AddMaterial(Material);

	ShaderEditor = make_shared<GUIShaderEditor>();
	ShaderEditor->Open("Sample.hlsl", Material);


	return true;
}

void Engine::OnUpdate(float Delta)
{
	{

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();


		ImGui::NewFrame();
		ImGui::Begin("Test");
		ImGui::Text("Sample");
		ImGui::End();

		ShaderEditor->Editor();

		ImGui::EndFrame();



		{
			ImGui::Render();

		}

	}

	static ComPtr<ID3D11RenderTargetView> SwapChainRTV = *SwapChainTexture->GetResource();
	static shared_ptr<D3DRS::D3DDepthStencilTexture> DepthStencilTexture = Renderer->GetSwapChainDepthStencil();

	static D3DViewProject ViewProject;

	XMStoreFloat4x4(&ViewProject.View, Camera->GetView());
	XMStoreFloat4x4(&ViewProject.Projection, Camera->GetProjection());

	Context->UpdateSubresource(Camera->GetBuffer().Get(), 0, nullptr, &ViewProject, 0, 0);

	Renderer->ClearTexture2D(SwapChainTexture, Colors::Black);
	Renderer->ClearDepthStencil(DepthStencilTexture);

	static auto Inst = SelectedScene->GetInstance(0);
	Inst->SelectRenderTarget(SwapChainRTV.GetAddressOf(), 1, *DepthStencilTexture->GetResource());
	Inst->SelectViewport(Renderer->GetViewport(0));
	Inst->SetRasterizerState(Renderer.get(), 0);

	Inst->GetTransform()->Rotate(90, 0, 0);

	Inst->DrawMesh(Renderer.get());
}

void Engine::OnRender(float Delta)
{

	Renderer->Execute();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	SwapChain->Present(0, 0);


}

void Engine::OnRelease()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
