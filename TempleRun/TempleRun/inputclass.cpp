////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "inputclass.h"


InputClass::InputClass()
{
	m_directInput = 0;
	m_keyboard = 0;
	m_mouse = 0;
	for (int i = 0; i < 256; i++)
		CurrentKey[i] = false;

	ClickStart = false;
	ClickTutorial = false;
	ClickResume = false;
	ClickRestart = false;
	ClickMain = false;

}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}


bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;


	// Store the screen size which will be used for positioning the mouse cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;
	
	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the main direct input interface.
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Initialize the direct input interface for the keyboard.
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the keyboard to not share with other programs.
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Now acquire the keyboard.
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		//return false;
	}

	// Initialize the direct input interface for the mouse.
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Acquire the mouse.
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		//return false;
	}

	return true;
}


void InputClass::Shutdown()
{
	// Release the mouse.
	if (m_mouse)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = 0;
	}

	// Release the keyboard.
	if (m_keyboard)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = 0;
	}

	// Release the main interface to direct input.
	if (m_directInput)
	{
		m_directInput->Release();
		m_directInput = 0;
	}

	return;
}


bool InputClass::Frame()
{
	bool result;


	// Read the current state of the keyboard.
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// Read the current state of the mouse.
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	// Process the changes in the mouse and keyboard.
	ProcessInput();
	UIClick();

	return true;
}


bool InputClass::ReadKeyboard()
{
	HRESULT result;


	// Read the keyboard device.
	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}


bool InputClass::ReadMouse()
{
	HRESULT result;


	// Read the mouse device.
	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputClass::UIClick()
{
	if (m_mouseLeft == true)
	{
		if (m_mouseX > m_screenWidth / 2 - 50 && m_mouseX < m_screenWidth / 2 + 50 &&
			m_mouseY > m_screenHeight / 2 - 40 && m_mouseY < m_screenHeight / 2)
		{
			ClickStart = true;
		}
		else if (m_mouseX > m_screenWidth / 2 - 80 && m_mouseX < m_screenWidth / 2 + 80 &&
			m_mouseY > m_screenHeight / 2 && m_mouseY < m_screenHeight / 2+40)
		{
			ClickTutorial = true;
		}

		if (m_mouseX > m_screenWidth / 2 - 60 && m_mouseX < m_screenWidth / 2 + 60 &&
			m_mouseY > m_screenHeight / 2 - 60 && m_mouseY < m_screenHeight / 2 - 20)
		{
			ClickResume = true;
		}
		else if (m_mouseX > m_screenWidth / 2 - 70 && m_mouseX < m_screenWidth / 2 + 70 &&
			m_mouseY > m_screenHeight / 2 - 20 && m_mouseY < m_screenHeight / 2 + 20)
		{
			ClickRestart = true;
		}
		else if (m_mouseX > m_screenWidth / 2 - 40 && m_mouseX < m_screenWidth / 2 + 40 &&
			m_mouseY > m_screenHeight / 2 + 20 && m_mouseY < m_screenHeight / 2 + 60)
		{
			ClickMain = true;
		}
	}
	else
	{
		ClickStart = false;
		ClickTutorial = false;
		ClickResume = false;
		ClickRestart = false;
		ClickMain = false;
	}
	return;
}

void InputClass::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	m_mouseLeft = m_mouseState.rgbButtons[0];
	m_mouseRight = m_mouseState.rgbButtons[1];

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	return;
}

POINT InputClass::GetMousePosition()
{
	POINT pt;
	pt.x = m_mouseX;
	pt.y = m_mouseY;

	return pt;
}

POINT InputClass::GetMouseVariance()
{
	POINT pt;
	pt.x = m_mouseState.lX;
	pt.y = m_mouseState.lY;

	return pt;
}

void InputClass::KeyDown(unsigned char key)
{
	// If a key is pressed then save that state in the key array.
	m_keyboardState[key] = true;
	return;
}
void InputClass::KeyUp(unsigned char key)
{
	// If a key is released then clear that state in the key array.
	m_keyboardState[key] = false;
	return;
}
bool InputClass::IsKeyDown(unsigned char key)
{
	// Return what state the key is in (pressed/not pressed).
	return m_keyboardState[key];
}

void InputClass::SetCurrentKeyDown(unsigned char key)
{
	// If a key is pressed then save that state in the key array.
	CurrentKey[key] = true;
	return;
}
void InputClass::SetCurrentKeyUp(unsigned char key)
{
	// If a key is released then clear that state in the key array.
	CurrentKey[key] = false;
	return;
}
bool InputClass::GetCurrentKey(unsigned char key)
{
	// Return what state the key is in (pressed/not pressed).
	return CurrentKey[key];
}