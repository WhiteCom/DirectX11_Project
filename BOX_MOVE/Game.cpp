#include "CommonHeader.h"
#include "Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;

Game::Game() noexcept :
    m_window(nullptr),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_9_1),
    m_cubePosition(0.0f, 0.0f, 0.0f)
{
}

void Game::Initialize(HWND window, int width, int height)
{
    m_window = window;
    m_outputWidth = width;
    m_outputHeight = height;

    m_keyboard = std::make_unique<Keyboard>();

    CreateDevice();
    CreateResources();

    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
}

void Game::CreateResources()
{
    // ���� Ÿ�� �� ���� ���� ����
    ComPtr<ID3D11Texture2D> backBuffer;
    m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf()));
    m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf());

    CD3D11_TEXTURE2D_DESC depthStencilDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, m_outputWidth, m_outputHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);
    ComPtr<ID3D11Texture2D> depthStencil;
    m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf());

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf());

    // CommonStates ���� (�����Ͷ�����, ���� ���� ��)
    m_states = std::make_unique<CommonStates>(m_d3dDevice.Get());

    // ���̴��� ��� ���۸� �����ϴ� BasicEffect ����
    m_effect = std::make_unique<BasicEffect>(m_d3dDevice.Get());
    m_effect->SetVertexColorEnabled(true); // ���� ���� ��� Ȱ��ȭ

    // �Է� ���̾ƿ� ����
    void const* shaderByteCode;
    size_t byteCodeLength;
    m_effect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

    HRESULT hr = m_d3dDevice->CreateInputLayout(VertexPositionColor::InputElements,
        VertexPositionColor::InputElementCount,
        shaderByteCode, byteCodeLength,
        m_inputLayout.ReleaseAndGetAddressOf());
    if (FAILED(hr)) throw std::runtime_error("CreateInputLayout failed.");

    // ť�� ������Ʈ�� ����
    // �� ���� �ٸ� ���� �������� ������ ������ �� �����Ƿ� 24���� ������ �ʿ��մϴ�.
    std::vector<VertexPositionColor> vertices =
    {
        // �ո� (����)
        { Vector3(-1.0f, -1.0f, -1.0f), Colors::Red },
        { Vector3(1.0f, -1.0f, -1.0f), Colors::Red },
        { Vector3(1.0f, 1.0f, -1.0f), Colors::Red },
        { Vector3(-1.0f, 1.0f, -1.0f), Colors::Red },

        // �޸� (���)
        { Vector3(-1.0f, -1.0f, 1.0f), Colors::Green },
        { Vector3(-1.0f, 1.0f, 1.0f), Colors::Green },
        { Vector3(1.0f, 1.0f, 1.0f), Colors::Green },
        { Vector3(1.0f, -1.0f, 1.0f), Colors::Green },

        // ���� (�Ķ�)
        { Vector3(-1.0f, 1.0f, -1.0f), Colors::Blue },
        { Vector3(1.0f, 1.0f, -1.0f), Colors::Blue },
        { Vector3(1.0f, 1.0f, 1.0f), Colors::Blue },
        { Vector3(-1.0f, 1.0f, 1.0f), Colors::Blue },

        // �Ʒ��� (���)
        { Vector3(-1.0f, -1.0f, -1.0f), Colors::Yellow },
        { Vector3(-1.0f, -1.0f, 1.0f), Colors::Yellow },
        { Vector3(1.0f, -1.0f, 1.0f), Colors::Yellow },
        { Vector3(1.0f, -1.0f, -1.0f), Colors::Yellow },

        // ���� �� (û��)
        { Vector3(-1.0f, -1.0f, 1.0f), Colors::Cyan },
        { Vector3(-1.0f, -1.0f, -1.0f), Colors::Cyan },
        { Vector3(-1.0f, 1.0f, -1.0f), Colors::Cyan },
        { Vector3(-1.0f, 1.0f, 1.0f), Colors::Cyan },

        // ������ �� (��ȫ)
        { Vector3(1.0f, -1.0f, -1.0f), Colors::Magenta },
        { Vector3(1.0f, -1.0f, 1.0f), Colors::Magenta },
        { Vector3(1.0f, 1.0f, 1.0f), Colors::Magenta },
        { Vector3(1.0f, 1.0f, -1.0f), Colors::Magenta }
    };

    // ���� ���� ����
    D3D11_BUFFER_DESC vertexBufferDesc = {};
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexPositionColor) * vertices.size();
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    D3D11_SUBRESOURCE_DATA vertexData = {};
    vertexData.pSysMem = vertices.data();
    hr = m_d3dDevice->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.ReleaseAndGetAddressOf());
    if (FAILED(hr)) throw std::runtime_error("CreateBuffer (Vertex) failed.");


    // �ε��� ������ ���� (12���� �ﰢ��)
    std::vector<WORD> indices =
    {
        0, 2, 1, 0, 3, 2, // ��
        4, 6, 5, 4, 7, 6, // ��
        8, 10, 9, 8, 11, 10, // ��
        12, 14, 13, 12, 15, 14, // �Ʒ�
        16, 18, 17, 16, 19, 18, // ����
        20, 22, 21, 20, 23, 22  // ������
    };

    // �ε��� ���� ����
    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(WORD) * indices.size();
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    D3D11_SUBRESOURCE_DATA indexData = {};
    indexData.pSysMem = indices.data();
    hr = m_d3dDevice->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.ReleaseAndGetAddressOf());
    if (FAILED(hr)) throw std::runtime_error("CreateBuffer (Index) failed.");

    // ����, ��, �������� ��� ����
    m_world = Matrix::Identity;

    m_view = Matrix::CreateLookAt(Vector3(0.f, 2.f, -5.f), Vector3::Zero, Vector3::UnitY);

    float aspectRatio = float(m_outputWidth) / float(m_outputHeight);
    m_projection = Matrix::CreatePerspectiveFieldOfView(XM_PIDIV4, aspectRatio, 0.1f, 100.f);

    m_effect->SetView(m_view);
    m_effect->SetProjection(m_projection);
}


