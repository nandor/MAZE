// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include <fstream>
#include <jpeglib.h>
#include <png.h>

#include "MZPlatform.h"
#include "MZEngine.h"
#include "MZRenderer.h"
#include "MZRsmngr.h"
#include "MZTexture.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
const MGLuint CUBE_FACES[] =
{
	MGL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	MGL_TEXTURE_CUBE_MAP_POSITIVE_X,
	MGL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	MGL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	MGL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	MGL_TEXTURE_CUBE_MAP_POSITIVE_Z
};

// ------------------------------------------------------------------------------------------------
Texture::Image::Image()
	: FileName(""),
	  Type(MGL_RGBA),
	  Data(NULL),
	  Width(0),
	  Height(0)
{
}

// ------------------------------------------------------------------------------------------------
Texture::Image::~Image()
{
	if (Data != NULL)
	{
		delete[] Data;
		Data = NULL;
	}
}

// ------------------------------------------------------------------------------------------------
void Texture::Image::Load()
{
	if (FileName.size() > 3 && FileName.substr(FileName.size() - 3) == "png")
	{
		LoadPNG();
		return;
	}

	if (FileName.size() > 3 && FileName.substr(FileName.size() - 3) == "jpg")
	{
		LoadJPG();
		return;
	}

	throw Exception("Invalid texture type: " + FileName);
}

// ------------------------------------------------------------------------------------------------
void Texture::Image::LoadPNG()
{
	FILE* fin = NULL;
	png_byte pngsig[8];
	png_structp pngPtr;
	png_infop infoPtr;
	png_bytep* rows = NULL;
	png_uint_32 rowBytes;

	if (!(fin = fopen(FileName.c_str(), "rb")))
		throw Exception("File not found: '" + FileName + "'");

	// Check the file header
	if (fread(pngsig, 1, sizeof(pngsig), fin) != sizeof(pngsig))
	{
		fclose(fin);
		throw Exception("Cannot read PNG header");
	}

	if (png_sig_cmp(pngsig, 0, sizeof(pngsig)) != 0)
	{
		fclose(fin);
		throw Exception("Invalid PNG file: '" + FileName + "'");
	}

	// Initialise libpng
	if (!(pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)))
	{
		fclose(fin);
		throw Exception("Cannot initialise PNG read struct");
	}

	if (!(infoPtr = png_create_info_struct(pngPtr)))
	{
		png_destroy_read_struct(&pngPtr, NULL, NULL);
		fclose(fin);
		throw Exception("Cannot initialise PNG info struct");
	}
	
	if (setjmp(png_jmpbuf(pngPtr)))
	{
		png_destroy_read_struct(&pngPtr, &infoPtr, NULL);

		if (rows != NULL)
		{
			delete[] rows;
			rows = NULL;
		}

		fclose(fin);
		throw Exception("Cannot load PNG: '" + FileName + "'");
	}

	// Read the header
	png_init_io(pngPtr, fin);
	png_set_sig_bytes(pngPtr, sizeof(pngsig));
	png_read_info(pngPtr, infoPtr);

	// Extract data from header
	Width = png_get_image_width(pngPtr, infoPtr);
	Height = png_get_image_height(pngPtr, infoPtr);
	BitDepth = png_get_bit_depth(pngPtr, infoPtr);
	Channels = png_get_channels(pngPtr, infoPtr);
	
	switch (png_get_color_type(pngPtr, infoPtr))
	{
		/**
			Grayscale images can have a bit depth of 1, 2, 4 or 16
			so we expand them to 8 bits
		*/
		case PNG_COLOR_TYPE_GRAY:
		{
			Type = MGL_LUMINANCE;

			if (BitDepth < 8)
			{
				png_set_expand_gray_1_2_4_to_8(pngPtr);
				BitDepth = 8;
			}

			break;
		}

		/**
			Grayscale images with an alpha channel
			have a bit depth of 8 or 16
		*/
		case PNG_COLOR_TYPE_GRAY_ALPHA:
		{
			Type = MGL_LUMINANCE_ALPHA;
			break;
		}

		/**
			Paletted images will be expanded to RGB
		*/
		case PNG_COLOR_TYPE_PALETTE:
		{
			Type = MGL_RGB;
            Channels = 3;           

            png_set_palette_to_rgb(pngPtr);

            break;
		}
		
		/**
			RGB image need no processing
		*/
		case PNG_COLOR_TYPE_RGB:
		{
			Type = MGL_RGB;
			break;
		}

		/**
			RGBA images need no processing
		*/
		case PNG_COLOR_TYPE_RGB_ALPHA:
		{
			Type = MGL_RGBA;
			break;
		}
	}

	// Convert transparency to an alpha channel
	if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(pngPtr);
		Channels += 1;
	}
	
	// Convert 16 bpp to 8 bpp
	if (BitDepth == 16)
	{
		png_set_strip_16(pngPtr);
		BitDepth = 8;
	}
		
	png_read_update_info(pngPtr, infoPtr);

	// Allocate memory for the image
	rowBytes = ::png_get_rowbytes(pngPtr, infoPtr);
	rows = new png_bytep[Height];
	Data = new unsigned char[rowBytes * Height];

	for (size_t i = 0; i < Height; ++i)
		rows[i] = ((png_bytep)Data) + (i * rowBytes);

	// Read the image
	png_read_image(pngPtr, rows);
	delete[] rows;
		
	// Cleanup
	png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
	fclose(fin);
}

