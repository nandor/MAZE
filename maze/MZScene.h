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
	class Light;
	class Object;
	class Camera;
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
		Scene(float width, float height, float depth);

		/**
			Clears up every entity
		*/
		~Scene();
		
		/**
			Creates a new light source
		*/
		Light* CreateLight(Light::Type type);

		/**
			Creates a new object
		*/
		Object* CreateObject();
		
		/**
			Updates all entities
		*/
		void Update();

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

	private:

		/**
			Removes an entity from the tree
		*/
		void RemoveEntity(Entity* entity);	

		/**
			Adds an entity to the tree
		*/
		void AddEntity(Entity* entity);
		
	private:

		/// Max depth of the tree
		const static size_t MAX_DEPTH = 16;

		/// Looseness factor
		const static float NODE_SCALE;

		/// Root of the tree
		SceneNode *mRoot;

		/// Handle - Entity mapping
		std::hash_map<unsigned, Entity*> mEntities;

		/// Entity count
		Atomic<unsigned> mEntityCount;
	};
};

#endif
