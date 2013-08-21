// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef MODEL_H
#define MODEL_H
#pragma once

namespace MAZE
{
	/// Forward declarations
	struct Bone;
	class Skeleton;
	class Animation;

	/// Maximum number of bones which can be present in a skeleton
	const size_t MAX_BONES = 36;
	
	/**
		An animation sequence is composed of keyframes which
		define the bone transforms. The skeleton is interpolated
		between the keyframes
	*/
	class Animation
	{
	public:

		struct Frame
		{
			float Key;

			struct
			{
				glm::vec3 Translation;
				glm::vec3 Rotation;
				glm::vec3 Scaling;
			} Transform[MAX_BONES];
		};

		/**	
			Write bone transforms into the skeleton
		*/
		void Animate(Skeleton * skel, float time);

	private:

		/// List of keyframes
		std::vector<Frame> mFrames;
	};

	/**
		The model class stores information about meshes:
		vertex positions, normal vectors, texture coordinates,
		bone weights and skeleton
	*/
	class Model : public Resource
	{
	public:
		
		/// Template related stuff
		const static Resource::Type TYPE;
		
		/**
			Single vertex of the model
		*/
		struct Vertex
		{
			glm::vec3 Position;
			glm::vec3 Normal;
			glm::vec2 UV;
		};
		
		#pragma pack(push, 1)	
		/**
			Bone weight
		*/
		struct Weight
		{
			unsigned char bi0;
			unsigned char bi1;
			unsigned char bi2;
			unsigned char bi3; 
			float bw0; 
			float bw1; 
			float bw2;
			float bw3; 
		};	

		/**
			Bone data
		*/
		struct Bone
		{
			std::string Name;
			uint16_t Parent;
			glm::vec3 Head;
			glm::vec3 Tail;
		};
		#pragma pack(pop)

		/**
			Vertex index
		*/
		typedef unsigned short Index;
				
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
			Checks if the model has a collision mesh
		*/
		bool HasCollisionMesh() const
		{
			return mCollision.size() > 0;
		}

		/**
			Returns the collision mesh
		*/
		std::vector<glm::vec3> GetCollisionMesh() const
		{
			return mCollision;
		}

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

		/// List of bone weights
		std::vector<Weight> mWeights;
		
		/// List of vertices of the collision mesh
		std::vector<glm::vec3> mCollision;

		/// List of bones
		std::vector<Bone> mBones;

		/// Number of vertices of the mesh
		size_t mCount;
		
		/// Mesh VBO
		MGLuint mMeshVBO;

		/// Weight VBO
		MGLuint mWeightVBO;
		
		/// Diffuse map
		Resource::Ptr<Texture> mDiffuseMap;

		/// Normal map
		Resource::Ptr<Texture> mBumpMap;

		/// Default skeleton
		Skeleton * mSkeleton;

		/// Builtin animations
		std::vector<Animation> mAnimations;
	};
};

#endif
