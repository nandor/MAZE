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
	struct CameraRenderData
	{
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
	};

	/**
		Object render data
	*/
	struct ObjectRenderData
	{
		/// Local matrix of the object
		glm::mat4 ModelMatrix;

		/// Inverse model matrix
		glm::mat4 NormalMatrix;

		/// Texture matrix
		glm::mat2 TextureMatrix;

		/// Reference to the model
		Resource::Ptr<Model> Model;
	};

	/**
		Ligth render data
	*/
	struct LightRenderData
	{
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

        /// Ground texture
        Resource::Ptr<Texture> GroundTexture;

        /// Ready state
        bool Ready;
    };         
};

#endif
