// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
#include "MZException.h"
#include "MZCamera.h"
#include "MZRenderer.h"
#include "MZRsmngr.h"
#include "MZEngine.h"
#include "MZWorld.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) 
	{
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}
		default:
		{
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
}

// ------------------------------------------------------------------------------------------------
Engine::Engine()
	: mRenderer(NULL),
	  mRsmngr(NULL),
	  mWorld(NULL),
	  mRunning(false),
	  mLastFrameTime(0.0f),
	  mMouse(0, 0),
	  mInstance(::GetModuleHandle(NULL)),
	  mWindow(NULL),
	  mDC(NULL)
{
	memset(mKeyState, 0, sizeof(mKeyState));
	::QueryPerformanceFrequency(&mFreq);
}

// ------------------------------------------------------------------------------------------------
Engine::~Engine()
{
	if (mWorld)
	{
		delete mWorld;
	}

	if (mRenderer)
	{
		mRenderer->Stop();
		delete mRenderer;
	}
	
	if (mRsmngr)
	{
		mRsmngr->Stop();
		delete mRsmngr;
	}
	
	if (mDC && !ReleaseDC(mWindow, mDC))
	{
		throw WindowsException("Engine::~Engine: ReleaseDC failed");
	}

	if (mWindow)
	{
		if (mConfig.FullScreen)
		{
			::ChangeDisplaySettings(NULL, 0);
		}

		if (!::DestroyWindow(mWindow))
		{
			throw WindowsException("Engine::~Engine: DestroyWindow failed");
		}
	}

	WNDCLASSEX wc;
	if (::GetClassInfoEx(mInstance, "MAZE", &wc))
	{
		::UnregisterClass("MAZE", mInstance);
	}
}

// ------------------------------------------------------------------------------------------------
void Engine::Init()
{
	// Create the window
	InitWindow();	

	// Initialise the renderer
	mRenderer = new Renderer(this);
	mRenderer->Init();
				
	// Initialise the  resource manager
	mRsmngr = new ResourceManager(this);
	mRsmngr->Init();

	// Initialise the world
	mWorld = new World(this);	
	mWorld->Init();
}

// ------------------------------------------------------------------------------------------------
void Engine::LoadConfig(const std::string& cfg)
{
	mConfig.WindowWidth = 1024;
	mConfig.WindowHeight = 576;
	mConfig.FullScreen = false;
	mConfig.WindowTitle = "MAZE";
	mConfig.ResourceDir = "./data";
	mConfig.Anisotropy = 16.0f;
	mConfig.TextureFilter = 2;
}

// ------------------------------------------------------------------------------------------------
void Engine::InitWindow()
{
	WNDCLASSEX wndClass;
	RECT wndRect;
	DWORD style, exStyle;
	PIXELFORMATDESCRIPTOR pfd;
	int pixelFormat;

	// Register the window class
	if (!GetClassInfoEx(mInstance, "MAZE", &wndClass))
	{
		memset(&wndClass, 0, sizeof(WNDCLASSEX));
		wndClass.cbSize = sizeof(WNDCLASSEX);
		wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wndClass.lpfnWndProc = (WNDPROC) WndProc;
		wndClass.cbClsExtra	= 0;
		wndClass.cbWndExtra	= 0;
		wndClass.hInstance = mInstance;
		wndClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndClass.hbrBackground = NULL;
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = "MAZE";

		if (!::RegisterClassEx(&wndClass))
		{
			throw WindowsException("Engine::InitWindow: RegisterClassEx failed");
		}
	}

	wndRect.left = 0;
	wndRect.top = 0;
	wndRect.bottom = mConfig.WindowHeight;
	wndRect.right = mConfig.WindowWidth;

	if (mConfig.FullScreen) 
	{		
		DEVMODE dmScreenSettings;
		::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmScreenSettings);

		dmScreenSettings.dmPelsWidth = mConfig.WindowWidth;
		dmScreenSettings.dmPelsHeight = mConfig.WindowHeight;
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		
		if (::ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			throw WindowsException("Engine::InitWindow: ChangeDisplaySettings failed");
		}

		exStyle = WS_EX_APPWINDOW;
		style = WS_POPUP;	
	} 
	else 
	{
		exStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;	
	}
	
	::AdjustWindowRectEx(&wndRect, style, FALSE, exStyle);

	// Create the window
	mWindow = ::CreateWindowEx(
		exStyle, 
		"MAZE", 
		mConfig.WindowTitle.c_str(),
		style | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0,
		wndRect.right - wndRect.left,
		wndRect.bottom - wndRect.top,
		NULL,
		NULL,
		mInstance,
		this
	);

	if (!mWindow)
	{
		throw WindowsException("Engine::InitWindow: CreateWindow failed");
	}
	
	if (!(mDC = ::GetDC(mWindow)))
	{
		throw WindowsException("Engine::InitWindow: GetDC failed");
	}

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	if (!(pixelFormat = ChoosePixelFormat(mDC, &pfd))) 
	{
		throw WindowsException("Engine::InitWindow: ChoosePixelFormat failed");
	}

	if (!SetPixelFormat(mDC, pixelFormat, &pfd)) 
	{
		throw WindowsException("Engine::InitWindow: SetPixelFormat failed");
	}
}

// ------------------------------------------------------------------------------------------------
void Engine::Quit()
{
	::PostMessage(mWindow, WM_CLOSE, NULL, NULL);
}

// ------------------------------------------------------------------------------------------------
void Engine::SetMousePos(const glm::ivec2& pos)
{
	::SetCursorPos(pos.x, pos.y);
}

// ------------------------------------------------------------------------------------------------
void Engine::MainLoop()
{
	::ShowWindow(mWindow, SW_SHOW);
	::SetForegroundWindow(mWindow);
	::SetCursorPos(mConfig.WindowWidth >> 1, mConfig.WindowHeight >> 1);
	::ShowCursor(FALSE);

	mRsmngr->Start();
	mRenderer->Start();
	
	mRunning = true;
	while (mRunning) 
	{
		MSG msg;
		POINT p;
		LARGE_INTEGER time;
		float frameBeg;
		
		::QueryPerformanceCounter(&time);
		frameBeg = time.QuadPart * 1000.0f / mFreq.QuadPart;

		// Handle events
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	
		{
			::TranslateMessage(&msg);		
			::DispatchMessage(&msg);	

			switch (msg.message)
			{
				case WM_QUIT:
				{
					mRunning = 0;
					break;
				}
				case WM_KEYDOWN:
				{
					mKeyState[msg.wParam] = true;
					break;
				}
				case WM_KEYUP:
				{
					mKeyState[msg.wParam] = false;
					break;
				}
			}
		}

		// Retrieve input device states
		::GetCursorPos(&p);
		mMouse.x = p.x;
		mMouse.y = p.y;

		// Compute frame time
		mTimeDelta = mLastFrameTime > 0 ? frameBeg - mLastFrameTime : 0.0f;

		// Update the scene
		mWorld->Update(mLastFrameTime, mTimeDelta);		
		mWorld->Render(mRenderer->GetBuffer());
		
		// Prevent CPU from burning
		mLastFrameTime = frameBeg;
		mRenderer->SwapBuffers();
	}
}
