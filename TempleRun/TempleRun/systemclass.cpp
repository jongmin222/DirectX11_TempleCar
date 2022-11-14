////////////////////////////////////////////////////////////////////////////////
// Filename: systemclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "systemclass.h"
//콘솔 창 출력
#pragma comment(linker , "/entry:WinMainCRTStartup /subsystem:console")

SystemClass::SystemClass()
{
	m_Input = 0;
	m_Graphics = 0;
	m_Fps = 0;
	m_Cpu = 0;
	m_Timer = 0;
	m_Sound = 0;
}


SystemClass::SystemClass(const SystemClass& other)
{
}


SystemClass::~SystemClass()
{
}


bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;


출처: https://kukuta.tistory.com/142 [HardCore in Programming]

	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 1600;
	screenHeight =900;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	m_Input = new InputClass;
	if(!m_Input)
	{
		return false;
	}

	// Initialize the input object.
	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	m_Graphics = new GraphicsClass;
	if(!m_Graphics)
	{
		return false;
	}

	// Initialize the graphics object.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if(!result)
	{
		return false;
	}
	// Create the fps object.
	m_Fps = new FpsClass;
	if (!m_Fps)
	{
		return false;
	}

	// Initialize the fps object.
	m_Fps->Initialize();
	// Create the cpu object.
	m_Cpu = new CpuClass;
	if (!m_Cpu)
	{
		return false;
	}

	// Initialize the cpu object.
	m_Cpu->Initialize();
	// Create the timer object.
	m_Timer = new TimerClass;
	if (!m_Timer)
	{
		return false;
	}

	// Initialize the timer object.
	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	// Create the sound object.
	m_Sound = new SoundClass;
	if (!m_Sound)
	{
		return false;
	}

	// Initialize the sound object.
	result = m_Sound->Initialize(m_hwnd);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize Direct Sound.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void SystemClass::Shutdown()
{
	// Release the graphics object.
	if(m_Graphics)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = 0;
	}

	// Release the input object.
	if(m_Input)
	{
		delete m_Input;
		m_Input = 0;
	}

	// Release the timer object.
	if (m_Timer)
	{
		delete m_Timer;
		m_Timer = 0;
	}

	// Release the cpu object.
	if (m_Cpu)
	{
		m_Cpu->Shutdown();
		delete m_Cpu;
		m_Cpu = 0;
	}

	// Release the fps object.
	if (m_Fps)
	{
		delete m_Fps;
		m_Fps = 0;
	}

	// Release the sound object.
	if (m_Sound)
	{
		m_Sound->Shutdown();
		delete m_Sound;
		m_Sound = 0;
	}

	// Shutdown the window.
	ShutdownWindows();
	
	return;
}

