// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
DWORD WINAPI Thread::ThreadStarter(LPVOID data)
{
	return ((Thread*)data)->Worker();
}

// ------------------------------------------------------------------------------------------------
Thread::Thread()
	: mRunning(false),
	  mSuspended(true),
	  mHandle(NULL)
{
}

// ------------------------------------------------------------------------------------------------
Thread::~Thread()
{
	if (mHandle)
	{
		if (mRunning)
		{
			mRunning = false;

			if (mSuspended)
			{
				mSuspended = false;
				::ResumeThread(mHandle);
			}

			::WaitForSingleObject(mHandle, 100);
		}

		::CloseHandle(mHandle);
	}
}

// ------------------------------------------------------------------------------------------------
void Thread::Start()
{	
	if (mRunning)
	{
		if (mSuspended)
		{
			Resume();
		}
		else
		{
			return;
		}
	}
	else
	{
		mRunning = true;
		mSuspended = false;
		if (!(mHandle = ::CreateThread(NULL, 0, ThreadStarter, this, 0, NULL)))
		{
			throw WindowsException("Failed to create thread");
		}
	}
}

// ------------------------------------------------------------------------------------------------
void Thread::Stop()
{
	if (mRunning && mHandle)
	{
		mRunning = false;	
		if (mSuspended)
		{
			::ResumeThread(mHandle);
		}
		
		::WaitForSingleObject(mHandle, 100);
		::CloseHandle(mHandle);
		mHandle = NULL;
	}
}

// ------------------------------------------------------------------------------------------------
void Thread::Suspend()
{
	if (mRunning && mHandle && !mSuspended)
	{
		mSuspended = true;
		::SuspendThread(mHandle);
	}
}

// ------------------------------------------------------------------------------------------------
void Thread::Resume()
{
	if (mRunning && mHandle && mSuspended)
	{
		::ResumeThread(mHandle);
		mSuspended = false;
	}
}

// ------------------------------------------------------------------------------------------------
Mutex::ScopedLock::ScopedLock(Mutex& m)
	: mMutex(m)
{
	mMutex.Lock();
}

// ------------------------------------------------------------------------------------------------
Mutex::ScopedLock::~ScopedLock()
{
	mMutex.Unlock();
}

// ------------------------------------------------------------------------------------------------
Mutex::Mutex()
{
	::InitializeCriticalSection(&mMutex);
}

// ------------------------------------------------------------------------------------------------
Mutex::~Mutex()
{
	::DeleteCriticalSection(&mMutex);
}

// ------------------------------------------------------------------------------------------------
void Mutex::Lock()
{
	::EnterCriticalSection(&mMutex);
}

// ------------------------------------------------------------------------------------------------
void Mutex::Unlock()
{
	::LeaveCriticalSection(&mMutex);
}
