// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
ResourceManager::ResourceManager(Engine* engine)
	: mEngine(engine),
	  mGLContext(NULL)
{
}

// ------------------------------------------------------------------------------------------------
ResourceManager::~ResourceManager()
{
	mGLContext ? wglMakeCurrent(mEngine->GetDC(), mGLContext) : 0;

	// Unload all resources
	for (ResourceHandleIter it = mHandleMap.begin(); it != mHandleMap.end(); ++it)
	{
		it->second->Unload();
	}

	for (ResourceHandleIter it = mHandleMap.begin(); it != mHandleMap.end(); ++it)
	{
		delete it->second;
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(mGLContext);
}

// ------------------------------------------------------------------------------------------------
void ResourceManager::Init()
{
	mGLContext = wglCreateContext(mEngine->GetDC());
	wglShareLists(mEngine->GetRenderer()->GetContext(), mGLContext);

	Log::Inst() << "[Rsmngr] Loading '" << mEngine->GetSetup().ResourceDir << "'";
	DiscoverFolder(mEngine->GetSetup().ResourceDir);
}

// ------------------------------------------------------------------------------------------------
void ResourceManager::DiscoverFolder(const std::string& dir)
{
	WIN32_FIND_DATA ffd;
	HANDLE handle;
	std::string folder = dir + "\\*";

	if ((handle = ::FindFirstFileA(folder.c_str(), &ffd)) == INVALID_HANDLE_VALUE)
	{
		return;
	}

	do
	{
		std::string fileName(ffd.cFileName);

		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (fileName != "." && fileName != "..")
			{
				DiscoverFolder(dir + "\\" + fileName);
			}
		}
		else
		{
			DiscoverResource(dir + "\\" + fileName);	
		}
	} while (::FindNextFile(handle, &ffd));
}

// ------------------------------------------------------------------------------------------------
void ResourceManager::DiscoverResource(const std::string& fn)
{
	size_t slashPos = fn.find_last_of('\\'), dotPos = fn.find_last_of('.');
	if (slashPos == std::string::npos || dotPos == std::string::npos)
	{
		return;
	}

	std::string path = fn.substr(0, slashPos);
	std::string id = fn.substr(slashPos + 1, dotPos - slashPos - 1);
	std::string ext = fn.substr(dotPos + 1);

	if (ext == "png" || ext == "jpg")
	{
		std::vector<std::string> tokens;
		Texture* tex = NULL;
		ResourceKeyIter it;

		Split(id, '.', tokens);
		id = tokens[0];
		
		if ((it = mKeyMap.find(std::make_pair(id, Texture::TYPE))) != mKeyMap.end())
		{
			tex = (Texture*)it->second;
		}

		if (tokens.size() == 2)
		{
			if (tokens[1] == "2d")
			{
				if (!tex)
				{
					tex = new Texture(this, id, Texture::TEX_2D);
					Add(tex);
				}
				
				Log::Inst() << "[Rsmngr] Discovered texture: '" << id << "'";
				tex->AddImage(fn);
				return;
			}
		}
		else if (tokens.size() == 3)
		{
			if (tokens[1] == "cube")
			{
				if (!tex)
				{
					Log::Inst() << "[Rsmngr] Discovered cubemap: '" << id << "'";
					tex = new Texture(this, id, Texture::TEX_CUBEMAP);
					Add(tex);
				}
				
				tex->AddImage(fn);
				return;
			}
		}
	}
	else if (ext == "ttf")
	{
		std::vector<std::string> tokens;

		Split(id, '.', tokens);
		
		Log::Inst() << "[Rsmngr] Discovered font: '" << id << "'";
		Add(new Font(this, tokens[0], atoi(tokens[1].c_str()), fn));
	}
	else if (ext == "mzo")
	{
		Log::Inst() << "[Rsmngr] Discovered model: '" << id << "'";
		Add(new Model(this, id, fn));
	}
	else if (ext == "ogg")
	{
		Log::Inst() << "[Rsmngr] Discovered sound: '" << id << "'";
		Add(new Sound(this, id, fn));
	}
}

// ------------------------------------------------------------------------------------------------
void ResourceManager::Add(Resource* res)
{
	Resource::Handle handle = ++mResourceCount;
	
	if (mKeyMap.find(std::make_pair(res->GetID(), res->GetType())) != mKeyMap.end())
	{
		throw Exception("Duplicate resource: '" + res->GetID() + "'");
	}

	res->SetHandle(handle);
	mHandleMap.insert(std::make_pair(handle, res));
	mKeyMap.insert(std::make_pair(std::make_pair(res->GetID(), res->GetType()), res));
}

// ------------------------------------------------------------------------------------------------
void ResourceManager::Post(Resource* res)
{
	mJobMutex.Lock();
	mJobQueue.push(res);

	if (IsSuspended())
	{
		Resume();
	}
	
	mJobMutex.Unlock();
}

// ------------------------------------------------------------------------------------------------
int ResourceManager::Worker()
{
	try
	{
		Log::Inst() << "[Rsmngr] Thread started";
		wglMakeCurrent(mEngine->GetDC(), mGLContext);

		while (IsRunning())
		{
			Resource *job;

			mJobMutex.Lock();
			if (mJobQueue.empty())
			{
				mJobMutex.Unlock();
				Suspend();
				continue;
			}

			job = mJobQueue.front();
			mJobQueue.pop();	
			mJobMutex.Unlock();

			try
			{
				Mutex::ScopedLock lock(job->mMutex);
				if (job->mState == Resource::LOADING_QUEUED)
				{
					job->mState = Resource::LOADING;
					job->Load();
					job->mState = Resource::LOADED;
					continue;
				}

				if (job->mState == Resource::UNLOADING_QUEUED)
				{
					job->mState = Resource::UNLOADING;
					job->Unload();
					job->mState = Resource::PREPARED;
					continue;
				}
			}
			catch (std::exception& e)
			{
				Log::Inst() << "[Rsmngr] Error: " << e.what();
			}
		}
		
		alcMakeContextCurrent(NULL);
		wglMakeCurrent(NULL, NULL);
		Log::Inst() << "[Rsmngr] Thread stopped";
		return 0;
	}
	catch (std::exception& e)
	{
		alcMakeContextCurrent(NULL);
		wglMakeCurrent(NULL, NULL);

		Log::Inst() << "[Rsmngr] Unhandled exception: " << e.what();
		mEngine->Quit();
		return -1;
	}
}