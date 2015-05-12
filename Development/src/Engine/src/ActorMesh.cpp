/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
using namespace Ogre;


namespace vega
{

	ActorMesh::ActorMesh(const char *_fileName, const char*_name, const char* material, int _mCollisionModel,
		float _mass, Ogre::Vector3 _mPosition, Ogre::Quaternion _mRotation, Ogre::Vector3 _scale,
		Ogre::Vector3 _mLinearVelocity)
		:pmEntity(nullptr)
	{
		mName = _name;//for Actor Base
		mNode = nullptr;//for Actor Base
		mEntity = nullptr;//for Actor Base
		mFileName = _fileName;
		mPosition = _mPosition;
		mRotation = _mRotation;
		mScale = _scale;
		mMaterialName = material;
		mLinearVelocity = _mLinearVelocity;
		mMass = _mass;
		mCollisionModel = CollisionModel(_mCollisionModel);
		_createNode(mNode, mPosition, mRotation, mScale);
		_createMesh();
		setCastShadows(true);
		setMaterialName(material);
	}
	//-------------------------------------------------------------------------------------
	ActorMesh::~ActorMesh()
	{
	}

	//-------------------------------------------------------------------------------------
	void ActorMesh::_createMesh()
	{
		// Загрузжаем файл меша
		_LoadInBackGround();
		// Создаем временную entity
		mEntity = GetEngine()->mGSceneMgr->createEntity(mName + "_Ent", mFileName);
		mNode->attachObject(mEntity);
		// Считаем вектор размера,по идее размер должен быть настоящим,т.е 
		calculateSizeUsingAxisAlignedBox();
		//Создаем Физику
		_switchCollision();
	}
	//-------------------------------------------------------------------------------------
	void ActorMesh::setMaterialName(const char* material)
	{
		mMaterialName = material;
		if (!(mMaterialName == "AutoDetect"))
			mEntity->setMaterialName(mMaterialName);
	}
	//-------------------------------------------------------------------------------------
	ActorMesh::ActorMesh(const char *_fileName, const char*_name)
		:ActorMesh(_fileName, _name, "DefMat",
		ACTORMESH_COLLISION_STATIC, 10.0f, Ogre::Vector3(0, 0, 0), Ogre::Quaternion(1, 0, 0, 0),
		Ogre::Vector3(1, 1, 1), Ogre::Vector3(0, 0, 0))
	{}
	//-------------------------------------------------------------------------------------
	ActorMesh::ActorMesh(const char *_fileName, const char*_name, const char* _material)
		: ActorMesh(_fileName, _name, _material,
		ACTORMESH_COLLISION_STATIC, 10.0f, Ogre::Vector3(0, 0, 0), Ogre::Quaternion(1, 0, 0, 0),
		Ogre::Vector3(1, 1, 1), Ogre::Vector3(0, 0, 0))
	{}
	//-------------------------------------------------------------------------------------
	void ActorMesh::_LoadInBackGround(){
		//Background loading
		// Загрузжаем файл меша
		bool exist = Ogre::MeshManager::getSingleton().resourceExists(mFileName);
		if (!exist)
		{
			int ticket = Ogre::ResourceBackgroundQueue::getSingleton().load(Ogre::MeshManager::getSingleton().getResourceType(), mFileName, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);

			while (!(ResourceBackgroundQueue::getSingleton().isProcessComplete(ticket)))
			{
				OGRE_THREAD_SLEEP(0);
				Ogre::Root::getSingleton().getWorkQueue()->processResponses();
			}
		}
		pmEntity = Ogre::MeshManager::getSingleton().getByName(mFileName).getPointer();
		pmEntity->setBackgroundLoaded(true);
	}
	//-------------------------------------------------------------------------------------
	void ActorMesh::setUsingNavMesh(bool use){
		mUsingNavMesh = use;
	}
	//-------------------------------------------------------------------------------------
	void ActorMesh::buildNavMesh(){
		std::vector<Ogre::Entity*> mNavmeshEnts;
		mNavmeshEnts.push_back(mEntity);
		bool successfully = false;

		if (mUsingNavMesh)
			successfully = GetEngine()->GetAI()->NavMeshBuild(mNavmeshEnts);
		if (successfully)
			GetEngine()->GetAI()->drawNavMesh();
		else
			SeriousWarning(false,"ERROR: could not generate useable navmesh from mesh.");
	}
	//-------------------------------------------------------------------------------------
	void ActorMesh::rebuildNavMesh(){
		destroyNavMesh();
		buildNavMesh();
	}
	//-------------------------------------------------------------------------------------
	void ActorMesh::destroyNavMesh(){
		if (mUsingNavMesh)
			GetEngine()->GetAI()->RecastCleanup();
	}
}