#pragma once

#include "CommonHeader.h"
#include "Timer.h"
#include "Graphics.h"

class App
{
public:
	App(HWND hWnd);
	void Go();
	~App();

	Graphics& Gfx() { return *m_pGfx; }

private:
	void DoFrame();

private:
	HWND m_hWnd;
	Timer m_Timer;
	std::unique_ptr<Graphics> m_pGfx;
	std::vector< std::unique_ptr< class Box>> m_boxes;
};