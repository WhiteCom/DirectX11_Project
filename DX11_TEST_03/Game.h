#pragma once

#include "CommonHeader.h"
#include "Timer.h"

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

	void DrawTestTriangle();

	void DrawTestTriangle2(float angle);

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	Timer m_Timer;
};