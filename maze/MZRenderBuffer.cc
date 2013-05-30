// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZRenderBuffer.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
RenderBuffer::RenderBuffer()
	: SkyTexture(NULL)
{
}

// ------------------------------------------------------------------------------------------------
RenderBuffer::~RenderBuffer()
{
}

// ------------------------------------------------------------------------------------------------
LightRenderData* RenderBuffer::AddLight()
{
	Lights.resize(Lights.size() + 1);
	return &(*Lights.rbegin());
}

// ------------------------------------------------------------------------------------------------
WidgetRenderData* RenderBuffer::AddWidget()
{
	Widgets.resize(Widgets.size() + 1);
	return &(*Widgets.rbegin());
}

// ------------------------------------------------------------------------------------------------
ObjectRenderData* RenderBuffer::AddObject()
{
	Objects.resize(Objects.size() + 1);
	return &(*Objects.rbegin());
}

// ------------------------------------------------------------------------------------------------
TextRenderData* RenderBuffer::AddText()
{
	Text.resize(Text.size() + 1);
	return &(*Text.rbegin());
}

// ------------------------------------------------------------------------------------------------
void RenderBuffer::Clear()
{
	Text.resize(0);
	Lights.resize(0);
	Widgets.resize(0);
	Objects.resize(0);
	ShadowCasters.resize(0);
	
	Text.reserve(1000);
	Lights.reserve(1000);
	Widgets.reserve(1000);
	Objects.reserve(1000);
	ShadowCasters.reserve(1000);
}
// ------------------------------------------------------------------------------------------------
bool SortDepth(const ObjectRenderData& a, const ObjectRenderData& b)
{
	return a.Z < b.Z;
}

// ------------------------------------------------------------------------------------------------
void RenderBuffer::Sort(const glm::mat4 view)
{
	for (size_t i = 0; i < Objects.size(); ++i)
	{
		Objects[i].Z = -(view * Objects[i].ModelMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)).z;
	}

	std::sort(Objects.begin(), Objects.end(), SortDepth);
}