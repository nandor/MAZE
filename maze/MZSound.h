// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2013 The MAZE project. All rights reserved.

#ifndef MZSOUND_H
#define MZSOUND_H
#pragma once

namespace MAZE
{
	class Sound : public Resource
	{
	public:

		/// Template related stuff
		const static Resource::Type TYPE;
		
	public:

		/**
			Crates a new model
			@param rsmngr Resource Manager
			@param id ID of the model
			@param fn File Name
		*/
		Sound(ResourceManager* rsmngr, const std::string& id, const std::string& fn);

		/**
			Destroys the model
		*/
		~Sound();

		/**	
			Retrieves the buffer object
		*/
		ALuint GetBuffer()
		{
			return mBuffer;
		}

		/**
			Loads & decodes the sound file
		*/
		void Load();

		/**
			Unloads the sound file
		*/
		void Unload();

	private:

		/// Name of the input file
		std::string mFileName;

		/// Sound format
		ALuint mFormat;

		/// Sound frequency
		unsigned mFrequency;

		/// OpenAL buffer
		ALuint mBuffer;
				
	};
};

#endif
