// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2013 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
SoundSource::SoundSource()
	: mPosition(0.0f),
	  mPitch(1.0f),
	  mGain(0.0f),
	  mPlaying(false),
	  mFinished(false),
	  mStarted(false),
	  mSource(0)
{
}

// ------------------------------------------------------------------------------------------------
SoundSource::~SoundSource()
{
	if (mSource) 
	{ 
		alDeleteSources(1, &mSource); 
		mSource = 0; 
	}
}

// ------------------------------------------------------------------------------------------------
void SoundSource::Play()
{
	ALint state;
	
	alGetSourcei(mSource, AL_SOURCE_STATE, &state);

	mPlaying = true;
	mStarted = false;

	if (state != AL_PLAYING)
	{
		if (mBuffer != NULL && mBuffer->IsReady())
		{
			if (!alIsSource(mSource))
			{
				alGenSources(1, &mSource);
			}

			alSourcei(mSource, AL_BUFFER, mBuffer->GetBuffer());
			alSourcePlay(mSource);
			mStarted = true;
		}
	}
}

// ------------------------------------------------------------------------------------------------
void SoundSource::Stop()
{
	alSourceStop(mSource);
}

// ------------------------------------------------------------------------------------------------
void SoundSource::Update()
{
	ALint state;	
	alGetSourcei(mSource, AL_SOURCE_STATE, &state);

	if (state == AL_STOPPED)
	{
		mFinished = true;
	}

	if (mPlaying && !mStarted)
	{
		if (mBuffer != NULL && mBuffer->IsReady())
		{
			if (!alIsSource(mSource))
			{
				alGenSources(1, &mSource);
			}

			alSourcei(mSource, AL_BUFFER, mBuffer->GetBuffer());
			alSourcePlay(mSource);
			mStarted = true;
		}
	}
}
