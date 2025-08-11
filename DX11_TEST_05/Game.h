#pragma once

#include "CommonHeader.h"
#include "Timer.h"
#include <Mouse.h> // from DirectXTK Library

class Game
{
public:
	Game() noexcept;
	~Game() noexcept;

public:
	HRESULT InitGame(HWND hWnd);
	void DestroyGame();

	void ClearBuffer();
	void UpdateGame();
	void RenderGame();

	void DrawBox(float angle, float x, float y);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	Timer m_Timer;

	std::unique_ptr<DirectX::Mouse> m_mouse;
};