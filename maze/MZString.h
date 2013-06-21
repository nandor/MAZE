// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef STRING_H
#define STRING_H
#pragma once

namespace MAZE
{
	// ------------------------------------------------------------------------------------------------
	inline void Split(const std::string& str, char delim, 
					  std::vector<std::string>& vec)
	{
		size_t current = 0, next;

		do
		{
			next = str.find_first_of(delim, current);
			vec.push_back(str.substr(current, next - current));
			current = next + 1;
		} while (next != std::string::npos);
	}
	
	// ------------------------------------------------------------------------------------------------
	inline std::string &LeftTrim(std::string &s) 
	{
		std::string::iterator it = s.begin();

		while (it < s.end() && isspace(*it))
		{
			++it;
		}

		s.erase(s.begin(), it);
		return s;
	}
	
	// ------------------------------------------------------------------------------------------------
	inline std::string &RightTrim(std::string &s) 
	{
		std::string::reverse_iterator it = s.rbegin();

		while (it < s.rend() && isspace(*it))
		{
			++it;
		}

		s.erase(it.base(), s.end());
		return s;
	}
	
	// ------------------------------------------------------------------------------------------------
	inline std::string &Trim(std::string &s) 
	{
		return LeftTrim(RightTrim(s));
	}

	// ------------------------------------------------------------------------------------------------
	inline bool StartsWith(const std::string& s, const std::string& what)
	{
		if (s.length() < what.length())
			return false;

		return s.substr(0, what.length()) == what;
	}
};

#endif
