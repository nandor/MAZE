// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
Resource::Resource(ResourceManager* rsmngr, Type type, const std::string& id)
	: mState(PREPARED),
	  mRefCount(0),
	  mID(id),
	  mHandle(0),
	  mType(type),
	  mRsmngr(rsmngr)
{
}

// ------------------------------------------------------------------------------------------------
Resource::~Resource()
{
}

// ------------------------------------------------------------------------------------------------
void Resource::IncRefCount()
{
	Mutex::ScopedLock lock(mMutex);

	if (++mRefCount == 1)
	{
		mState = LOADING_QUEUED;
		mRsmngr->Post(this);
	}
}

// ------------------------------------------------------------------------------------------------
void Resource::DecRefCount()
{
	Mutex::ScopedLock lock(mMutex);

	if (--mRefCount == 0)
	{
		mState = UNLOADING_QUEUED;
		mRsmngr->Post(this);
	}
}
