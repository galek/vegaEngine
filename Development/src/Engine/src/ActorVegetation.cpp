/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#include "ActorVegetation.h"

using namespace Ogre;

namespace vega
{

	ActorVegetation::ActorVegetation(const std::string &_fileName, const Ogre::Vector3& _position,
		const Ogre::Radian& _rot, float _scale, const std::string &_material)
	{
		_createMeshVegetation(_fileName, _material, _position, _rot, _scale);
		setCastShadows(true);

	}
	//	!This is for creation Vegetation from OgreMesh
	void ActorVegetation::_createMeshVegetation(const std::string &_fileName, const std::string &_material, const Ogre::Vector3& _position,
		const Ogre::Radian& _rot, float _scale)
	{
		mName = "ActorVegetation_" + _fileName;//for Actor Base


		// Prepare mesh for normalmapping
		pmEntity = Ogre::MeshManager::getSingleton().load(_fileName,
			Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
		unsigned short src, dest;
		if (!pmEntity->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
			pmEntity->buildTangentVectors(VES_TANGENT, src, dest);


		mEntity = GetEngine()->mGSceneMgr->createEntity(mName + "_Ent", _fileName);
		mEntity->setMaterialName(_material);

		_createNode();

		mNode->attachObject(mEntity);
		mNode->setPosition(_position);


		Ogre::Quaternion rot;
		rot.FromAngleAxis(_rot, Ogre::Vector3::UNIT_Y);

		mNode->setOrientation(rot);
		TODO("Possible bug^")
			mNode->scale(_scale, _scale, _scale);
	}

	void ActorVegetation::setCastShadows(bool _status)
	{
		TODO("Deprecated,rewrite")
	}
}