#pragma once
class Engine : public VirtualObject
{
private:
	
	shared_ptr<D3DHWDevice> HWDevice;
	shared_ptr<RenderManager> Renderer;
	ComPtr<ID3D11Device> Device;
	ComPtr<ID3D11DeviceContext> Context;
	ComPtr<IDXGISwapChain> SwapChain;

	shared_ptr<D3DRenderableTexture> SwapChainTexture;
	
	static shared_ptr<Engine> EngineInstance;
	shared_ptr<D3DARS::D3DACamera> Camera;
	
	shared_ptr<Scene> SelectedScene;

	shared_ptr<GUIShaderEditor> ShaderEditor;
	
	Engine()
	{
		HWDevice = D3DHWDevice::GetHWDevice();
		Device = D3DHWDevice::GetDevice();
		Context = D3DHWDevice::GetContext();
		SwapChain = D3DHWDevice::GetSwapChain();
		Renderer = make_shared<RenderManager>();
		Camera = make_shared<D3DACamera>();
	}

public:


private:
	virtual bool OnInit() override;
	virtual void OnUpdate(float Delta) override;
	virtual void OnRender(float Delta) override;
	virtual void OnRelease() override;

public:

	~Engine() {}

	static shared_ptr<Engine> GetEngineInstance() {
		if (EngineInstance == nullptr) 
			EngineInstance = make_shared<Engine>(Engine());
	
		return EngineInstance;
	}

};

