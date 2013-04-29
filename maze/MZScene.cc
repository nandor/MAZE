// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include <new.h>
#include <stack>
#include "MZLight.h"
#include "MZObject.h"
#include "MZPlatform.h"
#include "MZLog.h"
#include "MZEntity.h"
#include "MZScene.h"
#include "MZCamera.h"
#include "MZRenderBuffer.h"
using namespace MAZE;

const float Scene::NODE_SCALE = 0.2f;

// ------------------------------------------------------------------------------------------------
SceneNode::SceneNode(const BoundingBox& box)
	: Box(box),
	  Parent(NULL),
	  Index(0)
{
	for (size_t i = 0; i < 8; ++i)
	{
		Child[i] = NULL;
	}
}

// ------------------------------------------------------------------------------------------------
SceneNode::~SceneNode()
{
	for (size_t i = 0; i < 8; ++i)
	{
		if (Child[i] != NULL)
		{
			delete Child[i];
		}
	}
}

// ------------------------------------------------------------------------------------------------
bool SceneNode::IsEmpty()
{
	if (Items.size() > 0)
	{
		return false;
	}

	for (size_t i = 0; i < 8; ++i)
	{
		if (Child[i] != NULL)
		{
			return false;
		}
	}

	return true;
}

// ------------------------------------------------------------------------------------------------
SceneNode* SceneNode::Next(const Shape& clip)
{
	for (size_t i = 0; i < 8; ++i)
	{
		if (Child[i] && clip.Intersect(Child[i]->Box) != OUTSIDE)
		{
			return Child[i];
		}
	}
		
	SceneNode *node = this;
	while (node->Parent != NULL)
	{
		for (size_t i = node->Index + 1; i < 8; ++i)
		{
			if (node->Parent->Child[i] && clip.Intersect(node->Parent->Child[i]->Box) != OUTSIDE)
			{
				return node->Parent->Child[i];
			}
		}

		node = node->Parent;
	}
	
	return NULL;
}

// ------------------------------------------------------------------------------------------------
Scene::Scene(float width, float height, float depth)
{
	mRoot = new SceneNode(BoundingBox(
		-width, -height, -depth, 
		width * 2.0f, height * 2.0f, depth * 2.0f
	));
}

// ------------------------------------------------------------------------------------------------
Scene::~Scene()
{
	if (mRoot != NULL)
	{
		delete mRoot;
	}

	std::hash_map<unsigned, Entity*>::iterator it;
	for (it = mEntities.begin(); it != mEntities.end(); ++it)
	{
		delete it->second;
	}
}

// ------------------------------------------------------------------------------------------------
Light* Scene::CreateLight(Light::Type type)
{
	Light* light;
	unsigned id;

	light = new Light(type);
	id = ++mEntityCount;
	mEntities.insert(std::make_pair(id, light));

	return light;
}

// ------------------------------------------------------------------------------------------------
Object* Scene::CreateObject()
{
	Object* object;
	unsigned id;

	object = new Object();
	id = ++mEntityCount;
	mEntities.insert(std::make_pair(id, object));

	return object;
}

// ------------------------------------------------------------------------------------------------
void Scene::Update()
{	
	std::hash_map<unsigned, Entity*>::iterator it;
	for (it = mEntities.begin(); it != mEntities.end(); ++it)
	{
		Entity *ent = it->second;

		if (ent->IsDirty())
		{
			RemoveEntity(ent);
			ent->Update();
			AddEntity(ent);
		}
	}
}

// ------------------------------------------------------------------------------------------------
void Scene::RemoveEntity(Entity* entity)
{
	SceneNode* node;
	
	node = entity->mParentNode;
	if (node == NULL)
	{
		return;
	}

	size_t idx = -1;
	for (size_t i = 0; i < node->Items.size(); ++i)
	{
		if (node->Items[i] == entity)
		{
			idx = i;
			break;
		}
	}
	
	assert(idx != -1);
	node->Items.erase(node->Items.begin() + idx);

	while (node->IsEmpty() && node->Parent)
	{
		SceneNode* parent;

		parent = node->Parent;
		parent->Child[node->Index] = NULL;
		delete node;

		node = parent;
	}
}
		
