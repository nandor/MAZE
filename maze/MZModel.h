// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MODEL_H
#define MODEL_H
#pragma once

#include "MZMath.h"
#include "MZTexture.h"

namespace MAZE
{
	class Model : public Resource
	{
	public:
		
		/// Template related stuff
		const static Resource::Type TYPE = Resource::MODEL;
		
		/**
			Single vertex of the model
		*/
		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 UV;
		};
				
	public:

		/**
			Crates a new model
			@param rsmngr Resource Manager
			@param id ID of the model
			@param fn File Name
		*/
		Model(ResourceManager* rsmngr, const std::string& id, const std::string& fn);

		/**
			Destroys the model
		*/
		~Model();
				
		/**
			Loads the model from the file
		*/
		void Load();

		/**
			Unloads the model
		*/
		void Unload();
		
		/**
			The renderer builds the VAOs
		*/
		void Build();

		/**
			The renderer destroys the VAOs
		*/
		void Destroy();
		
	public:

		/**
			Creates a simple plane mesh
			@param rsmngr	Target rsmngr
			@param id		ID of the mesh
			@param size		Size of the plane
			@param cellSize Size of a texture cell
		*/
		static void CreatePlane(ResourceManager* rsmngr, 
								const std::string& id, 
								const std::string& diffuse,
								const std::string& bump,
								const glm::vec2& size,
								const glm::vec2& cellSize);

	private:

		/// Pointer modifies reference count
		friend class Resource::Ptr<Model>;

		/// Renderer needs access to the vbo
		friend class Renderer;

	private:

		/// Name of the source file
		std::string mFile;
		
		/// List of vertices of the mesh
		std::vector<Vertex> mVertices;

		/// List of vertices of the collision mesh
		std::vector<glm::vec3> mCollision;

		/// Number of vertices of the mesh
		size_t mVertexCount;
		
		/// Mesh VBO
		MGLuint mVBO;
		
		/// Diffuse map
		Resource::Ptr<Texture> mDiffuseMap;

		/// Normal map
		Resource::Ptr<Texture> mBumpMap;

		/// Bounding box min point
		glm::vec3 mBoxWorldMin;

		/// Bounding box max point
		glm::vec3 mBoxWorldMax;
	};
};

#endif
