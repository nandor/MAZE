// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include <new.h>
#include <stack>
#include "MZLog.h"
#include "MZRay.h"
#include "MZEngine.h"
#include "MZLight.h"
#include "MZScene.h"
#include "MZObject.h"
#include "MZEntity.h"
#include "MZCamera.h"
#include "MZFrustum.h"
#include "MZPlatform.h"
#include "MZBoundingBox.h"
#include "MZRenderBuffer.h"
using namespace MAZE;

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
	if (!Items.empty())
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
Scene::Scene(Engine* engine, float width, float height, float depth)
	: mEngine(engine),
	  mRoot(new SceneNode(BoundingBox(
		  0.0f, 0.0f, 0.0f,
		  width, height, depth
	  )))
{
}

// ------------------------------------------------------------------------------------------------
Scene::Scene(Engine *engine, const glm::vec3& size)
	: mEngine(engine),
	  mRoot(new SceneNode(BoundingBox(glm::vec3(0.0f), size)))
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
	if ((node = entity->fParentNode) == NULL)
	{
		return;
	}

	node->Items.remove(entity);

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
		float width  = target->Box.GetSize().x * 0.6f;
		float height = target->Box.GetSize().y * 0.6f;
		float depth  = target->Box.GetSize().z * 0.6f;
		float offX = target->Box.GetSize().x * 0.4f;
		float offY = target->Box.GetSize().y * 0.4f;
		float offZ = target->Box.GetSize().z * 0.4f;
		float x = target->Box.GetMin().x;
		float y = target->Box.GetMin().y;
		float z = target->Box.GetMin().z;

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

				if (box.Inside(entity->GetBoundingBox()))
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
				if (target->Child[i]->Box.Inside(entity->GetBoundingBox()))
				{
					target = target->Child[i];
					found = true;
					break;
				}
			}
		}
	}

	entity->fParentNode = target;
	target->Items.push_front(entity);
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
void Scene::QueryRenderables(const Frustum& v, RenderBuffer* buffer)
{
	/*
		In order to perform an iterative traversal, each node must save
		its index in its parent's child array and a link to the parent.

		When searching for the next node, we first search its right siblings.
		If no siblings match, we move up the tree until we find a parent
		whose right siblings are visible. This algorithm ensures that
		each node is visited once.

		If a node is inside the frustum, its children are visible too,
		so no further tests are required.
	*/

	SceneNode *node = mRoot, *tmp, *end = NULL;
	std::forward_list<Entity*>::iterator it;
	bool test = true;

	while (node)
	{
		for (it = node->Items.begin(); it != node->Items.end(); ++it)
		{
			if ((*it)->IsActive() &&
				(*it)->IsRenderable() &&
				(!test || !(*it)->GetBoundingBox().Outside(v)))
			{
				(*it)->Render(buffer, Entity::RENDER_GBUFFER);
			}
		}
		
		tmp = node;
		node = NULL;

		for (size_t i = 0; i < 8; ++i)
		{
			if (tmp->Child[i])
			{
				if (!test || v.Inside(tmp->Child[i]->Box))
				{
					test = false;
					node = tmp->Child[i];
					end = tmp;
					break;
				}

				if (!v.Outside(tmp->Child[i]->Box))
				{
					test = true;
					node = tmp->Child[i];
					end = NULL;
					break;
				}
			}
		}
		
		if (!node)
		{
			SceneNode *next = tmp, *parent;
			while (next->Parent != NULL && !node)
			{
				if ((parent = next->Parent) == end)
				{
					test = true;
				}

				for (size_t i = next->Index + 1; i < 8; ++i)
				{
					if (parent->Child[i])
					{
						if (!test || !v.Outside(parent->Child[i]->Box))
						{
							test = false;
							node = parent->Child[i];
							end = parent;
							break;
						}

						if (!v.Outside(parent->Child[i]->Box))
						{
							test = true;
							node = parent->Child[i];
							end = NULL;
							break;
						}
					}
				}

				next = parent;
			}
		}
	}
}

// ------------------------------------------------------------------------------------------------
void Scene::QueryShadowCasters(const Frustum& volume, RenderBuffer* buffer)
{
	SceneNode* node = mRoot;
	std::forward_list<Entity*>::iterator it;
	
	if (mEngine->GetSetup().EnableShadows)
	{
		while (node)
		{
			for (it = node->Items.begin(); it != node->Items.end(); ++it)
			{
				if ((*it)->IsActive() &&
					(*it)->IsRenderable() &&
					(*it)->IsShadowCaster() &&
					(*it)->GetType() != Entity::LIGHT &&
					!(*it)->GetBoundingBox().Outside(volume))
				{
					(*it)->Render(buffer, Entity::RENDER_SHADOW);
				}
			}

			node = node->Next(volume);
		}
	}
}

