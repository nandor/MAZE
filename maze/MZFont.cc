// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include <ft2build.h>
#include <freetype/ftglyph.h>
#include <freetype/freetype.h>
#include "MZFont.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
Font::Font(ResourceManager *rsmngr, const std::string& id, size_t height, const std::string& fn)
	: Resource(rsmngr, Resource::FONT, id),
	  mTexture(NULL),
	  mHeight(height),
	  mFileName(fn)
{
}

// ------------------------------------------------------------------------------------------------
Font::~Font()
{
}

// ------------------------------------------------------------------------------------------------
void Font::Load()
{
	FT_Library library;
	FT_Face face;
	FT_Glyph glyph[SIGMA];

	FT_Init_FreeType(&library);
	FT_New_Face(library, &mFileName[0], 0, &face);
	FT_Set_Char_Size(face, mHeight << 6, mHeight << 6, 96, 96);

	size_t texWidth = 0, texHeight = 0;

	for (char i = 0; i < SIGMA; ++i)
	{
		FT_BitmapGlyph bitmapGlyph;
		FT_Bitmap bitmap;

		FT_Load_Glyph(face, FT_Get_Char_Index (face, i + 32), FT_LOAD_DEFAULT);
		FT_Get_Glyph(face->glyph, &glyph[i]);
		FT_Glyph_To_Bitmap (&glyph[i], ft_render_mode_normal, 0, 1);

		bitmapGlyph = (FT_BitmapGlyph)glyph[i];
		bitmap = bitmapGlyph->bitmap;

		texWidth += bitmap.width;
		texHeight = std::max(texHeight, (size_t)bitmap.rows);

		mGlyphs[i].Advance = face->glyph->advance.x >> 6;
	}

	unsigned char* data;
	size_t rowBytes;
	float width, height;

	mglGenTextures(1, &mTexture);
	mglBindTexture(MGL_TEXTURE_2D, mTexture);
	mglTexParameteri(MGL_TEXTURE_2D, MGL_TEXTURE_MIN_FILTER, MGL_NEAREST);
	mglTexParameteri(MGL_TEXTURE_2D, MGL_TEXTURE_MAG_FILTER, MGL_NEAREST);

	rowBytes = texWidth + 3 - ((texWidth - 1) % 4);
	data = (unsigned char*)malloc(rowBytes * texHeight);
	memset(data, 0, rowBytes * texHeight);

	width = (float)texWidth;
	height = (float)texHeight;
	texWidth = 0;

	for (size_t i = 0; i < SIGMA; ++i)
	{
		FT_Bitmap bitmap;
		FT_BitmapGlyph bglyph;

		bglyph = (FT_BitmapGlyph)glyph[i];
		bitmap = bglyph->bitmap;

		for (int j = 0; j < bitmap.rows; ++j)
		{
			for (int k = 0; k < bitmap.width; ++k)
			{
				data[j * rowBytes + texWidth + k] = bitmap.buffer[j * bitmap.width + k];
			}
		}
		
		mGlyphs[i].Offset	 = glm::vec2(bglyph->left, mHeight - bglyph->top);
		mGlyphs[i].Size      = glm::vec2(bitmap.width, bitmap.rows);
		mGlyphs[i].MapOffset = glm::vec2(texWidth / width, 0.0f);
		mGlyphs[i].MapSize   = glm::vec2(bitmap.width / width, bitmap.rows / height);
			
		texWidth += bitmap.width;
		FT_Done_Glyph(glyph[i]);
	}

	mglTexImage2D(MGL_TEXTURE_2D, 0, MGL_INTENSITY, texWidth, texHeight, 0, MGL_LUMINANCE, MGL_UNSIGNED_BYTE, data);
	
	free(data);
	FT_Done_FreeType(library);
}
		
// ------------------------------------------------------------------------------------------------
void Font::Unload()
{
}
