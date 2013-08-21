// This file is part of the MAZE project
// Licensing information can be found in the LICENSE file
// (C) 2012 The MAZE project. All rights reserved.

#include "MZPlatform.h"
using namespace MAZE;


// ------------------------------------------------------------------------------------------------
const Resource::Type Model::TYPE = Resource::MODEL;

// ------------------------------------------------------------------------------------------------
#pragma pack(push, 1)

struct MZOHeader
{
	uint8_t Header[4];

	struct
	{
		uint8_t Major;
		uint8_t Minor;
		uint8_t Rev;
		uint8_t Build;
	} Version;
	
	uint32_t Flags;
	uint16_t VertexCount;
	uint16_t PhysCount;
	uint16_t BoneCount;

	uint16_t Pad[5];
};

#pragma pack(pop)

// ------------------------------------------------------------------------------------------------
Model::Model(ResourceManager* rsmngr, const std::string& id, const std::string& fn)
	: Resource(rsmngr, Resource::MODEL, id),
	  mFile(fn),
	  mMeshVBO(0),
	  mWeightVBO(0)
{
}

// ------------------------------------------------------------------------------------------------
Model::~Model()
{
	if (mMeshVBO)	{ mglDeleteBuffers(1, &mMeshVBO);	mMeshVBO = 0; }
	if (mWeightVBO) { mglDeleteBuffers(1, &mWeightVBO);	mWeightVBO = 0; }
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
		
		mVertices.resize(header.VertexCount);
		mWeights.resize(header.VertexCount);
		mCollision.resize(header.PhysCount);
		mBones.resize(header.BoneCount);
		mCount = mVertices.size();
	
		if (mVertices.size() > 0)
		{
			if (!fs.read((char*)&mVertices[0], mVertices.size() * sizeof(Vertex)))
			{
				throw Exception("Cannot read vertex data: ") << mID;
			}
		}

		if (mWeights.size() > 0)
		{
			if (!fs.read((char*)&mWeights[0], mWeights.size() * sizeof(Weight)))
			{
				throw Exception("Cannot read bone weight data: ") << mID;
			}
		}

		if (mCollision.size() > 0)
		{
			if (!fs.read((char*)&mCollision[0], mCollision.size() * sizeof(glm::vec3)))
			{			
				throw Exception("Cannot read collision data: ") << mID;
			}
		}

		if (mBones.size() > 0)
		{
			for (size_t i = 0; i < mBones.size(); ++i)
			{
				unsigned short length;
				if (!fs.read((char*)&length, sizeof(length)))
				{
					throw Exception("Cannot read bone: ") << mID;
				}

				mBones[i].Name.resize(length + 1);
				mBones[i].Name[length] = '\0';
				if (!fs.read((char*)&mBones[i].Name[0], length) ||
					!fs.read((char*)&mBones[i].Parent, sizeof(uint16_t)) ||					
					!fs.read((char*)&mBones[i].Head, sizeof(float) * 6))
				{
					throw Exception("Cannot read bone: ") << mID;
				}
			}
		}

		mDiffuseMap = mRsmngr->Get<Texture> (mID + "_diffuse");
		mBumpMap = mRsmngr->Get<Texture> (mID + "_bump");
	}

	// Upload the model to the GPU
	if (mVertices.size() > 0)
	{
		mglGenBuffers(1, &mMeshVBO);
		mglBindBuffer(MGL_ARRAY_BUFFER, mMeshVBO);
		mglBufferData(MGL_ARRAY_BUFFER, mVertices.size() * sizeof(mVertices[0]), &mVertices[0], MGL_STATIC_DRAW);

		mVertices.clear();
	}

	if (mWeights.size() > 0)
	{
		mglGenBuffers(1, &mWeightVBO);
		mglBindBuffer(MGL_ARRAY_BUFFER, mWeightVBO);
		mglBufferData(MGL_ARRAY_BUFFER, mWeights.size() * sizeof(mWeights[0]), &mWeights[0], MGL_STATIC_DRAW);

		mWeights.clear();
	}
		
	mglBindBuffer(MGL_ARRAY_BUFFER, 0);
	mglFinish();	
}

// ------------------------------------------------------------------------------------------------
void Model::Unload()
{
	mDiffuseMap.Free();
	mBumpMap.Free();

	mVertices.clear();
	mWeights.clear();
	mCollision.clear();

	if (mMeshVBO)	{ mglDeleteBuffers(1, &mMeshVBO);	mMeshVBO = 0; }
	if (mWeightVBO) { mglDeleteBuffers(1, &mWeightVBO);	mWeightVBO = 0; }
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
	model->mCount = 6;

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
