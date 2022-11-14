////////////////////////////////////////////////////////////////////////////////
// Filename: inputclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_


///////////////////////////////
// PRE-PROCESSING DIRECTIVES //
///////////////////////////////
#define DIRECTINPUT_VERSION 0x0800


/////////////
// LINKING //
/////////////
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")


//////////////
// INCLUDES //
//////////////
#include <dinput.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: InputClass
////////////////////////////////////////////////////////////////////////////////
class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	POINT GetMousePosition();
	POINT GetMouseVariance();

	void UIClick();

	void KeyDown(unsigned char);
	void KeyUp(unsigned char);
	bool IsKeyDown(unsigned char);
	void SetCurrentKeyDown(unsigned char);
	void SetCurrentKeyUp(unsigned char);
	bool GetCurrentKey(unsigned char);

	bool m_mouseLeft, m_mouseRight;

	bool ClickStart, ClickTutorial, ClickResume, ClickRestart, ClickMain;
private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	unsigned char CurrentKey[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	float m_mouseX, m_mouseY;

};

#endif