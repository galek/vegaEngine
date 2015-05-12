#pragma once

#include "..\Common\inc\IEngine.h"

namespace vega
{
	class DynamicsWorld;
	struct SubSystemsManager;
	struct Ragdoll;

	class VBulletPhysics :public iBasePhysics
	{
	public:
		VBulletPhysics( SubSystemsManager*_manager);
		virtual ~VBulletPhysics(){}

		virtual bool initialize();
		/**
		Updates PhysSystem engine
		*/
		virtual bool update(float time);
		virtual void destroy(){};
		virtual void cleanScene(){}
		inline DynamicsWorld*GetWorld(){ return mWorld; }
		inline SubSystemsManager*GetSubS(){ return mManager; };
		IBody* createPrimitiveSphereCollision(BaseActorInterface *_actor);
		IBody* createPrimitiveCapsuleCollision(BaseActorInterface *_actor);
		IBody* createConvex(BaseActorInterface *_actor);
		IBody* createTriangle(BaseActorInterface *_actor);
		IBody* createPrimitiveBoxCollision(BaseActorInterface *_actor);
		IBody* createStaticGeometry(BaseActorInterface *_actor);
		IBody* createRagdoll(BaseActorInterface *_actor);
		
		void addActorToList(Ragdoll* _actor);
		void deleteActor(Ragdoll*_actor);
		void deleteActorFromList(Ragdoll*_actor);
		VVector<Ragdoll*>mRagdolls;
	private:
		virtual IBody* addObject(BaseActorInterface *_actor);
	private:
		DynamicsWorld*mWorld;
		SubSystemsManager*mManager;
	};
}