// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MZFONT_H
#define MZFONT_H
#pragma once

#include "MZResource.h"

namespace MAZE
{
	class Font : public Resource
	{
	public:

		Font(const std::string& id, const std::string& fn, size_t h);
		virtual ~Font();

	private:

	};
};

#endif
