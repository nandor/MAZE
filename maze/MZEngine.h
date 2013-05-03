// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef ENGINE_H
#define ENGINE_H
#pragma once

#include <string>
#include "MZPlatform.h"
#include "MZMath.h"

namespace MAZE 
{
	class Renderer;
	class ResourceManager;
	class World;

	/**
		Main class which handles the windowing and user input
	*/
	class Engine
	{	
	public:

		/**
			Configuration
		*/
		struct Config 
		{
			/// Width of the rendering window
			unsigned WindowWidth;

			/// Height of the rendering windows
			unsigned WindowHeight;

			/// Full screen or windowed
			bool FullScreen;
			
			/// Title of the window
			std::string WindowTitle;

			/// Resource directory
			std::string ResourceDir;

			/// Anisotropic filtering
			float Anisotropy;

			/// Texture filtering
			int TextureFilter;
		};

		/**
			Keyboard
		*/
		enum Key
		{
			KEY_UP		= VK_UP,
			KEY_DOWN	= VK_DOWN,
			KEY_LEFT	= VK_LEFT,
			KEY_RIGHT	= VK_RIGHT,
			KEY_A		= 'A',
			KEY_S		= 'S',
			KEY_D		= 'D',
			KEY_W		= 'W',
			KEY_ESC		= VK_ESCAPE,
			KEY_SPACE	= VK_SPACE
		};
		
	public:

		/**
			Creates a new engine object
		*/
		Engine();

		/**
			Destroys the engine object
		*/
		~Engine();

		/**
			Loads configuration from a file
			@param cfg Configuration file name
		*/
		void LoadConfig(const std::string& cfg);

		/**
			Initializes the engine
		*/
		void Init();

		/**
			Starts the main loop
		*/
		void MainLoop();
		
		/**
			Sends a quit request to the engine
		*/
		void Quit();

		/**
			Returns a handle to the window
		*/
		HWND GetHandle()
		{
			return mWindow;
		}

		/**
			Returns the device context of the window
		*/
		HDC GetDC()
		{
			return mDC;
		}

		/**
			Returns the current configuration
		*/
		Config& GetConfig() 
		{
			return mConfig;
		}

		/**
			Returns the resource manager
		*/
		ResourceManager* GetResourceManager()
		{
			return mRsmngr;
		}

		/**
			Returns the renderer
		*/
		Renderer* GetRenderer()
		{
			return mRenderer;
		}

		/**
			Returns time difference
		*/
		float GetTimeDelta()
		{
			return mTimeDelta;
		}

		/**
			Returns the mouse position
		*/
		glm::ivec2& GetMousePos()
		{
			return mMouse;
		}

		/**
			Sets the mouse position
		*/
		void SetMousePos(const glm::ivec2& pos);

		/**
			Check if a key was pressed
		*/
		bool IsKeyDown(Key k)
		{
			return mKeyState[k];
		}

	private:
		
		/**
			Initializes the render window
		*/
		void InitWindow();

	private:

		/// Rendering subsytem
		Renderer* mRenderer;

		/// Resource manager
		ResourceManager* mRsmngr;

		/// Game world
		World* mWorld;

		/// Configuration data
		Config mConfig;

		/// Loop state
		bool mRunning;

		/// Last frame time
		float mLastFrameTime;

		/// Delta time
		float mTimeDelta;

		/// Mouse coordinates
		glm::ivec2 mMouse;

		/// Keyboard state
		bool mKeyState[256];

		/// Timer
		LARGE_INTEGER mFreq;

		/// Module instance
		HINSTANCE mInstance;

		/// Window handle
		HWND mWindow;
		
		/// Device context
		HDC mDC;
	};
};

#endif /*ENGINE_H*/