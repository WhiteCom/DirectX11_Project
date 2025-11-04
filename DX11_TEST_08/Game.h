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
	UINT m_texture_width;
	UINT m_texture_height;

	std::unique_ptr<DirectX::SpriteBatch> m_character_spriteBatch;
	std::unique_ptr<DirectX::SpriteBatch> m_background_spriteBatch;
	std::unique_ptr<AnimatedTexture> m_ship;
	DirectX::XMFLOAT2 m_shipPos;

	DirectX::XMMATRIX m_mat_Transform;

	std::unique_ptr<ScrollingBackground> m_backgroundStars;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_backgroundTex;
	DirectX::XMFLOAT2 m_backgroundSize;

	// 입력 위치 및 위치 변수
	std::unique_ptr<DirectX::Keyboard> m_keyBoard;

	Timer m_Timer;
	float m_UpdateDeltaTime;
};