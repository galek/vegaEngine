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
	class ENGINE_API ActorMesh:public Actor
	{
	public:
		// !@Cretion new actor mesh
		ActorMesh(const char *_fileName, const char*_name, const char* _material,
			int _mCollisionModel,float _mass,Ogre::Vector3 _mPosition,
			Ogre::Quaternion _mRotation,
			Ogre::Vector3 _scale,
			Ogre::Vector3 _mLinearVelocity);
		// !@Cretion new actor mesh
		ActorMesh(const char *_fileName, const char*_name);
		// !@Cretion new actor mesh
		ActorMesh(const char *_fileName, const char*_name, const char* _material);
		// !@Destruction Entity
		virtual ~ActorMesh();
		// !@Set Material for Mesh
		void setMaterialName(const char* _material = "DefMat");
		// !@Loading Mesh in background
		void _LoadInBackGround();
		// !@Building Navigation Mesh
		void buildNavMesh();
		void rebuildNavMesh();
		void destroyNavMesh();
		void setUsingNavMesh(bool use);
		__inline bool isUsingNavMesh(){ return mUsingNavMesh; }
	private:
		// !@Creation Mesh
		void _createMesh();
	private:
		Ogre::Mesh* pmEntity;
		bool mUsingNavMesh;
	};
}