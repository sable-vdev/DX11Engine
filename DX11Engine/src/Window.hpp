#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <iostream>

#include "EngineTypes.hpp"

class Window
{
public:
	Window(U32 width, U32 height, const std::wstring& windowTitle);
	~Window() = default;
	
	inline const HWND GetHWND() const { return m_hwnd; }
	inline HWND GetHWND() { return m_hwnd; }
	inline U32 GetWidth() const { return m_width; }
	inline U32 GetHeight() const { return m_height; }

	bool Run();

	static inline LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Window* pWindow = nullptr;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
			pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));

			pWindow->m_hwnd = hwnd;

			return TRUE;
		}
		else
		{
			pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		}

		if (pWindow)
		{
			return pWindow->HandleMessages(uMsg, wParam, lParam);
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
private:
	LRESULT HandleMessages(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	HWND m_hwnd;
	HINSTANCE m_hInstance;
	U32 m_width;
	U32 m_height;
	std::wstring m_windowTitle;
	std::wstring m_windowClass;

};

