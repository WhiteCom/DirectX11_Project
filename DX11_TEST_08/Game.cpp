#include "Game.h"
#include <sstream>

#define DEFAULT_VALUE 1.0f

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "DirectXTK.lib")

Game::Game() noexcept {}
Game::~Game() noexcept {}

HRESULT Game::InitGame(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	UINT swapCreateFlags = 0u;
#ifndef NDEBUG
	swapCreateFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr;

	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_swapChain,
		&m_d3dDevice,
		nullptr,
		&m_d3dContext
	);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error - Failed D3D11CreateDeviceAndSwapChain", L"Error", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	Microsoft::WRL::ComPtr<ID3D11Resource> backBuffer;
	hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), &backBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error - Failed GetBuffer From SwapChain", L"Error", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	hr = m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, &m_renderTargetView);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error - Failed Create RenderTargetView", L"Error", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// Binding Render Target View
	m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

	// Set Viewport
	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<FLOAT>(800);
	viewport.Height = static_cast<FLOAT>(600);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	m_d3dContext->RSSetViewports(1, &viewport);

	//==============================

	// Create Texture using sprite sheet
	//hr = DirectX::CreateDDSTextureFromFile(m_d3dDevice.Get(), L"G:/VS2022_Project/DX11_BomberMan/DX11_Test01/shipanimated.dds", nullptr, m_texture.GetAddressOf());
	hr = DirectX::CreateDDSTextureFromFile(m_d3dDevice.Get(), L"shipanimated.dds", nullptr, m_texture.GetAddressOf());
	if (FAILED(hr))
	{
		// To do...
		// Error Message
	}

	// Get Texture Width, Height
	ID3D11Resource* resource = nullptr;
	m_texture.Get()->GetResource(&resource);
	
	ID3D11Texture2D* texture2D = nullptr;
	resource->QueryInterface(&texture2D);

	D3D11_TEXTURE2D_DESC texDesc;
	texture2D->GetDesc(&texDesc);

	m_texture_width = texDesc.Width;
	m_texture_height = texDesc.Height;

	// Delete Reference
	resource->Release();
	texture2D->Release();

	// Create a sprite batch for drawing
	m_character_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_d3dContext.Get());
	m_background_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_d3dContext.Get());

	// Create an AnimatedTexture helper class instance and set it to use texture
	// Which is assumed to have 4 frames of animation with a FPS of 2 seconds
	m_ship = std::make_unique<AnimatedTexture>(DirectX::XMFLOAT2(0, 0), 0.0f, 2.0f, 0.5f);
	const int frameCount = 4;
	const int frameSeconds = 20;
	m_ship->Load(m_texture.Get(), frameCount, frameSeconds);

	m_shipPos.x = 10.f;
	m_shipPos.y = 10.f;

	m_mat_Transform = DirectX::XMMatrixIdentity();

	m_keyBoard = std::make_unique<DirectX::Keyboard>();

	//=====================================

	hr = DirectX::CreateWICTextureFromFile(m_d3dDevice.Get(), L"starfield.png", nullptr, m_backgroundTex.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		// To do...
		// Error Message
	}

	m_backgroundStars = std::make_unique<ScrollingBackground>();
	m_backgroundStars->Load(m_backgroundTex.Get());

	m_backgroundSize.x = 800.0f;
	m_backgroundSize.y = 1600.0f;

	m_backgroundStars->SetWindow(m_backgroundSize.x, m_backgroundSize.y);

	m_Timer.Mark();
	m_UpdateDeltaTime = m_Timer.Peek();

	return S_OK;
}

void Game::DestroyGame()
{
	// To do...
	m_ship.reset();

	m_backgroundStars.reset();
	m_backgroundTex.Reset();

	m_keyBoard.reset();
}

