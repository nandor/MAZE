// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2013 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Engine * e;

	if (msg == WM_CREATE) 
	{
		if (!(e = (Engine*)((CREATESTRUCT*)lParam)->lpCreateParams)) 
		{
			return ::DefWindowProc(hwnd, msg, wParam, lParam);
		}
		
		::SetWindowLong(hwnd, GWL_USERDATA, (LONG)e);
	}
	else if (!(e = (Engine*)::GetWindowLong(hwnd, GWL_USERDATA))) 
	{
		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}

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
	  mNetwork(NULL),
	  mWorld(NULL),
	  mDevice(NULL),
	  mContext(NULL),
	  mRunning(false),
	  mFocus(true),
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
	ShowWindow(mWindow, SW_HIDE);

	if (mWorld)
	{
		delete mWorld;
		mWorld = NULL;
	}

	if (mNetwork)
	{
		mNetwork->Stop();
		delete mNetwork;
		mNetwork = NULL;
	}

	if (mRenderer)
	{
		mRenderer->Stop();
		delete mRenderer;
		mRenderer = NULL;
	}
	
	if (mRsmngr)
	{
		mRsmngr->Stop();
		delete mRsmngr;
		mRsmngr = NULL;
	}
		
	if (mContext)
	{
		alcMakeContextCurrent(NULL);
		alcDestroyContext(mContext);
		mContext = NULL;
	}

	if (mDevice)
	{
		alcCloseDevice(mDevice);
		mDevice = NULL;
	}

	if (mDC && !ReleaseDC(mWindow, mDC))
	{
		throw WindowsException("Engine::~Engine: ReleaseDC failed");
	}

	if (mWindow)
	{
		if (mSetup.FullScreen)
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
	InitWindow();	
	InitSound();

	// Initialise the network
	mNetwork = new Network(this);
	mNetwork->Init();

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
	Config config;

	config.Read(cfg);
	mSetup.WindowWidth   = config["window"]["width"].AsInt(1024);
	mSetup.WindowHeight  = config["window"]["height"].AsInt(576);
	mSetup.FullScreen    = config["window"]["fullscreen"].AsBool(false);
	mSetup.WindowTitle	 = config["window"]["title"].AsString("MAZE");
	mSetup.ResourceDir   = config["rsmngr"]["dir"].AsString(".\\data");
	mSetup.Anisotropy    = config["gfx"]["anisotropy"].AsFloat(0.0f);
	mSetup.TextureFilter = config["gfx"]["texture"].AsInt(0);
	mSetup.ViewDistance  = config["gfx"]["viewDistance"].AsFloat(50.0f);
	mSetup.EnableShadows = config["gfx"]["shadows"].AsBool(false);
	mSetup.EnableDOF	 = config["gfx"]["dof"].AsBool(false);
	mSetup.EnableFog	 = config["gfx"]["fog"].AsBool(false);
	mSetup.Server		 = config["network"]["server"].AsString("127.0.0.1");
	mSetup.Port			 = config["network"]["port"].AsInt(12345);
	mSetup.CacheDir		 = config["cache"]["dir"].AsString(".\\cache");
	mSetup.CachePrograms = config["cache"]["programs"].AsBool(false);
	
	CreateDirectory(mSetup.ResourceDir.c_str(), NULL);
	CreateDirectory(mSetup.CacheDir.c_str(), NULL);
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
	wndRect.bottom = mSetup.WindowHeight;
	wndRect.right = mSetup.WindowWidth;

	if (mSetup.FullScreen) 
	{		
		DEVMODE dmScreenSettings;
		::EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dmScreenSettings);

		dmScreenSettings.dmPelsWidth = mSetup.WindowWidth;
		dmScreenSettings.dmPelsHeight = mSetup.WindowHeight;
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
		mSetup.WindowTitle.c_str(),
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
	pfd.cDepthBits = 16;
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
void Engine::InitSound()
{
	if ((mDevice = alcOpenDevice(NULL)) == NULL)
	{
		throw Exception("Cannot create OpenAL device");
	}

	if ((mContext = alcCreateContext(mDevice, NULL)) == NULL)
	{
		throw Exception("Cannot create OpenAL context");
	}

	alcMakeContextCurrent(mContext);
}

// ------------------------------------------------------------------------------------------------
void Engine::Quit()
{
	::PostMessage(mWindow, WM_CLOSE, 0, 0);
}

// ------------------------------------------------------------------------------------------------
bool Engine::IsFocused()
{
	return mWindow == ::GetFocus();
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
	::SetCursorPos(mSetup.WindowWidth >> 1, mSetup.WindowHeight >> 1);
	::ShowCursor(FALSE);
	
	mRsmngr->Start();
	mRenderer->Start();
	mNetwork->Start();
	
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
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	
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

					if (msg.wParam == VK_ESCAPE)
					{
						mRunning = 0;
						return;
					}
					
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

		mLastFrameTime = frameBeg;
		mRenderer->SwapBuffers();
	}
}
