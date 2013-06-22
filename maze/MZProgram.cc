// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
Program::Program(Renderer *renderer, const std::string& id)
	: mRenderer(renderer),
	  mProgram(0),
	  mID(id),
	  mCached(false),
	  mCacheable(renderer->GetEngine()->GetSetup().CachePrograms),
	  mCacheFile(renderer->GetEngine()->GetSetup().CacheDir + "\\" + id + ".glc")
{
	if (!mCacheable)
	{
		return;
	}

	FILE * fin;
	MGLsizei length;
	MGLenum format;
	void * buffer;

	if (!(fin = fopen(mCacheFile.c_str(), "rb")))
	{
		return;
	}

	fread(&format, sizeof(MGLenum), 1, fin);
	fread(&length, sizeof(MGLsizei), 1, fin);
		
	if (length <= 0 || !(buffer = malloc(length)))
	{
		fclose(fin);
		return;
	}

	if (fread(buffer, 1, length, fin) != length)
	{
		fclose(fin);
		free(buffer);
		return;
	}

	mProgram = mglCreateProgram();
	mglProgramBinary(mProgram, format, buffer, length);
	mCached = true;

	fclose(fin);
	free(buffer);

	GetUniforms();
	GetAttributes();
}

// ------------------------------------------------------------------------------------------------
Program::~Program()
{
	MGLint count = 0;
	std::vector<MGLuint> shaders;

	if (!mProgram)
	{
		return;
	}

	mglGetProgramiv(mProgram, MGL_ATTACHED_SHADERS, &count);

	if (count > 0)
	{
		shaders.resize(count);
		mglGetAttachedShaders(mProgram, count, NULL, &shaders[0]);

		for (size_t i = 0; i < (size_t)count; ++i)
		{
			mglDetachShader(mProgram, shaders[i]);
			mglDeleteShader(shaders[i]);
		}
	}

	mglDeleteProgram(mProgram);
}

// ------------------------------------------------------------------------------------------------
void Program::Compile(Program::ShaderType type, const std::string& fn)
{
	if (mCached)
	{
		return;
	}

	MGLuint shader;
	MGLint status;		
	MGLint length;
	FILE *fin;
	char* buffer;
	std::string log;
	
	if ((fin = fopen(fn.c_str(), "rb")) == NULL)
	{
		throw Exception("Cannot open shader: ") << fn;
	}

	fseek(fin, 0, SEEK_END);
	length = ftell(fin);
	fseek(fin, 0, SEEK_SET);

	buffer = new char[length + 1];
	fread(buffer, 1, length, fin);
	buffer[length] = '\0';
	fclose(fin);

	shader = mglCreateShader(type);
	mglShaderSource(shader, 1, (const MGLchar**)&buffer, NULL);
	delete[] buffer;

	mglCompileShader(shader);
	mglGetShaderiv(shader, MGL_COMPILE_STATUS, &status);
		
	mglGetShaderiv(shader, MGL_INFO_LOG_LENGTH, &length);
	log.resize(length + 1);
	mglGetShaderInfoLog(shader, length, NULL, &log[0]);

	if (status != MGL_TRUE)
	{
		throw Exception("Shader compilation failed '" + mID + "':\n") << log;
	}

	if (!mProgram && !(mProgram = mglCreateProgram()))
	{
		throw MGLException("Cannot create program");
	}

	mglAttachShader(mProgram, shader);
}

// ------------------------------------------------------------------------------------------------
void Program::Link()
{
	if (mCached)
	{
		return;
	}

	MGLint length, status;
	std::string log;

	if (!mProgram)
	{
		throw Exception("No shaders attached to program");
	}

	if (mCacheable)
	{
		mglProgramParameteri(mProgram, MGL_PROGRAM_BINARY_RETRIEVABLE_HINT, MGL_TRUE);
	}

	mglLinkProgram(mProgram);
	mglGetProgramiv(mProgram, MGL_LINK_STATUS, &status);
		
	mglGetProgramiv(mProgram, MGL_INFO_LOG_LENGTH, &length);
	log.resize(length + 1);
	mglGetProgramInfoLog(mProgram, length, NULL, &log[0]);
			
	if (status != MGL_TRUE)
	{
		throw Exception("Shader linking failed '" + mID + "': " + log);
	}

	GetUniforms();
	GetAttributes();
	
	if (mCacheable)
	{
		Cache();
	}
}

// ------------------------------------------------------------------------------------------------
void Program::GetUniforms()
{
	MGLint count = 0, length = 0;

	mglGetProgramiv(mProgram, MGL_ACTIVE_UNIFORMS, &count);
	mglGetProgramiv(mProgram, MGL_ACTIVE_UNIFORM_MAX_LENGTH, &length);
	
	for (MGLint i = 0; i < count; ++i)
	{
		UniformData uniform;

		uniform.Name.resize(length + 1);
		mglGetActiveUniform(
			mProgram, 
			i, 
			length, 
			NULL, 
			&uniform.Size, 
			&uniform.Type,
			&uniform.Name[0]
		); 
		
		while (*uniform.Name.rbegin() == '\0')
		{
			uniform.Name.pop_back();
		}

		uniform.Loc = mglGetUniformLocation(mProgram, &uniform.Name[0]);

		mUniforms.insert(std::make_pair(uniform.Name, uniform));
	}
}

