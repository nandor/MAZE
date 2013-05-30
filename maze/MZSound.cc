// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2013 The MAZE project. All rights reserved.

#include <al/al.h>
#include <vorbis/vorbisfile.h>
#include "MZLog.h"
#include "MZSound.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
Sound::Sound(ResourceManager* rsmngr, const std::string& id, const std::string& fn)
	: Resource(rsmngr, Resource::SOUND, id),
	  mFileName(fn),
	  mBuffer(0)
{
	alGenBuffers(1, &mBuffer);
}

// ------------------------------------------------------------------------------------------------
Sound::~Sound()
{
	if (mBuffer) { alDeleteBuffers(1, &mBuffer); }
}

// ------------------------------------------------------------------------------------------------
void Sound::Load()
{
	FILE *fin;
	vorbis_info *info;
	OggVorbis_File oggFile;
		
	if ((fin = fopen(mFileName.c_str(), "rb")) == NULL)
	{
		throw Exception("File not found: '") << mFileName << "'";
	}

	if (ov_open(fin, &oggFile, NULL, 0))
	{
		fclose(fin);
		throw Exception("Ogg error: '") << mFileName << "'";
	}

	if ((info = ov_info(&oggFile, -1)) == NULL)
	{
		fclose(fin);
		throw Exception("Cannot retrieve ogg info: '") << mFileName << "'";
	}

	mFormat = (info->channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;	
	mFrequency = info->rate;
	
	size_t bytes;
	std::vector<char> buffer;
	char segment[8192];
	int bitStream;

	do
	{
		bytes = ov_read(&oggFile, segment, sizeof(segment), 0, 2, 1, &bitStream);
		buffer.insert(buffer.end(), segment, segment + bytes);
	} while (bytes > 0);

	ov_clear(&oggFile);
	fclose(fin);

	// Upload samples to OpenAL
	alBufferData(mBuffer, mFormat, &buffer[0], buffer.size(), mFrequency);
}

// ------------------------------------------------------------------------------------------------
void Sound::Unload()
{
}
