/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

namespace vega
{
	class ENGINE_API ActorVegetation :public Actor
	{
	public:
		ActorVegetation(const std::string &_fileName,const Ogre::Vector3& _position=Ogre::Vector3(0,0,0), 
			const Ogre::Radian& _rot = Ogre::Radian(0), float _scale = 1.0f,const std::string &_material="DefVegetationMat");
		void setCastShadows(bool _status);
		virtual ~ActorVegetation(){}
	private:
		//	!This is for creation Vegetation from OgreMesh
		void _createMeshVegetation(const std::string &_fileName,const std::string &_material,const Ogre::Vector3& _position,
			const Ogre::Radian& _rot, float _scale);
	private:
		Ogre::MeshPtr pmEntity;
		bool collisionStatus;
	};
}