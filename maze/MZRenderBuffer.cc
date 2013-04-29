// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZRenderBuffer.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
RenderBuffer::RenderBuffer()
{
}

// ------------------------------------------------------------------------------------------------
RenderBuffer::~RenderBuffer()
{
}

// ------------------------------------------------------------------------------------------------
void RenderBuffer::Clear()
{
	Objects.resize(0);
	Objects.reserve(1000);
	Lights.resize(0);
	Lights.reserve(1000);
	ShadowCasters.resize(0);
	ShadowCasters.reserve(1000);
}
