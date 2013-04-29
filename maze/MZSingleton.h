// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef SINGLETON_H
#define SINGLETON_H
#pragma once

#include <cassert>
#include "MZPlatform.h"

namespace MAZE
{
	template <class T>
	class Singleton
	{		
	public:

		class Holder
		{
		public:

			Holder()
				: mInstance(NULL)
			{
			}

			~Holder()
			{
				if (mInstance != NULL)
					delete mInstance;
			}

			void SetInstance(T* inst)
			{
				mInstance = inst;
			}

		private:

			T* mInstance;
		};

		/**
			Singletons are initialized by calling the constructor
			of the class at startup, so no thread safety is needed
		*/
		Singleton()
		{
			assert(!mInstance);
			mInstance = (T*)this;
			mHolder.SetInstance(mInstance);
		}

		/**
			Returns a reference to the singleton instance
		*/
		static T& Inst()
		{
			return *mInstance;
		}

		/**
			Returns a poitner to the singleton instance
		*/
		static T* Ptr()
		{
			return mInstance;
		}

	private:

		// Do not allow copying
		Singleton(const Singleton<T>& );
		Singleton& operator = (const Singleton<T>& );

	private:
		
		static Holder mHolder;
		static T *mInstance;

	};
	
	template<class T> typename Singleton<T>::Holder Singleton<T>::mHolder;
	template<class T> T* Singleton<T>::mInstance = NULL;
};

#endif