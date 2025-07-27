#pragma once

#include "StepTimer.h"

class Game
{
public:
    Game() noexcept;

    void Initialize(HWND window, int width, int height);
    void Tick();
    void OnSuspending();
    void OnResuming();

private:
    void Update(DX::StepTimer const& timer);
    void Render();
    void Clear();
    void Present();
    void CreateDevice();
    void CreateResources();
    void OnDeviceLost();

    // ����̽� ���ҽ�
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;
    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    // ������ ���ҽ�
    std::unique_ptr<DirectX::CommonStates>          m_states;
    std::unique_ptr<DirectX::BasicEffect>           m_effect;
    Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_inputLayout;

    // ť�� ������Ʈ�� ���ҽ�
    Microsoft::WRL::ComPtr<ID3D11Buffer>            m_vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer>            m_indexBuffer;

    // ��ȯ ���
    DirectX::SimpleMath::Matrix                     m_world;
    DirectX::SimpleMath::Matrix                     m_view;
    DirectX::SimpleMath::Matrix                     m_projection;

    // �Է� ��ġ, ť�� ��ġ �߰�
    std::unique_ptr<DirectX::Keyboard>              m_keyboard;
    DirectX::SimpleMath::Vector3                    m_cubePosition;

    // ���� Ÿ�̸�
    DX::StepTimer                                   m_timer;
};
