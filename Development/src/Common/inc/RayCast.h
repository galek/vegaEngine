/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

namespace Ogre{
	class RaySceneQuery;
	class RaySceneQuery;
	class SceneManager;
	class RenderWindow;
	class Camera;
	class Vector2;
	class Vector3;
	class MovableObject;
	class Entity;
}

namespace vega {

	class CollisionTools {
	public:
		CollisionTools(struct SubSystemsManager*);
		~CollisionTools();
		bool raycastFromCamera(Ogre::RenderWindow* rw, Ogre::Camera* camera, const Ogre::Vector2 &mousecoords, Ogre::Vector3 &result, Ogre::MovableObject* &target, float &closest_distance, const Ogre::uint32 queryMask = 0xFFFFFFFF);
		bool raycastFromCamera(Ogre::RenderWindow* rw, Ogre::Camera* camera, const Ogre::Vector2 &mousecoords, Ogre::Vector3 &result, Ogre::Entity* &target, float &closest_distance, const Ogre::uint32 queryMask = 0xFFFFFFFF);
		bool collidesWithEntity(const Ogre::Vector3& fromPoint, const Ogre::Vector3& toPoint, const float collisionRadius = 2.5f, const float rayHeightLevel = 0.0f, const Ogre::uint32 queryMask = 0xFFFFFFFF);
		void calculateY(Ogre::SceneNode *n, const bool doTerrainCheck = true, const bool doGridCheck = true, const float gridWidth = 1.0f, const Ogre::uint32 queryMask = 0xFFFFFFFF);
		float getTSMHeightAt(const float x, const float z);
		bool raycastFromPoint(const Ogre::Vector3 &point, const Ogre::Vector3 &normal, Ogre::Vector3 &result, Ogre::MovableObject* &target, float &closest_distance, const Ogre::uint32 queryMask = 0xFFFFFFFF);
		bool raycastFromPoint(const Ogre::Vector3 &point, const Ogre::Vector3 &normal, Ogre::Vector3 &result, Ogre::Entity* &target, float &closest_distance, const Ogre::uint32 queryMask = 0xFFFFFFFF);
		bool raycast(const Ogre::Ray &ray, Ogre::Vector3 &result, Ogre::MovableObject* &target, float &closest_distance, const Ogre::uint32 queryMask = 0xFFFFFFFF);
		bool raycast(const Ogre::Ray &ray, Ogre::Vector3 &result, Ogre::Entity* &target, float &closest_distance, const Ogre::uint32 queryMask = 0xFFFFFFFF);
		void setHeightAdjust(const float heightadjust);
		float getHeightAdjust(void);
	private:
		float _heightAdjust;
		void GetMeshInformation(const Ogre::MeshPtr mesh,
			size_t &vertex_count,
			Ogre::Vector3* &vertices,
			size_t &index_count,
			Ogre::uint32* &indices,
			const Ogre::Vector3 &position,
			const Ogre::Quaternion &orient,
			const Ogre::Vector3 &scale);

		Ogre::RaySceneQuery *mRaySceneQuery;
		Ogre::RaySceneQuery *mTSMRaySceneQuery;
		Ogre::SceneManager *mSceneMgr;
	};

};