void SystemClass::GetKeyState()
{
	if (m_Input->m_mouseLeft == true)
		m_Graphics->MouseLClick = true;
	else
		m_Graphics->MouseLClick = false;


	if (m_Input->IsKeyDown(DIK_1)&& m_Input->GetCurrentKey(DIK_1) == false)
	{
		m_Input->SetCurrentKeyDown(DIK_1);
		if (m_Graphics->RenderCollider == false)
			m_Graphics->RenderCollider = true;
		else if (m_Graphics->RenderCollider == true)
			m_Graphics->RenderCollider = false;
	}
	else if(m_Input->IsKeyDown(DIK_1)==false && m_Input->GetCurrentKey(DIK_1) == true)
	{
		m_Input->SetCurrentKeyUp(DIK_1);
	}

	if (m_Input->IsKeyDown(DIK_2) && m_Input->GetCurrentKey(DIK_2) == false)
	{
		m_Input->SetCurrentKeyDown(DIK_2);
		if (m_Graphics->RenderFog == false)
			m_Graphics->RenderFog = true;
		else if (m_Graphics->RenderFog == true)
			m_Graphics->RenderFog = false;
	}
	else if (m_Input->IsKeyDown(DIK_2) == false && m_Input->GetCurrentKey(DIK_2) == true)
	{
		m_Input->SetCurrentKeyUp(DIK_2);
	}

	//pauseUI
	if (m_Graphics->Pause == true && m_Input->ClickResume == true)
	{
		m_Graphics->Play = true;
		m_Timer->Play = true;
		m_Graphics->Pause = false;
	}
	else if (m_Graphics->Pause == true && m_Input->ClickRestart == true)
	{
		m_Graphics->Restart();
		m_Timer->Restart();
		m_Graphics->Play = true;
		m_Timer->Play = true;
		m_Graphics->Pause = false;
	}
	else if (m_Graphics->Pause == true && m_Input->ClickMain == true)
	{
		m_Graphics->Restart();
		m_Timer->Restart();
		m_Graphics->Main = true;
		m_Graphics->Pause = false;
	}

	//mainUI
	if (m_Graphics->Main == true && m_Input->ClickStart == true)
	{
		m_Graphics->Main = false;
		m_Graphics->Play = true;
		m_Timer->Play = true;
	}
	else if (m_Graphics->Main == true && m_Input->ClickTutorial == true)
	{
		//미정
	}

	//successUI
	if (m_Graphics->Success == true)
	{
		m_Graphics->Play = false;
		m_Timer->Play = false;
	}

	if (m_Graphics->Success == true && m_Input->ClickRestart == true)
	{
		m_Graphics->Restart();
		m_Timer->Restart();
		m_Graphics->Play = true;
		m_Timer->Play = true;
		m_Graphics->Success = false;
	}
	else if (m_Graphics->Success == true && m_Input->ClickMain == true)
	{
		m_Graphics->Restart();
		m_Timer->Restart();
		m_Graphics->Main = true;
		m_Graphics->Success = false;
	}

	//failureUI
	if (m_Graphics->Failure == true)
	{
		m_Graphics->Play = false;
		m_Timer->Play = false;
	}

	if (m_Graphics->Failure == true && m_Input->ClickRestart == true)
	{
		m_Graphics->Restart();
		m_Timer->Restart();
		m_Graphics->Play = true;
		m_Timer->Play = true;
		m_Graphics->Failure = false;
	}
	else if (m_Graphics->Failure == true && m_Input->ClickMain == true)
	{
		m_Graphics->Restart();
		m_Timer->Restart();
		m_Graphics->Main = true;
		m_Graphics->Failure = false;
	}


	//pause -> play
	// play -> pause
	if (m_Input->IsKeyDown(DIK_P) && m_Input->GetCurrentKey(DIK_P) == false)
	{
		m_Input->SetCurrentKeyDown(DIK_P);
		if (m_Graphics->Pause == true && m_Graphics->Main == false && m_Graphics->Success == false && m_Graphics->Failure == false)
		{
			m_Graphics->Play = true;
			m_Timer->Play = true;

			m_Graphics->Pause = false;
		}
		else if (m_Graphics->Pause == false && m_Graphics->Main == false && m_Graphics->Success == false && m_Graphics->Failure == false)
		{
			m_Graphics->Play = false;
			m_Timer->Play = false;

			m_Graphics->Pause = true;
		}
	}
	else if (m_Input->IsKeyDown(DIK_P) == false && m_Input->GetCurrentKey(DIK_P) == true)
	{
		m_Input->SetCurrentKeyUp(DIK_P);
	}

	//pause->restart,play
	//if (m_Input->IsKeyDown(DIK_4) ==true&& m_Input->GetCurrentKey(DIK_4) == false)
	//{
	//	m_Input->SetCurrentKeyDown(DIK_4);
	//	if (m_Graphics->Main == false)
	//	{
	//		m_Graphics->Restart();
	//		m_Timer->Restart();

	//		m_Graphics->Play = true;
	//		m_Timer->Play = true;
	//		m_Graphics->Pause = false;
	//	}
	//}
	//else if (m_Input->IsKeyDown(DIK_4) == false && m_Input->GetCurrentKey(DIK_4) == true)
	//{
	//	m_Input->SetCurrentKeyUp(DIK_4);
	//}


	//main -> play
	// play,pause -> main
	//if (m_Input->IsKeyDown(DIK_5) && m_Input->GetCurrentKey(DIK_5) == false)
	//{
	//	m_Input->SetCurrentKeyDown(DIK_5);
	//	if (m_Graphics->Main == true)
	//	{
	//		m_Graphics->Main = false;	
	//		m_Graphics->Play = true;
	//		m_Timer->Play = true;
	//	}
	//	else if (m_Graphics->Main == false)
	//	{
	//		m_Graphics->Restart();
	//		m_Timer->Restart();
	//		m_Graphics->Main = true;
	//		m_Graphics->Play = false;
	//		m_Timer->Play = false;
	//	}
	//}
	//else if (m_Input->IsKeyDown(DIK_5) == false && m_Input->GetCurrentKey(DIK_5) == true)
	//{
	//	m_Input->SetCurrentKeyUp(DIK_5);
	//}

	if (m_Input->IsKeyDown(DIK_W))
			m_Graphics->CameraMoveFront = true;
	else
		m_Graphics->CameraMoveFront = false;

	if (m_Input->IsKeyDown(DIK_S))
		m_Graphics->CameraMoveBack = true;
	else
		m_Graphics->CameraMoveBack = false;

	if (m_Input->IsKeyDown(DIK_A))
		m_Graphics->CameraMoveLeft = true;
	else
		m_Graphics->CameraMoveLeft = false;

	if (m_Input->IsKeyDown(DIK_D))
		m_Graphics->CameraMoveRight = true;
	else
		m_Graphics->CameraMoveRight = false;

	if (m_Input->IsKeyDown(DIK_Q))
		m_Graphics->CameraMoveUp = true;
	else
		m_Graphics->CameraMoveUp = false;

	if (m_Input->IsKeyDown(DIK_E))
		m_Graphics->CameraMoveDown = true;
	else
		m_Graphics->CameraMoveDown = false;

	if (m_Input->IsKeyDown(DIK_UP))
		m_Graphics->MoveFront = true;
	else
		m_Graphics->MoveFront = false;

	if (m_Input->IsKeyDown(DIK_DOWN))
		m_Graphics->MoveBack = true;
	else
		m_Graphics->MoveBack = false;

	if (m_Input->IsKeyDown(DIK_LEFT))
	{
		if (m_Input->IsKeyDown(DIK_LSHIFT))
		{
			m_Graphics->TurnLeft = true;
			m_Graphics->MoveLeft = false;
		}
		else
		{
			m_Graphics->MoveLeft = true;
			m_Graphics->TurnLeft = false;
		}
	}
	else
	{
		m_Graphics->MoveLeft = false;
		m_Graphics->TurnLeft = false;
	}

	if (m_Input->IsKeyDown(DIK_RIGHT))
	{
		if (m_Input->IsKeyDown(DIK_LSHIFT))
		{
			m_Graphics->TurnRight = true;
			m_Graphics->MoveRight = false;
		}
		else
		{
			m_Graphics->MoveRight = true;
			m_Graphics->TurnRight = false;
		}
	}
	else
	{
		m_Graphics->MoveRight = false;
		m_Graphics->TurnRight = false;
	}

	if (m_Input->IsKeyDown(DIK_SPACE))
		m_Graphics->MoveJump = true;
	else
		m_Graphics->MoveJump = false;

	//TestMode
	if (m_Input->IsKeyDown(DIK_LALT) && m_Input->IsKeyDown(DIK_T) && m_Input->GetCurrentKey(DIK_T) == false)
	{
		m_Input->SetCurrentKeyDown(DIK_T);
		if (m_Graphics->TestMode == false)
			m_Graphics->TestMode = true;
		else if (m_Graphics->TestMode == true)
			m_Graphics->TestMode = false;
	}
	else if (m_Input->IsKeyDown(DIK_LALT) == false && m_Input->IsKeyDown(DIK_T) == false && m_Input->GetCurrentKey(DIK_T) == true)
	{
		m_Input->SetCurrentKeyUp(DIK_T);
	}



	if (m_Input->IsKeyDown(DIK_LCONTROL))
		m_Graphics->MoveSlide = true;
	else
		m_Graphics->MoveSlide = false;

	if (m_Graphics->GetCoin == true)
		m_Sound->PlayGetCoinSound();

	return;
}



