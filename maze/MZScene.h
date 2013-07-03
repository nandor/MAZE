// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef OCTREE_H
#define OCTREE_H
#pragma once

namespace MAZE
{
	class Entity;
	class Engine;
	class RenderBuffer;
	
	/**
		A node in the octree
	*/
	class SceneNode : public Aligned
	{
	public:

		/**
			Initializes the node
		*/
		SceneNode(const BoundingBox& box);

		/**
			Clears up attached entities
		*/
		~SceneNode();

		/**
			Checks if the node is empty
		*/
		bool IsEmpty();

		/**
			Returns the next node in the traversal
			@param clip Clipping volume
		*/
		template <typename T>
		SceneNode* Next(const T& v)
		{
			for (size_t i = 0; i < 8; ++i)
			{
				if (Child[i] && !v.Outside(Child[i]->Box))
				{
					return Child[i];
				}
			}
		
			SceneNode *node = this, *parent;
			while (node->Parent != NULL)
			{
				parent = node->Parent;
				for (size_t i = node->Index + 1; i < 8; ++i)
				{
					if (parent->Child[i] && !v.Outside(parent->Child[i]->Box))
					{
						return parent->Child[i];
					}
				}

				node = parent;
			}
	
			return NULL;
		}
			
	public:
		
		/// List of entities attached to the node
		std::forward_list<Entity*> Items;

		/// Bounding box of the node
		BoundingBox Box;
		
		/// List of children
		SceneNode *Child[8];

		/// Parent node
		SceneNode *Parent;

		/// Index which shows which child is this node
		size_t Index;
	};

	/**
		Scene manager - handles all game entities
	*/
	class Scene
	{
	public:
		
		/**
			Initializes the tree
		*/
		Scene(Engine* engine, const glm::vec3& size);

		/**
			Clears up every entity
		*/
		~Scene();

		/**
			Creates a new entity
		*/
		template <class T>
		T* Create()
		{
			T* entity;
			unsigned id;

			id = ++mEntityCount;

			entity = new T(mEngine);
			entity->fScene = this;
			entity->fHandle = id;

			mEntities.insert(std::make_pair(id, entity));

			return entity;
		}

		/**
			Creates a new entity
		*/
		template <class T>
		T* Create(const std::string& name)
		{
			T* entity;
			unsigned id;
			std::unordered_map<std::string, Entity*>::iterator it;

			if (name == "")
			{
				return Create<T> ();
			}

			if ((it = mNamedEntities.find(name)) != mNamedEntities.end())
			{
				throw Exception("Duplicate entity name: '" + name + "'");
			}

			id = ++mEntityCount;

			entity = new T(mEngine);
			entity->fScene = this;
			entity->fHandle = id;
			entity->fName = name;

			mEntities.insert(std::make_pair(id, entity));
			mNamedEntities.insert(std::make_pair(name, entity));

			return entity;
		}

		/**
			Retrieves an entity
		*/
		template <class  T>
		T* Get(const std::string& name)
		{
			std::unordered_map<std::string, Entity*>::iterator it;

			if ((it = mNamedEntities.find(name)) == mNamedEntities.end())
			{
				throw Exception("Entity '" + name + "' not found");
			}

			return static_cast<T*> (it->second);
		}

		/**
			Retrieves all visible entities
		*/
		void QueryRenderables(const Frustum& volume, RenderBuffer* buffer);

		/**
			Retrieves all shadows casters from a volume
		*/
		void QueryShadowCasters(const Frustum& volume, RenderBuffer* buffer);
		
		/**
			Retrieves maximum move distance
		*/
		glm::vec3 QueryDistance(Entity* who, const glm::vec3& dir);

		/**
			Handle all entities which can be picked up
		*/
		void QueryPickables(Entity *who);

		/**
			Get an entity which might be used by the player
		*/
		Entity* QueryUseable(Entity *who, const Ray& ray);

		/**
			Updates every entity from the scene
		*/
		void Update(float time, float dt);

	private:

		/**
			Removes an entity from the tree
		*/
		void RemoveEntity(Entity *ent);	

		/**
			Adds an entity to the tree
		*/
		void AddEntity(Entity *ent);
		
	private:

		/// Max depth of the tree
		const static size_t MAX_DEPTH = 16;

		/// Parent engine
		Engine *mEngine;

		/// Root of the tree
		SceneNode *mRoot;

		/// Handle - Entity mapping
		std::unordered_map<unsigned, Entity*> mEntities;

		/// Name - Entity mapping
		std::unordered_map<std::string, Entity*> mNamedEntities;

		/// Entity count
		Atomic<unsigned> mEntityCount;
	};
};

#endif
