#include "App.h"
#include "Box.h"

App::App(HWND hWnd)
{
	m_hWnd = hWnd;
	m_pGfx = std::make_unique<Graphics>(m_hWnd);

	std::mt19937 rng(std::random_device{} ());
	std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

	for (auto i = 0; i < 80; ++i)
	{
		m_boxes.push_back(std::make_unique<Box>(
			*m_pGfx,
			rng,
			adist,
			ddist,
			odist,
			rdist)
		);
	}

	m_pGfx->SetProjection( DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f) );
}

void App::Go()
{
	DoFrame();
}

App::~App()
{
}

void App::DoFrame()
{
	auto dt = m_Timer.Mark();
	m_pGfx->ClearBuffer(0.07f, 0.0f, 0.12f);
	
	// std::vector< std::unique_ptr< Box>>::iterator::value_type = std::unique_ptr<Box> 
	for (auto& b : m_boxes)
	{
		b->Update(dt);
		b->Draw(*m_pGfx);
	}
	m_pGfx->EndFrame();
}