void Game::UpdateGame()
{
	// To do...
	float dt = m_Timer.Mark() * 1.0f;

	m_backgroundStars->Update(dt * 500);
	m_ship->Update(dt);

	
	// To do...
	// 키 입력 체크
	DirectX::Keyboard::State kbState = m_keyBoard->GetState();
	float moveSpeed = 500.0f * dt;

	// 키 입력으로 이동 WASD
	if (kbState.A)
		m_shipPos.x -= moveSpeed;
	if (kbState.D)
		m_shipPos.x += moveSpeed;
	if (kbState.S)
		m_shipPos.y += moveSpeed;
	if (kbState.W)
		m_shipPos.y -= moveSpeed;

	float minX = 0.0f;
	float maxX = 800.0f - m_texture_width;
	float minY = 0.0f;
	float maxY = 600.0f - m_texture_height;

	m_shipPos.x = minX < (m_shipPos.x > maxX ? maxX : m_shipPos.x) ? (m_shipPos.x > maxX ? maxX : m_shipPos.x) : minX; // max( minX, min( m_shipPos.x, maxX ) )
	m_shipPos.y = minY < (m_shipPos.y > maxY ? maxY : m_shipPos.y) ? (m_shipPos.y > maxY ? maxY : m_shipPos.y) : minY; // max( minY, min( m_shipPos.y, maxY ) )
	
	// 시간에 따라 비행기를 이동시킴
	// 행렬 합성 순서 = S * R * T
	// To do...
	DirectX::XMMATRIX mat_Scale = DirectX::XMMatrixScaling(DEFAULT_VALUE, DEFAULT_VALUE, DEFAULT_VALUE);
	DirectX::XMMATRIX mat_Rotation = DirectX::XMMatrixRotationZ(dt);
	DirectX::XMMATRIX mat_Translation = DirectX::XMMatrixTranslation(m_shipPos.x, m_shipPos.y, 0.0f);

	m_mat_Transform = mat_Scale * mat_Rotation * mat_Translation;
}

void Game::ClearBuffer()
{
	//const float red_tick = sin(m_Timer.Peek()) / 2.0f + 0.5f;
	//const float green_tick = cos(m_Timer.Peek()) / 2.0f + 0.5f;
	//const float blue_tick = sin(m_Timer.Peek()) / 4.0f + 0.25f;
	//float ClearColor[4] = { red_tick, green_tick, blue_tick, 1.0f };
	float ClearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f };
	m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), ClearColor);
}

void Game::RenderGame()
{
	ClearBuffer();

	//
	// Background Render
	//
	m_background_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred);
	m_backgroundStars->Draw(m_background_spriteBatch.get()); // 배경은 별도 행렬 사용
	m_background_spriteBatch->End();

	//
	// Character Render
	//
	m_character_spriteBatch->Begin(DirectX::SpriteSortMode_Deferred, nullptr, nullptr, nullptr, nullptr, nullptr, m_mat_Transform);

	//m_backgroundStars->Draw(m_character_spriteBatch.get());
	m_ship->Draw(m_character_spriteBatch.get(), m_shipPos);

	m_character_spriteBatch->End();

	HRESULT hr;
	hr = m_swapChain->Present(1u, 0u);

	if (FAILED(hr))
	{
		throw std::runtime_error("Error - Game::RenderGame()");
		HRESULT hr_err = m_d3dDevice->GetDeviceRemovedReason();
		switch (hr_err)
		{
		case DXGI_ERROR_DEVICE_REMOVED:
			MessageBox(NULL, L"ERR MSG : DXGI_ERROR_DEVICE_REMOVED", L"ERROR", MB_OK | MB_ICONERROR);
			break;

		case DXGI_ERROR_DEVICE_RESET:
			MessageBox(NULL, L"ERR MSG : DXGI_ERROR_DEVICE_RESET", L"ERROR", MB_OK | MB_ICONERROR);
			break;

		case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
			MessageBox(NULL, L"ERR MSG : DXGI_ERROR_DRIVER_INTERNAL_ERROR", L"ERROR", MB_OK | MB_ICONERROR);
			break;

		case DXGI_ERROR_INVALID_CALL:
			MessageBox(NULL, L"ERR MSG : DXGI_ERROR_INVALID_CALL", L"ERROR", MB_OK | MB_ICONERROR);
			break;

			// To do...
			// If it occured more error Add Error message to here.

		default:
			MessageBox(NULL, L"ERR MSG : Unknown ERROR", L"ERROR", MB_OK | MB_ICONERROR);
			break;
		}
		return;
	}
}