// ------------------------------------------------------------------------------------------------
void Texture::Image::LoadJPG()
{	
	FILE* fin = NULL;
	struct jpeg_decompress_struct info;
	struct jpeg_error_mgr err; 
	
	if (!(fin = fopen(FileName.c_str(), "rb")))
		throw Exception("File not found: '" + FileName + "'");
	
	info.err = jpeg_std_error(&err);
	jpeg_create_decompress(&info);
	jpeg_stdio_src(&info, fin);
	jpeg_read_header(&info, TRUE);
	
	jpeg_start_decompress(&info);

	Width = info.output_width;
	Height = info.output_height;
	Channels = info.output_components;
	BitDepth = Channels << 3;
	
	switch (Channels)
	{
		case 1:
		{
			Type = MGL_LUMINANCE;
			break;
		}
		case 2:
		{
			Type = MGL_LUMINANCE_ALPHA;
			break;
		}
		case 3:
		{
			Type = MGL_RGB;
            break;
		}
		case 4:
		{
			Type = MGL_RGBA;
			break;
		}
	}

	Data = new unsigned char[Width * Height * Channels];

	BYTE *p = Data;
	size_t numLines = 0;

	while (info.output_scanline < info.output_height)
	{
		numLines = jpeg_read_scanlines(&info, &p, 1);
		p += numLines * Channels * Width;
	}

	jpeg_finish_decompress(&info);
	jpeg_destroy_decompress(&info);
	fclose(fin);
}

// ------------------------------------------------------------------------------------------------
void Texture::Image::Unload()
{
	if (Data)
	{
		delete[] Data;
		Data = NULL;
	}

	Width = 0;
	Height = 0;
	Type = MGL_RGBA;
}

// ------------------------------------------------------------------------------------------------
void Texture::Image::Upload(MGLenum target, size_t level)
{
	mglTexImage2D(target, level, MGL_RGBA, Width, Height, 0, Type, MGL_UNSIGNED_BYTE, Data);
}

// ------------------------------------------------------------------------------------------------
Texture::Texture(ResourceManager* rsmngr, const std::string& id, TextureType type)
	: Resource(rsmngr, TEXTURE, id),
	  mType(type),
	  mImages(NULL),
	  mTexture(NULL),
	  mMipmaps(false)
{
	mFilter = (Filter)2;
	mAnisotropy = 16.0f;
}

// ------------------------------------------------------------------------------------------------
Texture::~Texture()
{
	if (mTexture)
		mglDeleteTextures(1, &mTexture);

	for (size_t i = 0; i < mImages.size(); ++i)
		mImages[i].Unload();
}

// ------------------------------------------------------------------------------------------------
void Texture::AddImage(const std::string& name)
{
	mImages.resize(mImages.size() + 1);
	mImages.rbegin()->FileName = name;
}

// ------------------------------------------------------------------------------------------------
void Texture::Load()
{
	MGLuint texture;

	for (size_t i = 0; i < mImages.size(); ++i)
	{
		mImages[i].Load();
	}
	
	mglGenTextures(1, &texture);
	
	if (mType == TEX_CUBEMAP)
	{		
		mglBindTexture(MGL_TEXTURE_CUBE_MAP, texture);
		mglTexParameteri(MGL_TEXTURE_CUBE_MAP, MGL_TEXTURE_MAG_FILTER, MGL_LINEAR);
		mglTexParameteri(MGL_TEXTURE_CUBE_MAP, MGL_TEXTURE_MIN_FILTER, MGL_LINEAR);

		if (mImages.size() != 6)
			throw Exception("Cube map faces missing");

		for (size_t i = 0; i < 6; ++i)
		{
			mImages[i].Upload(CUBE_FACES[i]);
			mImages[i].Unload();
		}
			
		mglBindTexture(MGL_TEXTURE_CUBE_MAP, 0);
		mTexture = texture;
		return;
	}
	
	mglBindTexture(mType, texture);
	mglTexParameterf(mType, MGL_TEXTURE_MAX_ANISOTROPY, mAnisotropy);

	switch (mFilter)
	{
		case DEFAULT:
		{
			mglTexParameteri(mType, MGL_TEXTURE_MAG_FILTER, MGL_NEAREST);
			mglTexParameteri(mType, MGL_TEXTURE_MIN_FILTER, MGL_NEAREST); 
			break;
		}
		case BILINEAR:
		{
			mglTexParameteri(mType, MGL_TEXTURE_MAG_FILTER, MGL_LINEAR);
			mglTexParameteri(mType, MGL_TEXTURE_MIN_FILTER, MGL_LINEAR); 
			break;
		}
		case TRILINEAR:
		{
			mglTexParameteri(mType, MGL_TEXTURE_MAG_FILTER, MGL_LINEAR);
			mglTexParameteri(mType, MGL_TEXTURE_MIN_FILTER, MGL_LINEAR_MIPMAP_LINEAR); 
			mglTexParameteri(mType, MGL_GENERATE_MIPMAP, MGL_TRUE); 
			break;
		}
	}

	if (mImages.size() > 0)
	{
		mImages[0].Upload(mType, 0);
		mImages[0].Unload();
	}

	mglBindTexture(mType, 0);

	mglFlush();
	mglFinish();

	mTexture = texture;
}


// ------------------------------------------------------------------------------------------------
void Texture::Unload()
{
	for (size_t i = 0; i < mImages.size(); ++i)
	{
		mImages[i].Unload();
	}
	
	if (mTexture)
	{
		mglDeleteTextures(1, &mTexture);
		mTexture = 0;
	}
}

// ------------------------------------------------------------------------------------------------
void Texture::Bind()
{
	if (mTexture && GetState() == Resource::LOADED)
	{
		mglBindTexture(mType, mTexture);
	}
	else
	{
		mglBindTexture(mType, 0);
	}
}