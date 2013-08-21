// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef RENDERBUFFER_H
#define RENDERBUFFER_H
#pragma once

namespace MAZE
{
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
		
		/// Local matrix of the object
		glm::mat4 Skin[MAX_BONES];

		/// Reference to the model
		Resource::Ptr<Model> model;

		/// Do not instance, use skinning instead
		bool Skinned;

		/// ID of the entity
		unsigned Handle;

		/// Depth of the object
		float Z;
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

		/// Number of shadow levels
		int ShadowLevels;
		
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
		2D Widget render data
	*/
	class WidgetRenderData
	{
	public:

		/// Texture of the widget
		Resource::Ptr<Texture> texture;
		
		/// Position of the widget
		glm::vec2 Position;

		/// Size of the widget
		glm::vec2 Size;
		
		/// Layer
		float Z;
	};

	/**
		2D Font render data
	*/
	class TextRenderData
	{
	public:

		/// Font used
		Resource::Ptr<Font> font;

		/// Text
		std::string Text;

		/// Position
		glm::vec2 Position;
		
		/// Layer
		float Z;

	};
		
    /**
        Buffer which stores render data
    */
    class RenderBuffer : public Aligned
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
			Creates a new widget
		*/
		WidgetRenderData* AddWidget();

		/**
			Creates a new object
		*/
		ObjectRenderData* AddObject();

		/**
			Creates a new text widget
		*/
		TextRenderData* AddText();

		/**
			Sorts objects front to back
		*/
		void Sort(const glm::mat4 view);

    public:

		/// Camera		
		struct
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

			/// Focal distance
			float Focus;

		} Camera;

		/// Fog
		struct
		{
			/// Color of the fog
			glm::vec3 Color;

			/// Density of the fog (0.03 is okay)
			float Density;

			/// Height of the fog (30.0)
			float Height;

		} Fog;
               
        /// Active lights
        std::vector<LightRenderData> Lights;

		/// Active object
		std::vector<ObjectRenderData> Objects;
        
		/// List of shadow casters
		std::vector<ObjectRenderData> ShadowCasters;

		/// List of UI widgets
		std::vector<WidgetRenderData> Widgets;

		/// List of UI text widgets
		std::vector<TextRenderData> Text;
				        
        /// Texture of the skybox
        Resource::Ptr<Texture> SkyTexture;
		
		/// View volume
		Frustum ViewVolume;
    };         
};

#endif
