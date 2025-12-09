#include "Input.hpp"

bool Input::m_keys[256];
bool Input::m_mouseButtons[3];

Input::Input()
{

	for (int i = 0; i < 256; i++)
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

bool Input::GetKey(I32 key)
{
	return m_keys[key];
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
}

void Input::OnMouseDown(I32 key)
{
}
