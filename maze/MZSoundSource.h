// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2013 The MAZE project. All rights reserved.

#ifndef MZSOUNDSOURCE_H
#define MZSOUNDSOURCE_H
#pragma once

#include <al/al.h>
#include <glm/glm.hpp>
#include "MZSound.h"

namespace MAZE
{
    class SoundSource
    {
    public:
        
        SoundSource();
		~SoundSource();

		void Play();
		void Stop();

		void Update();

	public:

		void SetRepeat(bool repeat)
		{
			mRepeat = repeat;
			alSourcei(mSource, AL_LOOPING, mRepeat);
		}

		void SetSource(Resource::Ptr<Sound> src)
		{
			mBuffer = src;
		}
        
		void SetPosition(const glm::vec3& p)
		{
			mPosition = p;
			alSourcefv(mSource, AL_POSITION, &p[0]);
		}

		void SetPitch(float pitch)
		{
			mPitch = pitch;
			alSourcef(mSource, AL_PITCH, pitch);
		}

		void SetGain(float gain)
		{
			mGain = gain;
			alSourcef(mSource, AL_GAIN, gain);
		}
		
		bool Finished()
		{
			return mFinished;
		}

    private:
    
		/// Position of the sound source
		glm::vec3 mPosition;

		/// Pitch of the sound
		float mPitch;

		/// Gain of the sound
		float mGain;

		/// True if the sound is repeated
		bool mRepeat;

		/// True if the sound was finished playing
		bool mPlaying;

		/// True if the sound was finished
		bool mFinished;

		/// True if we've actually started playing the sound
		bool mStarted;

		/// OpenAL source id
		ALuint mSource;

		/// Source buffer
		Resource::Ptr<Sound> mBuffer;
    };
};

#endif
