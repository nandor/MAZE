// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZFONT_H
#define MZFONT_H
#pragma once

namespace MAZE
{
	class Font : public Resource
	{
	public:

		/// Size of the alphabet
		const static size_t SIGMA = 128 - 1 - 32;
			
		/// Individual character data
		struct Glyph
		{
			/// Offset in the texture
			glm::vec2 MapOffset;

			/// Size in the texture
			glm::vec2 MapSize;

			/// Glyph offset from reference
			glm::vec2 Offset;

			/// Glyph size
			glm::vec2 Size;

			/// Advance in the text
			size_t Advance;

		};
		
	public:
		
		/// Template related stuff
		const static Resource::Type TYPE;

		/**
			Createse a new font resource
			@param rsmngr	Resource manager
			@param id		ID of the resource
			@param height	Height of the font
			@param fn		Source file name
		*/
		Font(ResourceManager *rsmngr, const std::string& id, size_t height, const std::string& fn);
		
		/**
			Destroys the resource
		*/
		~Font();
				
		/**
			Loads the resource
		*/
		void Load();
		
		/**
			Unloads  the resource
		*/
		void Unload();

		/**
			Returns a texture of a specific size
		*/
		Glyph& GetGlyph(char chr)
		{
			return mGlyphs[chr - 32];
		}

	private:

		/// List of glyphs
		Glyph mGlyphs[SIGMA];

		/// Texture map
		MGLuint mTexture;

		/// Height of the font
		int mHeight;
		
		/// Name of the source file
		std::string mFileName;

		/// Renderer needs access
		friend class Renderer;
	};
};

#endif
