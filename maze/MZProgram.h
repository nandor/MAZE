// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#ifndef SHADER_H
#define SHADER_H
#pragma once

#include "MZPlatform.h"
#include "MZRsmngr.h"
#include "MZMath.h"
#include "MZGL.h"

namespace MAZE
{
	class Program
	{
	public:

		/**
			List of shader types
		*/
		enum ShaderType
		{
			VERTEX		 = MGL_VERTEX_SHADER,
			FRAGMENT	 = MGL_FRAGMENT_SHADER,
			GEOMETRY	 = 0,
			TESS_CONTROL = 0,
			TESS_EVAL	 = 0
		};

	public:

		/**
			Creates a new shader
		*/
		Program(const std::string& id);

		/**
			Destroys the shader
		*/
		~Program();

		/**
			Compiles the shader & attaches to program
			@param fn Path to the source file
		*/
		void Compile(ShaderType type, const std::string& fn);

		/**
			Links the shader & retrieves uniforms
		*/
		void Link();

		/**
			Bind the program to the current context
		*/
		void Use();

		void Uniform(const std::string& name, int val);
		void Uniform(const std::string& name, float val);
		void Uniform(const std::string& name, const glm::vec3& val);
		void Uniform(const std::string& name, const glm::vec4& val);
		void Uniform(const std::string& name, const glm::mat2& val);
		void Uniform(const std::string& name, const glm::mat4& val);
		void Uniform(const std::string& name, MGLenum type, size_t unit, MGLuint texture);

		void Uniform(const std::string& name, const float* val, int size);
		void Uniform(const std::string& name, const glm::mat4* val, int size);

	private:
		
		struct UniformData
		{
			/// Name of the uniform variable
			std::string Name;

			/// Type of the uniform variable
			MGLenum Type;

			/// Size (if it's an array)
			MGLint Size;

			/// Location of the variable
			MGLint Loc;
		};

		typedef std::map<std::string, UniformData>::iterator UniformIter;

	private:

		/// ID of the program
		MGLuint mProgram;

		/// Name of the program
		std::string mID;

		/// List of active uniforms
		std::map<std::string, UniformData> mUniforms;

	};
};

#endif