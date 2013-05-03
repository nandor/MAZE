// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef WORLD_H
#define WORLD_H
#pragma once

#include "MZScene.h"
#include "MZPlayer.h"
#include "MZTexture.h"

namespace MAZE
{
	class Engine;
	class Script;
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
			Generate the world
		*/
		void Generate();

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

		/// Reference to the engine
		Engine *mEngine;
		
		/// Scene manager
		Scene *mScene;

		/// Script state
		Script *mScript;
		
		/// Player
		Player *mPlayer;

		/// Size of the world
		glm::vec3 mSize;

		/// Light provided by the moon
		Light *mMoon;

		/// Light provided by the sun
		Light *mSun;

		/// Skybox cube map
		Resource::Ptr<Texture> mSkyTexture;
	};
};

#endif /*WORLD_H*/
