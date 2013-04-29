// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include <string>
#include <exception>
#include "MZGL.h"

#ifndef EXCEPTION_H
#define EXCEPTION_H
#pragma once

namespace MAZE 
{
	class Exception : public std::exception
	{
	public:

		Exception() 
			: mMessage("")
		{
		}

		Exception(const std::string& str)
			: mMessage(str)
		{
		}

		virtual ~Exception() throw() 
		{ 
		}

		Exception& operator << (int);
		Exception& operator << (size_t);
		Exception& operator << (float);
		Exception& operator << (const std::string& str);

		const char* what() const throw()
		{
			return mMessage.c_str();
		}

	protected:

		std::string mMessage;

	};

	class WindowsException : public Exception
	{
	public:

		WindowsException(const std::string& str);
		
		virtual ~WindowsException()
		{
		}

		DWORD GetErrorCode()
		{
			return mErrorCode;
		}

	private:

		DWORD mErrorCode;
	};

	class MGLException : public Exception
	{
	public:

		MGLException(const std::string& str);

		virtual ~MGLException()
		{
		}

		MGLenum GetErrorCode()
		{
			return mErrorCode;
		}

	private:

		MGLenum mErrorCode;

	};
};

#endif EXCEPTION_H
