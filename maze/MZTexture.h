// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef TEXTURE_H
#define TEXTURE_H
#pragma once

#include <string>
#include <vector>
#include "MZPlatform.h"
#include "MZResource.h"
#include "MZGL.h"

namespace MAZE
{
	class Texture : public Resource
	{
	public:

		/**
			List of different texture types
		*/
		enum TextureType
		{
			TEX_1D		= MGL_TEXTURE_1D,
			TEX_2D		= MGL_TEXTURE_2D,
			TEX_3D		= MGL_TEXTURE_3D,
			TEX_CUBEMAP = MGL_TEXTURE_CUBE_MAP
		};
		
		/**
			List of texture filters
		*/
		enum Filter
		{
			DEFAULT		= 0,
			BILINEAR	= 1,
			TRILINEAR	= 2
		};
		
		/// Template related stuff
		const static Resource::Type TYPE = Resource::TEXTURE;

	private:
		
		/**
			Image data which forms the texture
		*/
		class Image
		{
		public:

			/**
				Creates a blank image
			*/
			Image();

			/**
				Destroys the image
			*/
			~Image();

			/**
				Loads the image from the disk
			*/
			void Load();

			/**
				Unload the image
			*/
			void Unload();

			/**
				Upload the image to the GPU
			*/
			void Upload(MGLenum target, size_t level = 0);

		private:

			/**
				Loads a png texture
			*/
			void LoadPNG();
			
			/**
				Loads a jpg texture
			*/
			void LoadJPG();

		public:

			/// Source file name
			std::string FileName;
			
			/// Image type
			MGLuint Type;

			/// Image data
			unsigned char *Data;

			/// Width of the image
			unsigned Width;

			/// Height of the image
			unsigned Height;

			/// Bit depth
			unsigned BitDepth;

			/// Color channels
			unsigned Channels;
		};

	public:

		/**
			Creates a new texture
			@param rsmngr	Resource manager
			@param id		ID of the texture
			@param type		Type of the texture
		*/
		Texture(ResourceManager* rsmngr, const std::string& id, TextureType type);

		/**
			Destroys the texture
		*/
		~Texture();
		
		/**
			Adds an image to the texture
			@param name Name of the source image
		*/
		void AddImage(const std::string& name);

		/**
			Loads texture data from the disk
		*/
		void Load();

		/**
			Unloads the texture
		*/
		void Unload();

		/**
			Builds the texture
		*/
		void Build();

		/**
			Destroys the texture
		*/
		void Destroy();

		/**
			Binds the texture to the OpenGL context
		*/
		void Bind();
						
	private:
		
		/// Type of the texture
		TextureType mType;

		/// List of images
		std::vector<Image> mImages;

		/// Texture target
		MGLuint mTexture;

		/// Anisotropy level
		float mAnisotropy;

		/// Texture filter
		Filter mFilter;

		/// Mipmaps
		bool mMipmaps;
		
		friend class Resource::Ptr<Texture>;
		friend class Renderer;
	};
};

#endif
