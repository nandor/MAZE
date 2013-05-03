// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef RESOURCE_H
#define RESOURCE_H
#pragma once

#include "MZPlatform.h"
#include "MZException.h"
#include "MZThread.h"

namespace MAZE
{
	class Engine;
	class ResourceManager;

	/**
		All resources managed by the ResourceManager
		must be derived from this type
	*/
	class Resource
	{
	public:

		/**
			Describes the state of the object
		*/
		enum State
		{
			/// Discovered by the manager - unusable, loadable
			PREPARED,

			/// Queued for loading
			LOADING_QUEUED,

			/// Loading - unusable
			LOADING,

			/// Loaded - compilable
			LOADED,

			/// Queued for unloading
			UNLOADING_QUEUED,

			/// Unloading - unusable
			UNLOADING
		};

		/**
			List of resource types
		*/
		enum Type
		{
			/// 3D mesh
			MODEL,
			
			/// ogg sound
			SOUND,
			
			/// png texture
			TEXTURE,

			/// glsl program
			PROGRAM,
			
			/// Custom resource
			CUSTOM
		};

		/// Resource handle
		typedef unsigned int Handle;

		/// Resource key
		typedef std::pair<std::string, Type> Key;
		
		/**
			Reference to a resource

			Increases the reference count when it acquires
			a pointer to a resource

			Decreseas the reference count when it's deleted
			or gets out of scope
		*/
		template <typename T>
		class Ptr
		{
		public:
			
			/**
				@brief Constructs a reference from a raw pointer
				@param res Raw pointer to the resouse

				Increments the reference count of the pointed object
			*/
			explicit Ptr(T *res = NULL) : mResource(res)
			{
				if (mResource)
					mResource->IncRefCount();
			}

			/**
				@brief Constructs a reference from another pointer
				@param ptr Another pointer of the same type

				Increments the reference count of the pointed object
			*/
			Ptr(const Ptr& ptr) : mResource(ptr.mResource)
			{
				if (mResource)
					mResource->IncRefCount();
			}

			/**
				Releases the reference to the object when
				the Ptr goes out of scope
			*/
			~Ptr()
			{
				if (mResource)
					mResource->DecRefCount();
			}

			/**
				Copies another pointer. Frees the current
				object and increases the reference count
				of the new object.
			*/
			Ptr& operator = (const Ptr& ptr)
			{
				if (mResource)
					mResource->DecRefCount();

				mResource = ptr.mResource;
				if (mResource != NULL)
					mResource->IncRefCount();

				return *this;
			}

			/**
				@brief Dereferences the pointed object
			*/
			T& operator * ()
			{
				return *mResource;
			}

			/**
				Returns the pointed object
			*/
			T* operator -> ()
			{
				return mResource;
			}

			/**
				Returns the pointed object. 
				If you delete it, you are evil.
			*/
			operator T* ()
			{
				return (T*)mResource;
			}

			/**	
				Releases the resource
			*/
			void Free()
			{
				if (mResource)
				{
					mResource->DecRefCount();
					mResource = NULL;
				}
			}

		private:

			/// Pointer to the referenced resource
			T *mResource;
		};

	public:

		/**
			Creates a new resource
		*/
		Resource(ResourceManager* rsmngr, Type type, const std::string& id);
		
		/**
			Destroys the object
		*/
		virtual ~Resource();

		/**
			Returns the state of the object
			@return State descriptor
		*/
		State GetState()
		{
			return mState;
		}

		/**
			Returns the type of the resource
		*/
		Type GetType()
		{
			return mType;
		}

		/**
			Returns the handle of the resource
		*/
		Handle GetHandle()
		{
			return mHandle;
		}

		/**
			Returns the id of the resource
		*/
		std::string GetID()
		{
			return mID;
		}

		/**
			Returns the ammount of memory used by the object
			@return	sizeof(Resource)
		*/
		virtual size_t GetSizeInBytes()
		{
			return sizeof(*this);
		}

		/**
			Checks if the resource is ready
		*/
		virtual bool IsReady()
		{
			return mState == Resource::LOADED;
		}

		/**
			Loads resource data from the disk
		*/
		virtual void Load()
		{
		}
		
		/**
			Unloads data, frees memory
		*/
		virtual void Unload()
		{
		}
		
	protected:

		/**
			Increment the reference count
			Load the resource if it's 1
		*/
		virtual void IncRefCount();

		/**
			Decrement reference count
			Unload the resource if it gets to 0
		*/
		virtual void DecRefCount();

		/**
			Assign a handle to the resouce
		*/
		void SetHandle(Handle handle)
		{
			mHandle = handle;
		}

	protected:

		/// State of the object
		State mState;
		
		/// Number of objects using this resource
		unsigned mRefCount;

		/// Resource lock
		Mutex mMutex;

		/// ID of the resource
		std::string mID;

		/// Numeric handle of the resource
		Handle mHandle;

		/// Type of the resource
		Type mType;

		/// The resource manager which owns this
		ResourceManager *mRsmngr;
				
		/// Resource manager is friend
		friend ResourceManager;

	};
};

#endif
