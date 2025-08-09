#include "Game.h"
#include <sstream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
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

	return S_OK;
}

void Game::DestroyGame()
{
	// To do...
}

void Game::UpdateGame()
{
	// To do...
}

void Game::ClearBuffer()
{
	const float red_tick = sin(m_Timer.Peek()) / 2.0f + 0.5f;
	const float green_tick = cos(m_Timer.Peek()) / 2.0f + 0.5f;
	const float blue_tick = sin(m_Timer.Peek()) / 4.0f + 0.25f;
	float ClearColor[4] = { red_tick, green_tick, blue_tick, 1.0f };
	m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), ClearColor);
}

void Game::RenderGame()
{
	ClearBuffer();

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

