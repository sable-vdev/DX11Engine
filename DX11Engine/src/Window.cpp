#include "Window.hpp"
#include "Application.hpp"

Window::Window(U32 width, U32 height, const std::wstring& windowTitle) : m_windowTitle(windowTitle), m_windowClass(windowTitle + L"class"), 
	m_hInstance(GetModuleHandle(nullptr)), m_hwnd(nullptr), m_width(width), m_height(height)
{
	WNDCLASS wc{};
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = Window::WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = m_windowClass.c_str();

	if (!RegisterClass(&wc))
		std::cout << "Failed to register class\n";

	I32 posX = (GetSystemMetrics(SM_CXSCREEN) - m_width) / 2;
	I32 posY = (GetSystemMetrics(SM_CYSCREEN) - m_height) / 2;

	I32 windowStyles = WS_OVERLAPPEDWINDOW | WS_MAXIMIZE;

	RECT rect = { 0, 0, m_width, m_height };
	AdjustWindowRect(&rect, windowStyles, FALSE);
	I32 adjustedWidth = rect.right - rect.left;
	I32 adjustedHeight = rect.bottom - rect.top;

	m_hwnd = CreateWindowExW(0, m_windowClass.c_str(), m_windowTitle.c_str(), windowStyles,
		posX, posY, adjustedWidth, adjustedHeight, nullptr, nullptr, m_hInstance, this);

	if (!m_hwnd) 
		std::cout << "Failed to initialize the window handle";

	if (m_hwnd != 0)
	{
		ShowWindow(m_hwnd, SW_SHOW);
		SetForegroundWindow(m_hwnd);
		SetFocus(m_hwnd);
	}
}

bool Window::Run()
{
	MSG msg = {};
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
			return false;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return true;
}

LRESULT Window::HandleMessages(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CLOSE:
		DestroyWindow(m_hwnd);
		return 0;
	case WM_SIZE:
	{
		RECT lprect;
		GetClientRect(m_hwnd, &lprect);
		m_width = lprect.right - lprect.left;
		m_height = lprect.bottom - lprect.top;
		Application::Get().OnResize(m_width, m_height);
		return 0;
	}
	default:
		return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
	}
}
