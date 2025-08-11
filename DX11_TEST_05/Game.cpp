#include "Game.h"
#include <sstream>
#include <DirectXMath.h>

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

	//
	// Creawte Depth stencil state
	//
	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = 800u;
	descDepth.Height = 600u;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = m_d3dDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error - Failed CreateTexture2D", L"Error", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	//
	// Create View of Depth stencil texture
	//
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;

	hr = m_d3dDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &m_depthStencilView);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error - Failed Create DepthStencilView", L"Error", MB_OK | MB_ICONERROR);
		return E_FAIL;
	}

	// Bind Depth stencil view to OM
	m_d3dContext->OMSetRenderTargets(1u, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

	m_mouse = std::make_unique<DirectX::Mouse>();
	m_mouse->SetWindow(hWnd);

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
	m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Game::RenderGame()
{
	ClearBuffer();

	//DrawTestTriangle();
	DirectX::Mouse::State mouseState = m_mouse->GetState();
	
	DrawBox(-m_Timer.Peek(),
		0,
		0);

	DrawBox(m_Timer.Peek(),
		mouseState.x / 400.0f - 1.0f,
		-mouseState.y / 300.0f + 1.0f);

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
	}

	return;
}

void Game::DrawBox(float angle, float x, float z)
{
	HRESULT hr;

	struct Vertex
	{
		struct Position
		{
			float x;
			float y;
			float z;
		};

		Position pos;
	};

	//
	// Create Vertex Buffer (1 2d triangle at center of screen)
	//
	Vertex vertices[] =
	{
		{ -1.0f, -1.0f, -1.0f },
		{ 1.0f, -1.0f, -1.0f },
		{ -1.0f, 1.0f, -1.0f },
		{ 1.0f, 1.0f, -1.0f },
		{ -1.0f, -1.0f, 1.0f },
		{ 1.0f, -1.0f, 1.0f },
		{ -1.0f, 1.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f },
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;

	hr = m_d3dDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error - Game::DrawBox() [Create Vertex Buffer]", L"ERROR", MB_OK | MB_ICONERROR);
		return;
	}

	// Bind Vertex Buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	m_d3dContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	//
	// Create Index Buffer
	//
	const unsigned short indices[] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;

	hr = m_d3dDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error - Game::DrawBox() [Create Index Buffer]", L"ERROR", MB_OK | MB_ICONERROR);
		return;
	}

	// Bind index Buffer to pipeline
	m_d3dContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

	//
	// Create Constant Buffer for Transformation Matrix
	//
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};

	const ConstantBuffer cb =
	{
		{
			 DirectX::XMMatrixTranspose(
				DirectX::XMMatrixRotationZ(angle)
				* DirectX::XMMatrixRotationX(angle)
				* DirectX::XMMatrixTranslation(x, 0.0f, z + 4.0f)
				 * DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 10.0f)
			 )
		}
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;

	hr = m_d3dDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error - Game::DrawBox() [Create ConstantBuffer]", L"ERROR", MB_OK | MB_ICONERROR);
		return;
	}

	// Bind Constant Buffer to Vertex Shader
	m_d3dContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());

	// Lookup tagble for cube face colors
	struct ConstantBuffer2
	{
		struct 
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6]; // 6 face
	};

	const ConstantBuffer2 cb2 =
	{
		{
			{1.0f, 0.0f, 1.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 1.0f},
			{1.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 1.0f}
		}
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2;
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(cb2);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &cb2;

	hr = m_d3dDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error - Game::DrawBox() [Create ConstantBuffer2]", L"ERROR", MB_OK | MB_ICONERROR);
		return;
	}

	// Bind Constant Buffer to Pixel Shader
	m_d3dContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());

	//
	// Create Pixel Shader
	//
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

	hr = D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error - Game::DrawBox() [Read Pixel Shader File]", L"ERROR", MB_OK | MB_ICONERROR);
		return;
	}

	hr = m_d3dDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error - Game::DrawBox() [CreatePixelShader]", L"ERROR", MB_OK | MB_ICONERROR);
		return;
	}

	// Bind Pixel Shader
	m_d3dContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	//
	// Create Vertex Shader
	//
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;

	hr = D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error - Game::DrawBox() [Read Vertex Shader File]", L"ERROR", MB_OK | MB_ICONERROR);
		return;
	}

	hr = m_d3dDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error - Game::DrawBox() [CreateVertexShader]", L"ERROR", MB_OK | MB_ICONERROR);
		return;
	}

	// Bind Vertex Shader
	m_d3dContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	//
	// input (vertex) layout (2d position only)
	//
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	hr = m_d3dDevice->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);

	if (FAILED(hr))
	{
		MessageBox(NULL, L"Error - Game::DrawTestTriangle() [CreateInputLayout]", L"ERROR", MB_OK | MB_ICONERROR);
		return;
	}

	// Bind Vertex Layout
	m_d3dContext->IASetInputLayout(pInputLayout.Get());

	// Bind Render target
	//m_d3dContext->OMSetRenderTargets(1u, m_renderTargetView.GetAddressOf(), nullptr);

	// Set primitive topology to triangle list (group of 3 vertices)
	m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//
	// Configure Viewport
	//
	D3D11_VIEWPORT vp;
	vp.Width = 800; // 생성된 윈도우 사이즈와 맞추기
	vp.Height = 600; // 생성된 윈도우 사이즈와 맞추기
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	m_d3dContext->RSSetViewports(1u, &vp);

	m_d3dContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}