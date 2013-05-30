// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef WORLD_H
#define WORLD_H
#pragma once

#include "MZPlayer.h"
#include "MZTexture.h"
#include "MZLua.h"

namespace MAZE
{
	class Engine;
	class Object;
	class Scene;
	class RenderBuffer;

	/**
		Game logic
	*/
	class World
	{
	public:

		/**
			Creates a new world object
			@param engine Reference to the game engine
		*/
		World(Engine* engine);

		/**
			Destroys an existing world object
		*/
		~World();

		/**
			Initialises the world
		*/
		void Init();

		/**
			Update method - runs in the main thread
			@param time Total time
			@param dt	Delta time
		*/
		void Update(float time, float dt);

		/**
			Fills the back renderbuffer - run in the main thread
		*/
		void Render(RenderBuffer* buffer);
		
	private:

		void InitScene();
		void InitScript();

	private:

		/// Reference to the engine
		Engine *mEngine;
		
		/// Scene manager
		Scene *mScene;

		/// Player
		Player *mPlayer;
		
		/// Light provided by the moon
		Light *mMoon;

		/// Lua VM
		lua_State *mScript;		
		
		/// Skybox cube map
		Resource::Ptr<Texture> mSkyTexture;
	};
};

#endif /*WORLD_H*/
