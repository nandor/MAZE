// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef RSMNGR_H
#define RSMNGR_H
#pragma once

namespace MAZE
{
	class Engine;

	/**
		Resource handles
		Stores, loads and manages resources
	*/
	class ResourceManager : public Thread
	{
	public:

		/**
			Creates a new resource manager for the specified engine instance
			@param engine Game engine
		*/
		ResourceManager(Engine* engine);

		/**
			Destroys the manager and frees up resources
		*/
		~ResourceManager();

		/**
			Initialises the resource manager
			@param root Location of the data folder
		*/
		void Init();

		/**
			Retrieve a resource
		*/
		template <class T>
		Resource::Ptr<T> Get(const std::string& id)
		{
			ResourceKeyIter it;

			if ((it = mKeyMap.find(std::make_pair(id, T::TYPE))) == mKeyMap.end())
			{
				throw Exception("Resource not found: " + id);
			}
			
			if (it->second->GetType() != T::TYPE)
			{
				throw Exception("Resource is of invalid type: '" + id);
			}

			return Resource::Ptr<T>((T*)it->second);		
		}

		/**
			Retrieve a resource
		*/
		template <class T>
		Resource::Ptr<T> Get(const Resource::Handle& handle)
		{
			ResourceHandleIter it;

			if ((it = mHandleMap.find(handle)) == mHandleMap.end())
			{
				throw Exception("Resource not found: ") << handle;
			}
			
			if (it->second->GetType() != T::Type)
			{
				throw Exception("Invalid resource type: '" + it->second->GetID() + "'");
			}

			return Resource::Ptr<T>((T*)it->second);	
		}

		/**
			Adds a resource
		*/
		void Add(Resource* res);

		/**
			Removes a resource
		*/
		void Remove(Resource* res);
		
		/**
			Adds a job to the queue
		*/
		void Post(Resource* res);

	private:

		/**
			Searches for resources in a folder
		*/
		void DiscoverFolder(const std::string& dir);

		/**
			Checks if a file is a resource
		*/
		void DiscoverResource(const std::string& fn);

		/**
			Loads resources from storage, runs in the thread pool
		*/
		int Worker();

	private:
				
		/// Pointer to the engine
		Engine* mEngine;

		/// OpenGL context
		HGLRC mGLContext;
		
		/// Job queue
		std::queue<Resource*> mJobQueue;

		/// Job queue lock
		Mutex mJobMutex;
		
		/// Atomic resource counter for id generation
		Atomic<unsigned> mResourceCount;

		/// ID - handle conversion
		std::unordered_map<Resource::Handle, Resource*> mHandleMap;

		/// Hash map containing resources
		std::unordered_map<Resource::Key, Resource*, Resource::KeyHash> mKeyMap;

		/// Resource iterator
		typedef std::unordered_map<Resource::Key, Resource*, Resource::KeyHash>::iterator ResourceKeyIter;
		typedef std::unordered_map<Resource::Handle, Resource*>::iterator ResourceHandleIter;
	};
};

#endif
