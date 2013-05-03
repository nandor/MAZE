// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef OCTREE_H
#define OCTREE_H
#pragma once

#include <list>
#include <hash_map>
#include "MZAtomic.h"
#include "MZLight.h"
#include "MZBound.h"

namespace MAZE
{
	class Entity;
	class RenderBuffer;
	
	/**
		A node in the octree
	*/
	class SceneNode
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
		SceneNode* Next(const Shape& clip);
			
	public:
		
		/// List of entities attached to the node
		std::vector<Entity*> Items;

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
		Scene(Engine* engine, float width, float height, float depth);

		/**
			Initializes the tree
		*/
		Scene(Engine* engine, const glm::vec3& size);

		/**
			Clears up every entity
		*/
		~Scene();
				
		/**
			Updates all entities
		*/
		void Update();

		/**
			Creates a new entity
		*/
		template <class T>
		T* Create()
		{
			T* entity;
			unsigned id;

			id = ++mEntityCount;

			entity = new T();
			entity->fScene = this;
			entity->fEngine = mEngine;
			entity->fHandle = id;

			mEntities.insert(std::make_pair(id, entity));

			return entity;
		}

		/**
			Retrieves all visible entities
		*/
		void QueryScene(const ViewFrustum& volume, RenderBuffer* buffer);

		/**
			Retrieves all shadows casters from a volume
		*/
		void QueryShadows(const ViewFrustum& volume, RenderBuffer* buffer);

		/**
			Retrieves maximum move distance
		*/
		glm::vec3 QueryDistance(const BoundingBox& box, const glm::vec3& dir);
		
		/**
			Updates the position of an entity
		*/
		void UpdateEntity(Entity *ent)
		{
			RemoveEntity(ent);
			AddEntity(ent);
		}

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

		/// Looseness factor
		const static float NODE_SCALE;

		/// Parent engine
		Engine *mEngine;

		/// Root of the tree
		SceneNode *mRoot;

		/// Handle - Entity mapping
		std::hash_map<unsigned, Entity*> mEntities;

		/// Entity count
		Atomic<unsigned> mEntityCount;
	};
};

#endif