void Game::Tick()
{
    m_timer.Tick([&]()
        {
            Update(m_timer);
        });

    Render();
}

void Game::Update(DX::StepTimer const& timer)
{
    float elapsedTime = float(timer.GetTotalSeconds());

    float moveSpeed = 2.0f * (float(timer.GetElapsedSeconds()));
    
    // Q : X Axis (+)
    // E : X Axis (-)
    // A : Y Axis (+)
    // D : Y Axis (-)
    // Z : Z Axis (+)
    // C : Z Axis (-)
    DirectX::Keyboard::State keyboardState = m_keyboard->GetState();
    if (keyboardState.Q)
        m_cubePosition.x += moveSpeed;
    else if (keyboardState.E)
        m_cubePosition.x -= moveSpeed;

    if (keyboardState.A)
        m_cubePosition.y += moveSpeed;
    else if (keyboardState.D)
        m_cubePosition.y -= moveSpeed;

    if (keyboardState.Z)
        m_cubePosition.z += moveSpeed;
    else if (keyboardState.C)
        m_cubePosition.z -= moveSpeed;

    // �ð��� ���� ť�긦 ȸ����Ŵ
    //m_world = Matrix::CreateRotationY(elapsedTime) * Matrix::CreateRotationX(elapsedTime * 0.5f);
    m_world = Matrix::CreateTranslation(m_cubePosition);

    m_effect->SetWorld(m_world);
}

void Game::Render()
{
    if (m_timer.GetFrameCount() == 0) return;
    Clear();

    // �Է� �����(IA) �ܰ� ����
    UINT stride = sizeof(VertexPositionColor);
    UINT offset = 0;
    m_d3dContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    m_d3dContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
    m_d3dContext->IASetInputLayout(m_inputLayout.Get());
    m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // �����Ͷ����� ���� ���� (���� ���� ���)
    m_d3dContext->RSSetState(m_states->CullClockwise());
    m_d3dContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);

    // ���̴� �� ��� ���� ����
    m_effect->Apply(m_d3dContext.Get());

    // ť�� �׸���
    m_d3dContext->DrawIndexed(36, 0, 0);

    Present();
}


// --- �Ʒ��� Direct3D �ʱ�ȭ �� ��ġ ���� �ڵ��, ������ �ʿ䰡 ���� �����ϴ�. ---

void Game::OnSuspending() { m_d3dContext->ClearState(); }
void Game::OnResuming() 
{ 
    m_timer.ResetElapsedTime();
    m_keyboard.reset();
}
void Game::OnDeviceLost() { m_states.reset(); m_effect.reset(); m_inputLayout.Reset(); m_vertexBuffer.Reset(); m_indexBuffer.Reset(); }

void Game::CreateDevice()
{
    UINT creationFlags = 0;
#ifdef _DEBUG
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    static const D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0, D3D_FEATURE_LEVEL_9_3, D3D_FEATURE_LEVEL_9_2, D3D_FEATURE_LEVEL_9_1 };
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, creationFlags, featureLevels, _countof(featureLevels), D3D11_SDK_VERSION, device.GetAddressOf(), &m_featureLevel, context.GetAddressOf());
    if (FAILED(hr)) throw std::runtime_error("D3D11CreateDevice failed");
    device.As(&m_d3dDevice);
    context.As(&m_d3dContext);

    ComPtr<IDXGIDevice1> dxgiDevice;
    m_d3dDevice.As(&dxgiDevice);
    ComPtr<IDXGIAdapter> dxgiAdapter;
    dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf());
    ComPtr<IDXGIFactory2> dxgiFactory;
    dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf()));

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.Width = m_outputWidth;
    swapChainDesc.Height = m_outputHeight;
    swapChainDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

    dxgiFactory->CreateSwapChainForHwnd(m_d3dDevice.Get(), m_window, &swapChainDesc, nullptr, nullptr, m_swapChain.ReleaseAndGetAddressOf());
}

void Game::Clear()
{
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, (float)m_outputWidth, (float)m_outputHeight);
    m_d3dContext->RSSetViewports(1, &viewport);
}

void Game::Present() 
{ 
    m_swapChain->Present(1, 0); 
}