// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZEXCEPTION_H
#define MZEXCEPTION_H
#pragma once

#define NOT_IMPLEMENTED { throw Exception("Not implemented: ") << __FUNCTION__; }

namespace MAZE 
{
	class Exception : public std::exception
	{
	public:

		Exception() throw()
			: mMessage("")
		{
		}

		Exception(const std::string& str) throw()
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

		WindowsException(const std::string& str) throw();
		
		virtual ~WindowsException() throw()
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

		MGLException(const std::string& str) throw();

		virtual ~MGLException() throw()
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

#endif
