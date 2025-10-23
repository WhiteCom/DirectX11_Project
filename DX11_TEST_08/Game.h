#pragma once

#include "CommonHeader.h"
#include "Timer.h"

#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include "SpriteBatch.h"

#include "AnimatedTexture.h"
#include "ScrollingBackground.h"

class Game
{
public:
	Game() noexcept;
	~Game() noexcept;

public:
	HRESULT InitGame( HWND hWnd );
	void DestroyGame();

	void ClearBuffer();
	void UpdateGame();
	void RenderGame();

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;

	// Test Code - 2025.10.23
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	std::unique_ptr<AnimatedTexture> m_ship;
	DirectX::XMFLOAT2 m_shipPos;

	std::unique_ptr<ScrollingBackground> m_backgroundStars;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backgroundTex;
	DirectX::XMFLOAT2 m_backgroundSize;

	Timer m_Timer;
	float m_UpdateDeltaTime;
};