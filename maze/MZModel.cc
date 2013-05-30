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
	unsigned char Header[4];

	struct
	{
		char Major;
		char Minor;
		char Rev;
		char Build;
	} Version;
	
	unsigned short MeshVertices;
	unsigned short CollisionFaces;

	struct
	{
		float MinX;
		float MinY;
		float MinZ;
		float MaxX;
		float MaxY;
		float MaxZ;
	} Box;
};

#pragma pack(pop)

// ------------------------------------------------------------------------------------------------
Model::Model(ResourceManager* rsmngr, const std::string& id, const std::string& fn)
	: Resource(rsmngr, Resource::MODEL, id),
	  mFile(fn),
	  mVBO(0)
{
}

// ------------------------------------------------------------------------------------------------
Model::~Model()
{
	if (mVBO) { mglDeleteBuffers(1, &mVBO);		 mVBO = 0; }
}

// ------------------------------------------------------------------------------------------------
void Model::Load()
{
	MZOHeader header;

	if (mFile != "")
	{
		std::ifstream fs(mFile.c_str(), std::ios::binary);

		if (!fs.is_open() ||
			!fs.read((char*)&header, sizeof(header)) || 
			memcmp(header.Header, "MZO\0", 4))
		{
			throw Exception("Invalid mzo: '" + mFile + "'");
		}
		
		mBoxWorldMin.x = header.Box.MinX;
		mBoxWorldMin.y = header.Box.MinY;
		mBoxWorldMin.z = header.Box.MinZ;
		
		mBoxWorldMax.x = header.Box.MaxX;
		mBoxWorldMax.y = header.Box.MaxY;
		mBoxWorldMax.z = header.Box.MaxZ;

		mVertices.resize(header.MeshVertices);
		mCollision.resize(header.CollisionFaces);
		mVertexCount = mVertices.size();
	
		if (mVertices.size() > 0)
		{
			if (!fs.read((char*)&mVertices[0], mVertices.size() * sizeof(Vertex)))
			{
				throw Exception("Cannot read vertex data");
			}
		}

		if (mCollision.size() > 0)
		{
			if (!fs.read((char*)&mCollision[0], mCollision.size() * sizeof(glm::vec3)))
			{			
				throw Exception("Cannot read collision data");
			}
		}

		mDiffuseMap = mRsmngr->Get<Texture> (mID + "_diffuse");
		mBumpMap = mRsmngr->Get<Texture> (mID + "_bump");
	}

	// Upload the model to the GPU
	if (mVertices.size() > 0)
	{
		mglGenBuffers(1, &mVBO);
		mglBindBuffer(MGL_ARRAY_BUFFER, mVBO);
		mglBufferData(MGL_ARRAY_BUFFER, mVertices.size() * sizeof(mVertices[0]), &mVertices[0], MGL_STATIC_DRAW);
		mglBindBuffer(MGL_ARRAY_BUFFER, 0);

		mVertices.clear();

		mglBindBuffer(MGL_ARRAY_BUFFER, 0);
		mglFinish();	
	}
}

// ------------------------------------------------------------------------------------------------
void Model::Unload()
{
	mDiffuseMap.Free();
	mBumpMap.Free();

	mVertices.clear();
	mCollision.clear();

	if (mVBO) { mglDeleteBuffers(1, &mVBO);		 mVBO = 0; }
}


// ------------------------------------------------------------------------------------------------
void Model::CreatePlane(ResourceManager* rsmngr, 
						const std::string& id, 
						const std::string& diffuse, 
						const std::string& bump, 
						const glm::vec2& size, 
						const glm::vec2& cellSize)
{
	MGLuint vbo = 0;
	Model* model;
	
	model = new Model(rsmngr, id, "");

	try
	{
		model->mDiffuseMap = rsmngr->Get<Texture> (diffuse);
		model->mBumpMap = rsmngr->Get<Texture> (bump);		
	} 
	catch (std::exception&)
	{
		delete model;
		throw;
	}
		
	float w = size.x / 2.0f, h = size.y / 2.0f;

	model->mVertices.resize(6);
	model->mVertexCount = 6;

	model->mVertices[0].Position = glm::vec3(-w, 0.0f, -h);
	model->mVertices[1].Position = glm::vec3(-w, 0.0f,  h);	
	model->mVertices[2].Position = glm::vec3( w, 0.0f,  h);
	model->mVertices[3].Position = glm::vec3(-w, 0.0f, -h);
	model->mVertices[4].Position = glm::vec3( w, 0.0f,  h);
	model->mVertices[5].Position = glm::vec3( w, 0.0f, -h);

	model->mVertices[0].Normal	 = glm::vec3(0.0f, 1.0f, 0.0f);
	model->mVertices[1].Normal	 = glm::vec3(0.0f, 1.0f, 0.0f);
	model->mVertices[2].Normal	 = glm::vec3(0.0f, 1.0f, 0.0f);
	model->mVertices[3].Normal	 = glm::vec3(0.0f, 1.0f, 0.0f);
	model->mVertices[4].Normal	 = glm::vec3(0.0f, 1.0f, 0.0f);
	model->mVertices[5].Normal	 = glm::vec3(0.0f, 1.0f, 0.0f);

	model->mVertices[0].UV		 = glm::vec2(0.0f,					0.0f				 );
	model->mVertices[1].UV		 = glm::vec2(0.0f,					2.0f * h / cellSize.y);
	model->mVertices[2].UV		 = glm::vec2(2.0f * w / cellSize.x, 2.0f * h / cellSize.y);
	model->mVertices[3].UV		 = glm::vec2(0.0f,					0.0f				 );
	model->mVertices[4].UV		 = glm::vec2(2.0f * w / cellSize.x, 2.0f * h / cellSize.y);
	model->mVertices[5].UV		 = glm::vec2(2.0f * w / cellSize.x, 0.0f				 );
	
	rsmngr->Add(model);
}
