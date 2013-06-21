// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef LOG_H
#define LOG_H
#pragma once

namespace MAZE
{
	/**
		The Log class handles the log files. The only instance should
		be created when the application starts, before engine startup.

		Instances to this class should be retrieved using the singleton
		interface.
	*/
	class Log : public Singleton<Log>
	{
	private:
		
		class Stream
		{
		public:
			
			Stream(Log* log, const std::string& message)
				: mLogger(log),
				  mMessage(message)
			{
			}

			~Stream()
			{
				mLogger->Write(mMessage);
			}
			
			Stream& operator << (const std::string& val)
			{
				mMessage += val;
				return *this;
			}

			Stream& operator << (const char* val)
			{
				mMessage.append(val);
				return *this;
			}

			Stream& operator << (int val)
			{
				std::stringstream ss;
				ss << mMessage << val;
				mMessage = ss.str();
				return *this;
			}

			Stream& operator << (size_t val)
			{
				std::stringstream ss;
				ss << mMessage << val;
				mMessage = ss.str();
				return *this;
			}

			Stream& operator << (float val)
			{
				std::stringstream ss;
				ss << mMessage << val;
				mMessage = ss.str();
				return *this;
			}

		private:

			/// Message to be written
			std::string mMessage;

			/// Parent logger
			Log* mLogger;
		};

	public:

		/**
			Creates a new log file
		*/
		Log(const std::string& fn);

		/**
			Destroys the log, closing the output file
		*/
		~Log();

		/**
			Writes a new line to the output stream
			@param message Line
		*/
		void Write(const std::string& message);

		/**
			Returns a new output stream which writes
			a line when it gets out of scope
			@param T	Type of the item
			@parvam val Value to be written
		*/
		template <typename T>
		Stream operator << (const T& val)
		{
			std::stringstream ss;
			ss << val;
			return Stream(this, ss.str());
		}

	private:

		/// Log file
		std::ofstream mLogFile;

		/// Write lock
		::CRITICAL_SECTION mMutex;
	};
};

#endif
