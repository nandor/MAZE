// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include <sstream>
#include "MZPlatform.h"
#include "MZException.h"
#include "MZGL.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
Exception& Exception::operator << (int iVal)
{
	std::stringstream ss;
	ss << mMessage << iVal;
	mMessage = ss.str();
	return *this;
}

// ------------------------------------------------------------------------------------------------
Exception& Exception::operator << (size_t uVal)
{
	std::stringstream ss;
	ss << mMessage << uVal;
	mMessage = ss.str();
	return *this;
}

// ------------------------------------------------------------------------------------------------
Exception& Exception::operator << (float fVal)
{
	std::stringstream ss;
	ss << mMessage << fVal;
	mMessage = ss.str();
	return *this;
}

// ------------------------------------------------------------------------------------------------
Exception& Exception::operator << (const std::string& str)
{
	mMessage.append(str);
	return *this;
}

// ------------------------------------------------------------------------------------------------
WindowsException::WindowsException(const std::string& str)
{
	LPVOID message;
	DWORD length;
	
	mErrorCode = ::GetLastError();

	length = ::FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        mErrorCode,
		0,
        (LPTSTR) &message,
        0, 
		NULL 
	);

	std::stringstream ss;
	ss << str << ": " << std::string((const char*)message, length);
	mMessage = ss.str();

    LocalFree(message);
}

// ------------------------------------------------------------------------------------------------
MGLException::MGLException(const std::string& str)
{
	mErrorCode = mglGetError();
	
	std::stringstream ss;
	ss << str << ": ";
	
	switch (mErrorCode)
	{
		case MGL_NO_ERROR: 
		{
			ss << "No error"; 
			break;
		}
		case MGL_INVALID_ENUM:
		{
			ss << "No error"; 
			break;
		}
		case MGL_INVALID_OPERATION: 
		{
			ss << "No error"; 
			break;
		}
		case MGL_INVALID_FRAMEBUFFER_OPERATION: 
		{
			ss << "Invalid framebuffer operation"; 
			break;
		}
		case MGL_OUT_OF_MEMORY: 
		{
			ss << "Out of memory"; 
			break;
		}
		case MGL_STACK_OVERFLOW: 
		{
			ss << "Stack overflow"; 
			break;
		}
		case MGL_STACK_UNDERFLOW: 
		{
			ss << "Stack underflow"; 
			break;
		}
	}

	mMessage = ss.str();
}
