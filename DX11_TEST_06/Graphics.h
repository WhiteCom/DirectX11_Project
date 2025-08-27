#pragma once

#include "CommonHeader.h"
#include "Timer.h"
#include <Mouse.h> // from DirectXTK Library
#include <Keyboard.h> // from DirectXTK Library
#include <DirectXMath.h>

class Graphics
{
	friend class Bindable;

public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;

public:
	//HRESULT Init(HWND hWnd);
	//void Destroy();
	void EndFrame();

	void ClearBuffer(float red, float green, float blue) noexcept;
	//void Update();

	//void DrawBox(float angle, float x, float y);
	void DrawIndexed(UINT count) noexcept;
	void SetProjection(DirectX::FXMMATRIX proj) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	std::unique_ptr<DirectX::Mouse> m_mouse;
	std::unique_ptr<DirectX::Keyboard> m_keyboard;

	DirectX::XMMATRIX m_projection;

	float m_brightness;
};
