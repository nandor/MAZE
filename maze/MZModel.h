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

		/**
			Mesh & material associated with it
		*/
		class Mesh
		{
		public:

			/// Diffuse color of the surface
			glm::vec3 Diffuse;

			/// Texture map
			Resource::Ptr<Texture> DiffuseMap;

			/// Number of vertices composing the mesh
			size_t VertexCount;
			
			/// Start of the mesh
			size_t VertexOffset;
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
			@param w		Width of the plane
			@param h		Height of the plane
			@param cw		Texture repeat cell width
			@param ch		Texture repeat cell height
		*/
		static void CreatePlane(ResourceManager* rsmngr, const std::string& id, float w, float h, float cw, float ch);

	private:

		/// Name of the model file
		std::string mFile;

		/// Meshes composing the model
		std::vector<Mesh> mMeshes;

		/// All vertices from the model
		std::vector<Vertex> mVertices;
		
		/// VBO id
		MGLuint mVBO;

		/// Vertex count
		size_t mVertexCount;

		/// True if the model is loaded from a file
		bool mFromFile;

		friend class Resource::Ptr<Model>;
		friend class Renderer;
	};
};

#endif
