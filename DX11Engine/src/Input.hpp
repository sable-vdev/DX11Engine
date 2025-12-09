#pragma once
#include "EngineTypes.hpp"
#include "Logger.hpp"
class Input
{
public:
	Input();
	~Input();

	static bool GetKey(I32 key);

	static void OnKeyPress(I32 key);
	static void OnKeyUp(I32 key);

	static void OnMousePress(I32 key);
	static void OnMouseDown(I32 key);

private:
	static bool m_keys[256];
	static bool m_mouseButtons[3];
	bool m_padding = false;
	static float2 m_mousePos;
	static float m_mouseDelta;

};

