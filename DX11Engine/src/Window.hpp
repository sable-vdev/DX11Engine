#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <iostream>
#include <ShObjIdl_core.h>
#include <comdef.h>

#include "Logger.hpp"
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

	//source https://stackoverflow.com/a/72429080
	static bool OpenWin32FileDialog(std::string& filePath)
	{
		HRESULT res;
		if (FAILED(res = !CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE)))
		{
			_com_error err(res);
			LOG_ERROR("CoInitializeEx failed 0x{:08X} ({})", static_cast<unsigned long>(res), Logger::WideToNarrow(err.ErrorMessage()));
			return false;
		}

		IFileOpenDialog* fileDialog;
		if (FAILED(res = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_ALL, IID_IFileOpenDialog, reinterpret_cast<void**>(&fileDialog))))
		{
			LOG_ERROR("CoCreateInstance failed 0x{:08X}", static_cast<unsigned long>(res));
			CoUninitialize();
			return false;
		}

		if (FAILED(res = fileDialog->Show(nullptr)))
		{
			if(res != HRESULT_FROM_WIN32(ERROR_CANCELLED))
				LOG_ERROR("File Dialog Show failed 0x{:08X}", static_cast<unsigned long>(res));

			fileDialog->Release();
			CoUninitialize();
			return false;
		}

		IShellItem* files;
		if (FAILED(res = fileDialog->GetResult(&files)))
		{
			LOG_ERROR("File Dialog GetResult failed 0x{:08X}", static_cast<unsigned long>(res));
			fileDialog->Release();
			CoUninitialize();
			return false;
		}

		PWSTR wPathString;
		if (FAILED(res = files->GetDisplayName(SIGDN_FILESYSPATH, &wPathString)))
		{
			LOG_ERROR("File GetDisplayName failed 0x{:08X}", static_cast<unsigned long>(res));
			files->Release();
			fileDialog->Release();
			CoUninitialize();
			return false;
		}

		filePath = Logger::WideToNarrow(wPathString);

		CoTaskMemFree(wPathString);
		files->Release();
		fileDialog->Release();
		CoUninitialize();

		return true;
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