void SystemClass::Run()
{
	MSG msg;
	bool done, result;


	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Loop until there is a quit message from the window or the user.
	done = false;
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			result = Frame();
			if(!result)
			{
				done = true;
			}
		}

	}

	return;
}


bool SystemClass::Frame()
{
	bool result;

	m_Input->Frame();
	m_Timer->Frame();
	m_Fps->Frame();
	m_Cpu->Frame();

	GetKeyState();
	// Check if the user pressed escape and wants to exit the application.
	if(m_Input->IsKeyDown(DIK_ESCAPE))
	{
		return false;
	}

	// Do the frame processing for the graphics object.
	result = m_Graphics->Frame(m_Fps->GetFps(), m_Cpu->GetCpuPercentage(), m_Timer->GetFrameTime(), m_Timer->GetStartTime(), m_Input->GetMouseVariance(), m_Input->GetMousePosition());
	if (!result)
	{
		return false;
	}

	return true;
}


LRESULT CALLBACK SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	switch(umsg)
	{
		// Check if a key has been pressed on the keyboard.
		case WM_KEYDOWN:
		{
			// If a key is pressed send it to the input object so it can record that state.
			m_Input->KeyDown((unsigned int)wparam);
			return 0;
		}

		// Check if a key has been released on the keyboard.
		case WM_KEYUP:
		{
			// If a key is released then send it to the input object so it can unset the state for that key.
			m_Input->KeyUp((unsigned int)wparam);
			return 0;
		}

		// Any other messages send to the default message handler as our application won't make use of them.
		default:
		{
			return DefWindowProc(hwnd, umsg, wparam, lparam);
		}
	}
}


void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;


	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	m_hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	m_applicationName = L"TempleCar";

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = m_hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = posY = 0;
	}
	else
	{
		// If windowed then set it to 1600x900 resolution.
		screenWidth  = 1600;
		screenHeight = 900;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}



	// Create the window with the screen settings and get the handle to it.
	m_hwnd = CreateWindowEx(WS_EX_APPWINDOW, m_applicationName, m_applicationName, 
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
						    posX, posY, screenWidth, screenHeight, NULL, NULL, m_hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(m_hwnd, SW_SHOW);
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);

	return;
}


void SystemClass::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// Remove the window.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;

	return;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}