// ------------------------------------------------------------------------------------------------
void Scene::AddEntity(Entity* entity)
{
	SceneNode* target = mRoot;
	for (size_t lvl = 0, found = true; lvl < MAX_DEPTH && found; ++lvl)
	{
		float width = target->Box.GetSize().x / 2.0f * (1 + NODE_SCALE);
		float height = target->Box.GetSize().y / 2.0f * (1 + NODE_SCALE);
		float depth = target->Box.GetSize().z / 2.0f * (1 + NODE_SCALE);
		float x = target->Box.GetPosition().x;
		float y = target->Box.GetPosition().y;
		float z = target->Box.GetPosition().z;

		float offX = target->Box.GetSize().x  / 2.0f * (1 - NODE_SCALE);
		float offY = target->Box.GetSize().y  / 2.0f * (1 - NODE_SCALE);
		float offZ = target->Box.GetSize().z  / 2.0f * (1 - NODE_SCALE);

		found = false;
		for (size_t i = 0; i < 8; ++i)
		{
			if (target->Child[i] == NULL)
			{
				BoundingBox box(
					x + ((i & 1) ? offX : 0.0f),
					y + ((i & 2) ? offY : 0.0f),
					z + ((i & 4) ? offZ : 0.0f),
					width,
					height,
					depth
				);

				if (box.Intersect(entity->GetBoundingBox()) == INSIDE)
				{
					target->Child[i] = new SceneNode(box);
					target->Child[i]->Index = i;
					target->Child[i]->Parent = target;
					target = target->Child[i];
					found = true;
					break;
				}
			}
			else
			{
				if (target->Child[i]->Box.Intersect(entity->GetBoundingBox()) == INSIDE)
				{
					target = target->Child[i];
					found = true;
					break;
				}
			}
		}
	}

	target->Items.push_back(entity);
}

// ------------------------------------------------------------------------------------------------
void Scene::QueryScene(const ViewFrustum& volume, RenderBuffer* buffer)
{
	SceneNode* node = mRoot;
	std::vector<Entity*>::iterator it;

	while (node)
	{
		for (it = node->Items.begin(); it != node->Items.end(); ++it)
		{
			Entity *ent = *it;
			
			if (volume.Intersect(ent->GetBoundingBox()) != OUTSIDE)
			{
				switch (ent->GetEntityType())
				{
					case Entity::LIGHT:
					{
						buffer->Lights.resize(buffer->Lights.size() + 1);
						((Light*)ent)->Prepare(&(*buffer->Lights.rbegin()));
						break;
					}	
					case Entity::OBJECT:
					{
						buffer->Objects.resize(buffer->Objects.size() + 1);
						((Object*)ent)->Prepare(&(*buffer->Objects.rbegin()));
						break;
					}
					case Entity::PARTICLE:
					{
						break;
					}
				}
			}
		}

		node = node->Next(volume);
	}
}

// ------------------------------------------------------------------------------------------------
void Scene::QueryShadows(const ViewFrustum& volume, RenderBuffer* buffer)
{
	SceneNode* node = mRoot;
	std::vector<Entity*>::iterator it;

	while (node)
	{
		for (it = node->Items.begin(); it != node->Items.end(); ++it)
		{
			if ((*it)->GetEntityType() == Entity::OBJECT && 
				volume.Intersect((*it)->GetBoundingBox()) != OUTSIDE &&
				(*it)->IsShadowCaster())
			{
				buffer->ShadowCasters.resize(buffer->ShadowCasters.size() + 1);
				((Object*)(*it))->Prepare(&(*buffer->ShadowCasters.rbegin()));
			}
		}

		node = node->Next(volume);
	}
}

// ------------------------------------------------------------------------------------------------
glm::vec3 Scene::QueryDistance(const BoundingBox& entity, const glm::vec3& dir)
{	
	BoundingBox extended = entity.Extend(dir);
	std::vector<BoundingBox> objects;
	std::vector<Entity*>::iterator it;
	glm::vec3 final = dir;

	SceneNode* node = mRoot;
	while (node)
	{
		for (it = node->Items.begin(); it != node->Items.end(); ++it)
		{
			if (extended.Intersect((*it)->GetBoundingBox()) == INTERSECT && (*it)->IsCollider())
			{
				objects.push_back((*it)->GetBoundingBox());
			}
		}

		node = node->Next(extended);
	}

	if (objects.size() > 0)
	{
		BoundingBox box;

		box = entity.Extend(glm::vec3(dir.x, 0.0f, 0.0f));
		for (size_t i = 0; i < objects.size(); ++i)
		{
			if (box.Intersect(objects[i]) == INTERSECT)
			{
				if (dir.x >= 0.0f)
				{
					final.x = std::min(objects[i].GetMin().x - entity.GetMax().x, final.x);
				}
				else
				{
					final.x = std::max(objects[i].GetMax().x - entity.GetMin().x, final.x);
				}
			}
		}

		box = entity.Extend(glm::vec3(final.x, dir.y, 0.0f));
		for (size_t i = 0; i < objects.size(); ++i)
		{
			if (box.Intersect(objects[i]) == INTERSECT)
			{
				if (dir.y >= 0.0f)
				{
					final.y = std::min(objects[i].GetMin().y - entity.GetMax().y, final.y);
				}
				else
				{
					final.y = std::max(objects[i].GetMax().y - entity.GetMin().y, final.y);
				}
			}
		}
		
		box = entity.Extend(glm::vec3(final.x, final.y, dir.z));
		for (size_t i = 0; i < objects.size(); ++i)
		{
			if (box.Intersect(objects[i]) == INTERSECT)
			{
				if (dir.z >= 0.0f)
				{
					final.z = std::min(objects[i].GetMin().z - entity.GetMax().z , final.z);
				}
				else
				{
					final.z = std::max(objects[i].GetMax().z - entity.GetMin().z, final.z);
				}
			}
		}

		return final;
	}

	return dir;
}
