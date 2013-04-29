// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef ATOMIC_H
#define ATOMIC_H
#pragma once

#include "MZPlatform.h"
#include "MZException.h"

namespace MAZE
{
	/**
		Provides a wrapper for atomic operations on integers
	*/
	template <typename T>
	class Atomic
	{
	public:

		Atomic() 
			: mVal(0)
		{
		}

		Atomic(const T& rhs)
			: mVal(rhs)
		{
		}

		Atomic(const Atomic& rhs) 
			: mVal(rhs.mVal)
		{
		}

		Atomic(unsigned val) 
			: mVal(val)
		{
		}
		
		operator unsigned()
		{
			return mVal;
		}

		unsigned operator ++ ()
		{
			switch (sizeof(T))
			{
				case 4: 
				{
					return static_cast<T>(::InterlockedIncrement((LONG*)&mVal));
				}
				case 8:
				{
					return static_cast<T>(::InterlockedIncrement64((LONGLONG*)&mVal));
				}
				default:
				{
					throw Exception("Not implemented: ") << __FUNCTION__;
				}
			}
		}

		unsigned operator ++ (int)
		{
			switch (sizeof(T))
			{
				case 4:
				{
					return static_cast<T>(::InterlockedIncrement((LONG*)&mVal) - 1);
				}
				case 8:
				{
					return static_cast<T>(::InterlockedIncrement64((LONGLONG*)&mVal) - 1);
				}
				default: 
				{
					throw Exception("Not implemented: ") << __FUNCTION__;
				}
			}
		}
		
		/**
			Decrements the value of the counter
		*/
		unsigned operator -- ()
		{
			switch (sizeof(T))
			{
				case 4:
				{
					return ::InterlockedDecrement((LONG*)&mVal);
				}
				case 8:
				{
					return ::InterlockedDecrement64((LONGLONG*)&mVal);
				}
				default:
				{
					throw Exception("Not implemented: ") << __FUNCTION__;
				}
			}
		}

		/**
			Decrements the value of the counter
		*/
		unsigned operator -- (int)
		{
			switch (sizeof(T))
			{
				case 4:
				{
					return ::InterlockedDecrement((LONG*)&mVal) + 1;
				}
				case 8: 
				{
					return ::InterlockedDecrement64((LONGLONG*)&mVal) + 1;
				}
				default: 
				{
					throw Exception("Not implemented: ") << __FUNCTION__;
				}
			}
		}

	private:

		/// Atomic value, must be 4 or 8 bytes wide
		volatile T mVal;

	};
}

#endif
