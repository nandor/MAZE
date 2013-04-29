// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef THREAD_H
#define THREAD_H
#pragma once

#include <cassert>
#include <vector>
#include <queue>
#include "MZPlatform.h"
#include "MZException.h"

namespace MAZE
{
	/**
		Wrapper around a Win32 thread
	*/
	class Thread
	{
	public:

		/**
			Initialises the thread, does not create is
		*/
		Thread();

		/**
			Stops the thread
		*/
		virtual ~Thread();

		/**
			Creates & starts the thread
		*/
		virtual void Start();

		/**
			Sends the stop signal & joins with the thread
		*/
		virtual void Stop();

		/**
			Suspends the thread
		*/
		virtual void Suspend();

		/**
			Resumes the thread
		*/
		virtual void Resume();

		/**
			Checks if the thread is running
		*/
		bool IsRunning()
		{
			return mRunning;
		}

		/**
			Checks if the thread is suspended
		*/
		bool IsSuspended()
		{
			return mSuspended;
		}

	protected:
		
		/**
			Worker function
		*/
		virtual int Worker() = 0;

	private:

		/// Starter function
		static DWORD WINAPI ThreadStarter(LPVOID);

	private:

		/// Flag used to stop the thread
		bool mRunning;

		/// Flag used to suspend the thread
		bool mSuspended;

		/// Handle of the thread
		HANDLE mHandle;
	};

	/**
		Wrapper around a Win32 critical section
	*/
	class Mutex
	{
	public:

		/**
			Scoped lock
		*/
		class ScopedLock
		{
		public:

			/**
				Locks the user when the object is initialized
			*/
			ScopedLock(Mutex& mutex);
			
			/**
				Unlocks the mutex when the object gets out of scope
			*/
			~ScopedLock();

		private:

			/// Referenced mutex
			Mutex& mMutex;
		};

	public:

		/// Disallow copying
		Mutex(const Mutex& m);
		void operator = (const Mutex& m);

		/**
			Creates a new mutex
		*/
		Mutex();
		
		/**
			Destroys an unlocked mutex
		*/
		~Mutex();

		/**
			Blocks and locks the mutex
		*/
		void Lock();
		
		/**
			Unlocks the mutex
		*/
		void Unlock();

	private:

		/// Win32 fast mutex
		::CRITICAL_SECTION mMutex;

	};
};

#endif
