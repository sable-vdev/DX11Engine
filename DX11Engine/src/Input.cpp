#include "Input.hpp"

bool Input::m_keys[256];
bool Input::m_mouseButtons[3];
float2 Input::m_mousePos;

Input::Input()
{

	for (int i{}; i < 256; i++)
	{
		m_keys[i] = false;

		if (i < 3)
		{
			m_mouseButtons[i] = false;
		}
	}

	LOG("Input created!");
}

Input::~Input()
{
}

bool Input::GetKey(KeyCode key)
{
	return m_keys[(int)key];
}

void Input::OnKeyPress(I32 key)
{
	m_keys[key] = true;
}

void Input::OnKeyUp(I32 key)
{
	m_keys[key] = false;
}

void Input::OnMousePress(I32 key)
{
	m_mouseButtons[key] = true;
}

void Input::OnMouseUp(I32 key)
{
	m_mouseButtons[key] = false;
}
