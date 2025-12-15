#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "EngineTypes.hpp"
#include "Logger.hpp"

enum class KeyCode : I32
{
    None = 0x00,

    // Letters
    A = 'A',
    B = 'B',
    C = 'C',
    D = 'D',
    E = 'E',
    F = 'F',
    G = 'G',
    H = 'H',
    I = 'I',
    J = 'J',
    K = 'K',
    L = 'L',
    M = 'M',
    N = 'N',
    O = 'O',
    P = 'P',
    Q = 'Q',
    R = 'R',
    S = 'S',
    T = 'T',
    U = 'U',
    V = 'V',
    W = 'W',
    X = 'X',
    Y = 'Y',
    Z = 'Z',

    // Numbers (top row)
    Num0 = '0',
    Num1 = '1',
    Num2 = '2',
    Num3 = '3',
    Num4 = '4',
    Num5 = '5',
    Num6 = '6',
    Num7 = '7',
    Num8 = '8',
    Num9 = '9',

    // Function keys
    F1 = VK_F1,
    F2 = VK_F2,
    F3 = VK_F3,
    F4 = VK_F4,
    F5 = VK_F5,
    F6 = VK_F6,
    F7 = VK_F7,
    F8 = VK_F8,
    F9 = VK_F9,
    F10 = VK_F10,
    F11 = VK_F11,
    F12 = VK_F12,

    // Control keys
    Escape = VK_ESCAPE,
    Tab = VK_TAB,
    CapsLock = VK_CAPITAL,
    Shift = VK_SHIFT,
    LeftShift = VK_LSHIFT,
    RightShift = VK_RSHIFT,
    Ctrl = VK_CONTROL,
    LeftCtrl = VK_LCONTROL,
    RightCtrl = VK_RCONTROL,
    Alt = VK_MENU,
    LeftAlt = VK_LMENU,
    RightAlt = VK_RMENU,
    Space = VK_SPACE,
    Enter = VK_RETURN,
    Backspace = VK_BACK,

    // Arrows
    LeftArrow = VK_LEFT,
    RightArrow = VK_RIGHT,
    UpArrow = VK_UP,
    DownArrow = VK_DOWN,

    // Special
    Insert = VK_INSERT,
    DeleteKey = VK_DELETE,
    Home = VK_HOME,
    End = VK_END,
    PageUp = VK_PRIOR,
    PageDown = VK_NEXT,

    // Numpad
    Numpad0 = VK_NUMPAD0,
    Numpad1 = VK_NUMPAD1,
    Numpad2 = VK_NUMPAD2,
    Numpad3 = VK_NUMPAD3,
    Numpad4 = VK_NUMPAD4,
    Numpad5 = VK_NUMPAD5,
    Numpad6 = VK_NUMPAD6,
    Numpad7 = VK_NUMPAD7,
    Numpad8 = VK_NUMPAD8,
    Numpad9 = VK_NUMPAD9,
    NumpadAdd = VK_ADD,
    NumpadSub = VK_SUBTRACT,
    NumpadMul = VK_MULTIPLY,
    NumpadDiv = VK_DIVIDE,
    NumpadEnter = VK_RETURN,


    Left = VK_LBUTTON,
    Right = VK_RBUTTON,
    Middle = VK_MBUTTON,
};


class Input
{
public:
	Input();
	~Input();

	static bool GetKey(KeyCode key);
    static bool GetMouse(KeyCode key) { return m_mouseButtons[(int)key]; }
    static float2 GetMousePos() { return m_mousePos; };

	static void OnKeyPress(I32 key);
	static void OnKeyUp(I32 key);

	static void OnMousePress(I32 key);
	static void OnMouseUp(I32 key);

    static inline void OnMouseMove(float x, float y) { m_mousePos.x = x; m_mousePos.y = y; };
private:
	static bool m_keys[256];
	static bool m_mouseButtons[3];
	static float2 m_mousePos;
	static float m_mouseDelta;

};

