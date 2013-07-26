// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
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
Scene::Scene(Engine *engine, const glm::vec3& size)
	: mEngine(engine),
	  mRoot(new SceneNode(BoundingBox(
		glm::vec3(-3.0f, 0.0f, -3.0f), 
		size + glm::vec3(6.0f, 0.0f, 6.0f)
	)))
{
}

// ------------------------------------------------------------------------------------------------
Scene::~Scene()
{
	if (mRoot != NULL)
	{
		delete mRoot;
	}

	std::unordered_map<unsigned, Entity*>::iterator it;
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
	__m128 mask[] =
	{
		_mm_castsi128_ps(_mm_setr_epi32(0x00000000, 0x00000000, 0x00000000, 0x00000000)),
		_mm_castsi128_ps(_mm_setr_epi32(0xFFFFFFFF, 0x00000000, 0x00000000, 0x00000000)),
		_mm_castsi128_ps(_mm_setr_epi32(0x00000000, 0xFFFFFFFF, 0x00000000, 0x00000000)),
		_mm_castsi128_ps(_mm_setr_epi32(0xFFFFFFFF, 0xFFFFFFFF, 0x00000000, 0x00000000)),
		_mm_castsi128_ps(_mm_setr_epi32(0x00000000, 0x00000000, 0xFFFFFFFF, 0x00000000)),
		_mm_castsi128_ps(_mm_setr_epi32(0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0x00000000)),
		_mm_castsi128_ps(_mm_setr_epi32(0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000)),
		_mm_castsi128_ps(_mm_setr_epi32(0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x00000000)),
	};

	SceneNode* target = mRoot;
	for (size_t lvl = 0, found = true; lvl < MAX_DEPTH && found; ++lvl)
	{
		__m128 size, off, pos;

		pos  = target->Box.GetMin();
		size = _mm_sub_ps(target->Box.GetMax(), pos);
		off  = _mm_mul_ps(size, _mm_set_ps1(0.4f));
		size = _mm_mul_ps(size, _mm_set_ps1(0.6f));

		found = false;
		for (size_t i = 0; i < 8; ++i)
		{
			if (target->Child[i] == NULL)
			{
				__m128 min = _mm_add_ps(pos, _mm_and_ps(mask[i], off));
				__m128 max = _mm_add_ps(min, size);

				size_t mask = _mm_movemask_ps(_mm_and_ps(
					_mm_cmple_ps(min, entity->GetBoundingBox().GetMin()),
					_mm_cmple_ps(entity->GetBoundingBox().GetMax(), max)
				));

				if ((mask & 7) == 7)
				{
					target->Child[i] = new SceneNode(BoundingBox(min, size));
					target->Child[i]->Index = i;
					target->Child[i]->Parent = target;
					target = target->Child[i];
					found = true;
					break;
				}
			}
			else
			{
				size_t mask = _mm_movemask_ps(_mm_and_ps(
					_mm_cmple_ps(target->Child[i]->Box.GetMin(), entity->GetBoundingBox().GetMin()),
					_mm_cmple_ps(entity->GetBoundingBox().GetMax(), target->Child[i]->Box.GetMax())
				));

				if ((mask & 7) == 7)
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
	std::unordered_map<unsigned, Entity*>::iterator it = mEntities.begin();
	while (it != mEntities.end())
	{
		Entity *ent = it->second;

		if (ent->fDelete && ent->IsDeletable())
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
void Scene::Delete()
{
	std::unordered_map<unsigned, Entity*>::iterator it = mEntities.begin();
	while (it != mEntities.end())
	{
        if (!it->second->fPersistent)
        {
            it->second->fDelete = true;
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
	BoundingBox ext = entity.Extend(dir);
	__m128 move;
		
	move = _mm_setr_ps(dir.x, dir.y, dir.z, 0.0f);
		
	SceneNode* node = mRoot;
	while (node)
	{
		std::forward_list<Entity*>::iterator it;
		for (it = node->Items.begin(); it != node->Items.end(); ++it)
		{
			const BoundingBox& box = (*it)->GetBoundingBox();
							
			if ((*it)->IsCollider() && (*it)->IsActive() && !box.Outside(ext))
			{
				if ((*it)->GetType() == Entity::OBJECT && (*it)->HasCollisionMesh())
				{
					glm::mat4 mtx = (*it)->GetModelMat();
					std::vector<glm::vec3> mesh = ((Object*)(*it))->GetModel()->GetCollisionMesh();

					for (size_t i = 0; i < mesh.size(); i += 3)
					{
						__m128 a, b, c, n, l, d;

						// Retrieve world space coordinated of the triangle
						a = _mm_loadu_ps(glm::value_ptr(mtx * glm::vec4(mesh[i + 0], 1.0f)));
						b = _mm_loadu_ps(glm::value_ptr(mtx * glm::vec4(mesh[i + 1], 1.0f)));
						c = _mm_loadu_ps(glm::value_ptr(mtx * glm::vec4(mesh[i + 2], 1.0f)));

						// Compute & normalize the normal vector
						n = Cross(_mm_sub_ps(b, a), _mm_sub_ps(c, a));
						l = _mm_mul_ps(n, n);
						l = _mm_hadd_ps(l, l);
						l = _mm_hadd_ps(l, l);
						l = _mm_sqrt_ps(l);
						n = _mm_div_ps(n, l);

						// Compute the D component of the plane: d = dot(n, a)
						d = _mm_mul_ps(n, a);
						d = _mm_hadd_ps(d, d);
						d = _mm_hadd_ps(d, d);						
						d = Invert(d);

						// n.w = d
						_mm_store_ss((float*)&n + 3, d);
						
						for (size_t j = 0; j < 8; ++j)
						{
							__m128 d1, d2, mask;

							// d1 = dist(plane, c)
							d1 = _mm_mul_ps(n, entity.GetCorner(j));
							d1 = _mm_hadd_ps(d1, d1);
							d1 = _mm_hadd_ps(d1, d1);

							// d2 = dist(plane, c + move)
							d2 = _mm_mul_ps(n, ext.GetCorner(j));
							d2 = _mm_hadd_ps(d2, d2);
							d2 = _mm_hadd_ps(d2, d2);
							
							// EPS <= d1 && -EPS <= d2
							mask = _mm_and_ps(
								_mm_cmplt_ps(d2, _mm_setzero_ps()), 
								_mm_cmplt_ps(_mm_setzero_ps(), d1)
							);

							if (_mm_movemask_ps(mask) == 0xF)
							{
								__m128 t;
								
								// t = corner + move - d2 * n
								t = _mm_add_ps(
									entity.GetCorner(j), 
									_mm_sub_ps(move, _mm_mul_ps(d2, n))
								);
								
								if (Inside(a, b, c, t))
								{
									d2 = _mm_sub_ps(_mm_set_ps1(EPS), d2);
									d2 = _mm_mul_ps(d2, n);
									move = _mm_add_ps(move, d2);
									ext = entity.Extend(move);
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
							__m128 d1, d2, plane, mask;

							plane = box.GetPlane(i);

							d1 = _mm_mul_ps(plane, entity.GetCorner(j));
							d1 = _mm_hadd_ps(d1, d1);
							d1 = _mm_hadd_ps(d1, d1);

							d2 = _mm_mul_ps(plane, ext.GetCorner(j));
							d2 = _mm_hadd_ps(d2, d2);
							d2 = _mm_hadd_ps(d2, d2);
							
							// EPS <= d1 && -EPS <= d2
							mask = _mm_and_ps(
								_mm_cmplt_ps(d2, _mm_setzero_ps()), 
								_mm_cmplt_ps(_mm_setzero_ps(), d1)
							);
							
							if (_mm_movemask_ps(mask) == 0xF)
							{
								d2 = _mm_sub_ps(_mm_set_ps1(EPS), d2);
								d2 = _mm_mul_ps(d2, plane);
								move = _mm_add_ps(move, d2);
								ext = entity.Extend(move);
							}
						}
					}
				}
			}
		}

		node = node->Next(ext);
	}

	move = _mm_max_ps(move, _mm_sub_ps(mRoot->Box.GetMin(), entity.GetMin()));
	move = _mm_min_ps(move, _mm_sub_ps(mRoot->Box.GetMax(), entity.GetMax()));

	return glm::vec3(((float*)&move)[0], ((float*)&move)[1], ((float*)&move)[2]);
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
	SceneNode* node = mRoot;
	Entity* entity = NULL;
	__m128 min, dist, cmp;
	
	min = _mm_set_ps1(FLT_MAX);
	while (node)
	{
		std::forward_list<Entity*>::iterator it;
		for (it = node->Items.begin(); it != node->Items.end(); ++it)
		{
			if ((*it)->IsActive() &&
				(*it)->IsUseable() &&
				!(*it)->GetBoundingBox().Outside(ray))
			{
				dist = ray.Distance((*it)->GetBoundingBox());

				cmp = _mm_and_ps(_mm_cmplt_ss(dist, min), _mm_and_ps(
					_mm_cmple_ss(_mm_set_ss(0.0f), dist),
					_mm_cmple_ss(dist, _mm_set_ss(2.5f))
				));
				
				if ((_mm_movemask_ps(cmp) & 1) == 1)
				{
					min = dist;
					entity = (*it);
				}
			}
		}

		node = node->Next(ray);
	}

	return entity;
}
