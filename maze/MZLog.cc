// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include <ctime>
#include "MZLog.h"
#include "MZString.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
Log::Log(const std::string& fn)
	: mLogFile(fn)
{
	::InitializeCriticalSection(&mMutex);
}

// ------------------------------------------------------------------------------------------------
Log::~Log()
{
	::DeleteCriticalSection(&mMutex);
	mLogFile.close();
}

// ------------------------------------------------------------------------------------------------
void Log::Write(const std::string& message)
{
	time_t rawtime;
	struct tm * t;
	std::string header;
	std::vector<std::string> lines;

	time(&rawtime);
	t = gmtime(&rawtime);
	
	std::stringstream ss;
	ss << "[" << t->tm_hour << ":" << t->tm_min << ":" << t->tm_mday
	   << " #" << ::GetCurrentThreadId() << "]";
	header = ss.str();

	Split(message, '\n', lines);

	::EnterCriticalSection(&mMutex);
	
	for (size_t i = 0; i < lines.size(); ++i)
	{
		mLogFile << header << ": " << Trim(lines[i]) << std::endl;
	}

	::LeaveCriticalSection(&mMutex);
}
