// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H
#pragma once

#include "MZPlatform.h"
#include "MZTexture.h"
#include "MZModel.h"
#include "MZBound.h"

namespace MAZE
{
	/**
		Camera render data
	*/
	class CameraRenderData
	{
	public:

		/// Projection matrix
		glm::mat4 ProjMatrix;

		/// View matrix
		glm::mat4 ViewMatrix;

		/// Sky view matrix
		glm::mat4 SkyMatrix;

		/// Camera position
		glm::vec3 Position;

		/// Camera direction
		glm::vec3 Direction;

		/// Near plane
		float NearPlane;

		/// Far plane
		float FarPlane;

		/// Focal distance
		float Focus;
	};

	/**
		Object render data
	*/
	class ObjectRenderData
	{
	public:

		/// Local matrix of the object
		glm::mat4 ModelMatrix;

		/// Texture matrix
		glm::mat2 TextureMatrix;

		/// Position of the object
		glm::vec3 Position;

		/// Reference to the model
		Resource::Ptr<Model> Model;

		/// Depth of the object
		float Z;
		
		/// ID of the entity
		unsigned Handle;
	};

	/**
		Ligth render data
	*/
	class LightRenderData
	{
	public:

		/// Type of the light
		size_t Type;

		/// Position of the light
		glm::vec4 Position;

		/// Direction of the light
		glm::vec4 Direction;

		/// Ambient color
		glm::vec3 Ambient;

		/// Light color
		glm::vec3 Diffuse;
			
		/// Specular color
		glm::vec3 Specular;
				
		/// Volumse model matrix
		glm::mat4 ModelMatrix;

		/// Inverse model matrix
		glm::mat4 NormalMatrix;

		/// True if the light casts shadows
		bool CastsShadows;

		/// Intersection result
		bool Inside;

		/// List of shadow maps
		struct
		{
			/// Position in the shadow caster array
			size_t Index;

			/// Number of shadow caster
			size_t Count;

			/// Projection matrix
			glm::mat4 MVP;

			/// Distance
			float NearZ;

		} Shadow[4];
	};	
	

    /**
        Buffer which stores render data
    */
    class RenderBuffer
    {	
    public:

        /**
            Creates a new buffer
        */
		RenderBuffer();

        /**
            Destroys the buffer
        */
        ~RenderBuffer();

        /**
            Resets the buffer
        */
        void Clear();

		/**
			Creates a new light
		*/
		LightRenderData* AddLight();

		/**
			Sorts objects front to back
		*/
		void Sort(const glm::mat4 view);

    public:

		/// Camera
		CameraRenderData Camera;
               
        /// Active lights
        std::vector<LightRenderData> Lights;

		/// Active object
		std::vector<ObjectRenderData> Objects;
        
		/// List of shadow casters
		std::vector<ObjectRenderData> ShadowCasters;

        /// Size of the world
        glm::vec3 WorldSize;
		        
        /// Texture of the skybox
        Resource::Ptr<Texture> SkyTexture;

        /// Ready state
        bool Ready;

		/// View volume
		ViewFrustum ViewVolume;
    };         
};

#endif
