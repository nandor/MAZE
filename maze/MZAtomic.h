// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2013 The MAZE project. All rights reserved.

#ifndef MZATOMIC_H
#define MZATOMIC_H
#pragma once

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
		
#if defined(_MSC_VER)
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

#else

		unsigned operator ++ ()
		{
			return __sync_fetch_and_add(&mVal, static_cast<T> (1));
		}

		unsigned operator ++ (int)
		{
			return __sync_fetch_and_add(&mVal, static_cast<T> (1)) - 1;
		}
		
		unsigned operator -- ()
		{
			return __sync_fetch_and_sub(&mVal, static_cast<T> (1));
		}
		
		unsigned operator -- (int)
		{
			return __sync_fetch_and_sub(&mVal, static_cast<T> (1)) + 1;
		}

#endif

	private:

		/// Atomic value, must be 4 or 8 bytes wide
		volatile T mVal;

	};
}

#endif
