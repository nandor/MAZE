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
Scene::Scene(Engine* engine, float width, float height, float depth)
	: mEngine(engine),
	  mRoot(new SceneNode(BoundingBox(
		-width, -height, -depth, 
		width * 2.0f, height * 2.0f, depth * 2.0f
	  )))
{
}

// ------------------------------------------------------------------------------------------------
Scene::Scene(Engine *engine, const glm::vec3& size)
	: mEngine(engine),
	  mRoot(new SceneNode(BoundingBox(-size, size * 2.0f)))
{
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
void Scene::RemoveEntity(Entity* entity)
{
	SceneNode* node;
	
	node = entity->fParentNode;
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

	entity->fParentNode = target;
	target->Items.push_back(entity);
}

// ------------------------------------------------------------------------------------------------
void Scene::Update(float time, float dt)
{
	std::hash_map<unsigned, Entity*>::iterator it = mEntities.begin();
	while (it != mEntities.end())
	{
		Entity *ent = it->second;

		if (ent->fDelete)
		{
			mNamedEntities.erase(ent->GetName());
			RemoveEntity(ent);
			delete ent;
			mEntities.erase(it++);
			continue;
		}

		if (ent->IsActive())
		{
			ent->Update(time, dt);

			if (ent->fDirty)
			{
				RemoveEntity(ent);
				ent->UpdateInternals();
				ent->fDirty = false;
				AddEntity(ent);
			}
		}

		++it;
	}
}

// ------------------------------------------------------------------------------------------------
void Scene::QueryRenderables(const ViewFrustum& volume, RenderBuffer* buffer)
{
	SceneNode* node = mRoot;
	std::vector<Entity*>::iterator it;

	while (node)
	{
		for (it = node->Items.begin(); it != node->Items.end(); ++it)
		{
			if ((*it)->IsActive() &&
				(*it)->IsRenderable() &&
				(*it)->GetBoundingBox().Intersect(volume))
			{
				(*it)->Render(buffer, Entity::RENDER_GBUFFER);
			}
		}

		node = node->Next(volume);
	}

	buffer->Sort(volume.GetView());
}

// ------------------------------------------------------------------------------------------------
void Scene::QueryShadowCasters(const ViewFrustum& volume, RenderBuffer* buffer)
{
	SceneNode* node = mRoot;
	std::vector<Entity*>::iterator it;
	
	while (node)
	{
		for (it = node->Items.begin(); it != node->Items.end(); ++it)
		{
			if ((*it)->IsActive() &&
				(*it)->IsRenderable() &&
				(*it)->IsShadowCaster() &&
				(*it)->GetType() != Entity::LIGHT &&
				(*it)->GetBoundingBox().Intersect(volume) != OUTSIDE)
			{
				(*it)->Render(buffer, Entity::RENDER_SHADOW);
			}
		}

		node = node->Next(volume);
	}
}

// ------------------------------------------------------------------------------------------------
glm::vec3 Scene::QueryDistance(Entity* who, const glm::vec3& dir)
{	
	BoundingBox entity = who->GetBoundingBox();
	BoundingBox extended = entity.Extend(dir);
	std::vector<Entity*> objects;
	glm::vec3 final = dir;
	
	SceneNode* node = mRoot;
	while (node)
	{
		std::vector<Entity*>::iterator it;
		for (it = node->Items.begin(); it != node->Items.end(); ++it)
		{
			if ((*it)->IsCollider() &&
				(*it)->IsActive() &&
				(*it)->GetBoundingBox().Intersect(extended) == INTERSECT)
			{
				objects.push_back(*it);
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
			BoundingBox& target = objects[i]->GetBoundingBox();
			if (box.Intersect(target) == INTERSECT)
			{
				if (dir.x >= 0.0f)
				{
					final.x = std::min(target.GetMin().x - entity.GetMax().x, final.x);
				}
				else
				{
					final.x = std::max(target.GetMax().x - entity.GetMin().x, final.x);
				}
			}
		}
		
		box = entity.Extend(glm::vec3(final.x, 0.0f, dir.z));
		for (size_t i = 0; i < objects.size(); ++i)
		{
			BoundingBox& target = objects[i]->GetBoundingBox();
			if (box.Intersect(target) == INTERSECT)
			{
				if (dir.z >= 0.0f)
				{
					final.z = std::min(target.GetMin().z - entity.GetMax().z , final.z);
				}
				else
				{
					final.z = std::max(target.GetMax().z - entity.GetMin().z, final.z);
				}
			}
		}

		box = entity.Extend(glm::vec3(final.x, dir.y, final.z));
		for (size_t i = 0; i < objects.size(); ++i)
		{
			BoundingBox& target = objects[i]->GetBoundingBox();
			if (box.Intersect(target) == INTERSECT)
			{
				if (dir.y >= 0.0f)
				{
					final.y = std::min(target.GetMin().y - entity.GetMax().y, final.y);
				}
				else
				{
					final.y = std::max(target.GetMax().y - entity.GetMin().y, final.y);
				}
			}
		}

		return final;
	}

	return dir;
}

// ------------------------------------------------------------------------------------------------
void Scene::QueryPickables(Entity *who)
{
	std::vector<Entity*> objects;	
	SceneNode* node = mRoot;

	while (node)
	{
		std::vector<Entity*>::iterator it;
		for (it = node->Items.begin(); it != node->Items.end(); ++it)
		{
			if ((*it)->IsPickable() &&
				(*it)->IsActive() &&
				(*it)->GetBoundingBox().Intersect(who->GetBoundingBox()))
			{
				(*it)->OnPick(who);
			}
		}

		node = node->Next(who->GetBoundingBox());
	}
}

// ------------------------------------------------------------------------------------------------
Entity* Scene::QueryUseable(Entity *who)
{
	std::vector<Entity*> objects;	
	SceneNode* node = mRoot;
	Entity* entity = NULL;
	float angle = PI / 2.0f;

	while (node)
	{
		std::vector<Entity*>::iterator it;
		for (it = node->Items.begin(); it != node->Items.end(); ++it)
		{
			if ((*it)->IsPickable() &&
				(*it)->IsActive() &&
				(*it)->GetBoundingBox().Intersect(who->GetBoundingBox()))
			{
			}
		}

		node = node->Next(who->GetBoundingBox());
	}

	return entity;
}