// ------------------------------------------------------------------------------------------------
glm::vec3 Scene::QueryDistance(Entity* who, const glm::vec3& dir)
{	
	BoundingBox entity = who->GetBoundingBox();
	BoundingBox extended = entity.Extend(dir);
	BoundingBox temp = entity.Extend(dir);
	glm::vec3 move = dir;
		
	SceneNode* node = mRoot;
	while (node)
	{
		std::forward_list<Entity*>::iterator it;
		for (it = node->Items.begin(); it != node->Items.end(); ++it)
		{
			BoundingBox& box = (*it)->GetBoundingBox();
				
			if ((*it)->IsCollider() && (*it)->IsActive() && !box.Outside(extended))
			{
				if ((*it)->GetType() == Entity::OBJECT && (*it)->HasCollisionMesh())
				{
					glm::mat4 mtx = (*it)->GetModelMat();
					std::vector<glm::vec3> mesh = ((Object*)(*it))->GetModel()->GetCollisionMesh();

					for (size_t i = 0; i < mesh.size(); i += 3)
					{
						glm::vec3 a(mtx * glm::vec4(mesh[i + 0], 1.0f));
						glm::vec3 b(mtx * glm::vec4(mesh[i + 1], 1.0f));
						glm::vec3 c(mtx * glm::vec4(mesh[i + 2], 1.0f));

						glm::vec3 n = glm::normalize(glm::cross(b - a, c - a));
						float d = -glm::dot(n, a);
						
						for (size_t j = 0; j < 8; ++j)
						{
							float d1 = glm::dot(n, entity.GetCorner(j)) + d;
							float d2 = glm::dot(n, temp.GetCorner(j)) + d;

							if (d1 >= 0.0f && d2 <= 0.0f)
							{
								glm::vec3 pt = entity.GetCorner(j) + move - d2 * n;

								if (::Inside(a, b, c, pt))
								{
									move += (-d2 + EPS) * n;
									temp = entity.Extend(move);
								}
							}
						}
					}
				}
				else
				{
					for (size_t i = 0; i < 6; ++i)
					{
						for (size_t j = 0; j < 8; ++j)
						{
							glm::vec4& plane = box.GetPlane(i);

							float d1 = glm::dot(plane, glm::vec4(entity.GetCorner(j), 1.0f));
							float d2 = glm::dot(plane, glm::vec4(temp.GetCorner(j), 1.0f));

							if (d1 >= 0.0f && d2 <= 0.0f)
							{
								move += (-d2 + EPS) * glm::vec3(plane);
								temp = entity.Extend(move);
							}
						}
					}
				}
			}
		}

		node = node->Next(extended);
	}

	move = glm::max(move, mRoot->Box.GetMin() - entity.GetMin());
	move = glm::min(move, mRoot->Box.GetMax() - entity.GetMax());
	return move;
}

// ------------------------------------------------------------------------------------------------
void Scene::QueryPickables(Entity *who)
{
	std::vector<Entity*> objects;	
	SceneNode* node = mRoot;

	while (node)
	{
		std::forward_list<Entity*>::iterator it;
		for (it = node->Items.begin(); it != node->Items.end(); ++it)
		{
			if ((*it)->IsPickable() &&
				(*it)->IsActive() &&
				!(*it)->GetBoundingBox().Outside(who->GetBoundingBox()))
			{
				(*it)->OnPick(who);
			}
		}

		node = node->Next(who->GetBoundingBox());
	}
}

// ------------------------------------------------------------------------------------------------
Entity* Scene::QueryUseable(Entity *who, const Ray& ray)
{
	std::vector<Entity*> objects;	
	SceneNode* node = mRoot;
	Entity* entity = NULL;
	float minDist = std::numeric_limits<float>::max();

	while (node)
	{
		std::forward_list<Entity*>::iterator it;
		for (it = node->Items.begin(); it != node->Items.end(); ++it)
		{
			if ((*it)->IsActive() &&
				(*it)->IsUseable() &&
				!(*it)->GetBoundingBox().Outside(ray))
			{
				float dist = ray.Distance((*it)->GetBoundingBox());

				if (dist < minDist && 0.0f <= dist && dist <= 3.5f)
				{
					minDist = dist;
					entity = (*it);
				}
			}
		}

		node = node->Next(ray);
	}

	return entity;
}
