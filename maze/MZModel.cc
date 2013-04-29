// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include <fstream>
#include "MZPlatform.h"
#include "MZException.h"
#include "MZRenderer.h"
#include "MZRsmngr.h"
#include "MZEngine.h"
#include "MZModel.h"
using namespace MAZE;

// ------------------------------------------------------------------------------------------------
#pragma pack(push, 1)

struct MZOHeader
{
	char Header[4];

	struct
	{
		char Major;
		char Minor;
		char Rev;
		char Build;
	} Version;
	
	unsigned short GroupCount;
	unsigned short PolyCount;
	unsigned short VertexCount;
	unsigned short Pad;

	struct
	{
		float SphereRadius;
		float BoxX;
		float BoxY;
		float BoxZ;
		float BoxWidth;
		float BoxHeight;
		float BoxDepth;
	} Collision;
};

struct MZOMesh
{
	unsigned VertexCount;

	struct
	{
		float R;
		float G;
		float B;
	} Diffuse;

	float Specular;
	char Texture[16];
};

#pragma pack(pop)

// ------------------------------------------------------------------------------------------------
Model::Model(ResourceManager* rsmngr, const std::string& id, const std::string& fn)
	: Resource(rsmngr, Resource::MODEL, id),
	  mFile(fn),
	  mVBO(0),
	  mFromFile(true)
{
}

// ------------------------------------------------------------------------------------------------
Model::~Model()
{
	if (mVBO) 
	{ 
		mglDeleteBuffers(1, &mVBO); 
		mVBO = 0; 
	}
}

// ------------------------------------------------------------------------------------------------
void Model::Load()
{
	MZOHeader header;
	MZOMesh mesh;

	if (mFromFile)
	{
		std::ifstream fs(mFile.c_str(), std::ios::binary);

		if (!fs.is_open())
		{
			throw Exception("Cannot open file: '" + mFile + "'");
		}

		if (!fs.read((char*)&header, sizeof(header)) ||
			memcmp(header.Header, "MZO\0", 4))
		{
			throw Exception("Invalid header: '" + mFile + "'");
		}

		mMeshes.resize(header.GroupCount);	
		for (size_t i = 0, off = 0; i < header.GroupCount; ++i)
		{
			if (!fs.read((char*)&mesh, sizeof(mesh)))
				throw Exception("Cannot read mesh #") << i << ": '" + mFile + "'";

			mMeshes[i].Diffuse = glm::vec3(mesh.Diffuse.R, mesh.Diffuse.G, mesh.Diffuse.B);
			mMeshes[i].VertexOffset = off;
			mMeshes[i].VertexCount = mesh.VertexCount;

			if (mesh.Texture[0] != '\0')
				mMeshes[i].DiffuseMap = mRsmngr->Get<Texture>(mesh.Texture);

			off += mesh.VertexCount;
		}
	
		mVertexCount = header.VertexCount;
		mVertices.resize(header.VertexCount);
		if (!fs.read((char*)&mVertices[0], header.VertexCount * sizeof(Vertex)))
			throw Exception("Cannot read vertex data");
		
		size_t bufferSize = mVertexCount * sizeof(Vertex);
	}

	// Upload the model to the GPU
	if (mVertices.size() > 0)
	{
		mglGenBuffers(1, &mVBO);

		if (!mVBO)
		{
			throw Exception("Cannot create VBO");
		}

		mglBindBuffer(MGL_ARRAY_BUFFER, mVBO);
		mglBufferData(MGL_ARRAY_BUFFER, mVertices.size() * sizeof(Vertex), &mVertices[0], MGL_STATIC_DRAW);
		mglBindBuffer(MGL_ARRAY_BUFFER, 0);

		mglFinish();	
	}
}

// ------------------------------------------------------------------------------------------------
void Model::Unload()
{
	mMeshes.clear();
	mVertices.clear();

	if (mVBO)
	{
		mglDeleteBuffers(1, &mVBO);
		mVBO = 0;
	}
}


// ------------------------------------------------------------------------------------------------
void Model::CreatePlane(ResourceManager* rsmngr, const std::string& id, float w, float h, float cw, float ch)
{
	MGLuint vbo = 0;
	Model* model;
	
	
	model = new Model(rsmngr, id, "");

	w = w / 2.0f;
	h = h / 2.0f;

	model->mVertices.resize(6);
	model->mVertices[0].Position = glm::vec3(-w, 0.0f, -h);
	model->mVertices[0].Normal	 = glm::vec3(0.0f, 1.0f, 0.0f);
	model->mVertices[0].UV		 = glm::vec2(0.0f, 0.0f);
	model->mVertices[1].Position = glm::vec3(-w, 0.0f, h);	
	model->mVertices[1].Normal	 = glm::vec3(0.0f, 1.0f, 0.0f);
	model->mVertices[1].UV		 = glm::vec2(0.0f, 2.0f * h / ch);
	model->mVertices[2].Position = glm::vec3(w, 0.0f, h);
	model->mVertices[2].Normal	 = glm::vec3(0.0f, 1.0f, 0.0f);
	model->mVertices[2].UV		 = glm::vec2(2.0f * w / cw, 2.0f * h / ch);
	model->mVertices[3].Position = glm::vec3(-w, 0.0f, -h);
	model->mVertices[3].Normal	 = glm::vec3(0.0f, 1.0f, 0.0f);
	model->mVertices[3].UV		 = glm::vec2(0.0f, 0.0f);
	model->mVertices[4].Position = glm::vec3(w, 0.0f, h);
	model->mVertices[4].Normal	 = glm::vec3(0.0f, 1.0f, 0.0f);
	model->mVertices[4].UV		 = glm::vec2(2.0f * w / cw, 2.0f * h / ch);
	model->mVertices[5].Position = glm::vec3(w, 0.0f, -h);
	model->mVertices[5].Normal	 = glm::vec3(0.0f, 1.0f, 0.0f);
	model->mVertices[5].UV		 = glm::vec2(2.0f * w / cw, 0.0f);
	model->mVertexCount = 6;

	model->mMeshes.resize(1);
	model->mMeshes[0].Diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	model->mMeshes[0].DiffuseMap = rsmngr->Get<Texture> ("grass");
	model->mMeshes[0].VertexOffset = 0;
	model->mMeshes[0].VertexCount = 6;

	model->mFromFile = false;
	
	rsmngr->Add(model);
}