// ------------------------------------------------------------------------------------------------
void Program::GetAttributes()
{
	MGLint count = 0, length = 0, loc = 0, size;
	MGLenum type;
	std::string name;

	mglGetProgramiv(mProgram, MGL_ACTIVE_ATTRIBUTES, &count);
	mglGetProgramiv(mProgram, MGL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &length);
	
	for (MGLint i = 0; i < count; ++i)
	{
		name.clear();
		name.resize(length + 1);

		mglGetActiveAttrib(mProgram, i, length, NULL, &size, &type, &name[0]);
		
		if (name.size() > 1)
		{
			while (*name.rbegin() == '\0')
			{
				name.pop_back();
			}

			loc = mglGetAttribLocation(mProgram, name.c_str());

			mAttributes.insert(std::make_pair(name, loc));
		}
	}
}

// ------------------------------------------------------------------------------------------------
void Program::Cache()
{
	MGLint length = 0;
	MGLsizei read;
	MGLenum format;
	void *binary;
	FILE *fout;

	if (!(fout = fopen(mCacheFile.c_str(), "wb")))
	{
		return;
	}

	mglGetProgramiv(mProgram, MGL_PROGRAM_BINARY_LENGTH, &length);
	if (length <= 0)	
	{
		fclose(fout);
		return;
	}

	if (!(binary = malloc(length)))
	{
		fclose(fout);
		return;
	}

	mglGetProgramBinary(mProgram, length, &read, &format, binary);

	fwrite(&format, sizeof(MGLenum), 1, fout);
	fwrite(&read, sizeof(MGLsizei), 1, fout);
	fwrite(binary, 1, read, fout);

	free(binary);
	fclose(fout);
}

// ------------------------------------------------------------------------------------------------
MGLuint Program::Attribute(const std::string& name)
{
	AttribIter it;

	if ((it = mAttributes.find(name)) == mAttributes.end())
	{
		return -1;
	}

	return it->second;
}

// ------------------------------------------------------------------------------------------------
void Program::Use()
{
	mglUseProgram(mProgram);
}

// ------------------------------------------------------------------------------------------------
void Program::Uniform(const std::string& name, int val)
{	
	UniformIter it;

	if ((it = mUniforms.find(name)) == mUniforms.end())
		return;

	mglUniform1i(it->second.Loc, val);
}

// ------------------------------------------------------------------------------------------------
void Program::Uniform(const std::string& name, float val)
{
	UniformIter it;

	if ((it = mUniforms.find(name)) == mUniforms.end())
		return;

	mglUniform1f(it->second.Loc, val);
}

// ------------------------------------------------------------------------------------------------
void Program::Uniform(const std::string& name, const glm::vec3& val)
{
	UniformIter it;

	if ((it = mUniforms.find(name)) == mUniforms.end())
		return;

	mglUniform3fv(it->second.Loc, 1, glm::value_ptr(val));
}

// ------------------------------------------------------------------------------------------------
void Program::Uniform(const std::string& name, const glm::vec4& val)
{
	UniformIter it;

	if ((it = mUniforms.find(name)) == mUniforms.end())
		return;

	mglUniform4fv(it->second.Loc, 1, glm::value_ptr(val));
}

// ------------------------------------------------------------------------------------------------
void Program::Uniform(const std::string& name, const glm::mat2& val)
{
	UniformIter it;

	if ((it = mUniforms.find(name)) == mUniforms.end())
		return;

	mglUniformMatrix2fv(it->second.Loc, 1, MGL_FALSE, glm::value_ptr(val));
}

// ------------------------------------------------------------------------------------------------
void Program::Uniform(const std::string& name, const glm::mat4& val)
{
	UniformIter it;

	if ((it = mUniforms.find(name)) == mUniforms.end())
		return;

	mglUniformMatrix4fv(it->second.Loc, 1, MGL_FALSE, glm::value_ptr(val));
}

// ------------------------------------------------------------------------------------------------
void Program::Uniform(const std::string& name, MGLenum type, size_t unit, MGLuint texture)
{
	UniformIter it;

	if ((it = mUniforms.find(name)) == mUniforms.end() || !texture)
	{
		return;
	}

	mglActiveTexture(MGL_TEXTURE + unit);
	mglBindTexture(type, texture);
	mglUniform1i(it->second.Loc, unit);
}
// ------------------------------------------------------------------------------------------------
void Program::Uniform(const std::string& name, const float* val, int size)
{
	UniformIter it;

	if ((it = mUniforms.find(name)) == mUniforms.end())
	{
		return;
	}

	mglUniform1fv(it->second.Loc, size, val);
}

// ------------------------------------------------------------------------------------------------
void Program::Uniform(const std::string& name, const glm::mat4* val, int size)
{
	UniformIter it;

	if ((it = mUniforms.find(name)) == mUniforms.end())
	{
		return;
	}

	mglUniformMatrix4fv(it->second.Loc, size, MGL_FALSE, (float*)val);
}