#include "CommonHeader.h"
#include "App.h"
#include "Graphics.h"
#include "Box.h"

HWND g_hWnd = 0;
HINSTANCE g_hInstance = 0;

HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);

int APIENTRY WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine, 
	_In_ int nCmdShow)
{
	HRESULT hr = InitWindow(hInstance, nCmdShow);
	if (FAILED(hr))
		return 0;

	// Game Class
	App game(g_hWnd);

	//hr = game.Gfx();
	if (FAILED(hr))
	{
		// Error Message - Fail Direct Initialize
		return 0;
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (WM_QUIT != msg.message)
	{
		if(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			game.Go();
		}
	}

	return -1;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
		break;

	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_XBUTTONDOWN:
	case WM_XBUTTONUP:
	case WM_MOUSEHOVER:
		DirectX::Mouse::ProcessMessage(msg, wParam, lParam);
		break;

	case WM_CLOSE:
	{
		int result = MessageBox(NULL, L"Really Quit?", L"Alarm", MB_YESNO);
		if (result == IDYES)
		{
			PostQuitMessage(0);
		}
	}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

//=============================================================

HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wcex = {};
	
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = nullptr;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = TEXT("DX11_Project");
	wcex.hIconSm = nullptr;
		
	if (!RegisterClassEx(&wcex))
		return E_FAIL;

	g_hInstance = hInstance;
	RECT rc = { 0, 0, 800, 600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	g_hWnd = CreateWindow(
		TEXT("DX11_Project"),
		TEXT("My_DX11_Project"),
		WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	if (!g_hWnd)
		return E_FAIL;

	ShowWindow(g_hWnd, nCmdShow);
	return S_OK;
}