#include "VBulletPhysics.h"
#include "VBodys.h"
#include "OgreBulletDynamicsWorld.h"

namespace vega
{
	VBulletPhysics::VBulletPhysics( SubSystemsManager*_manager)
		:mWorld(nullptr), mManager(_manager)
	{
	}
	//-----------------------------------------------------------------------------
	bool VBulletPhysics::initialize(){
		mWorld = new DynamicsWorld(mManager->mGSceneMgr, Ogre::AxisAlignedBox(Ogre::Vector3(-10000.0f, -10000.0f, -10000.0f), Ogre::Vector3(10000.0f, 10000.0f, 10000.0f)), Ogre::Vector3(0.0f, -9.81f, 0.0f), true, true, 10000);
		return true;
	}
	//-----------------------------------------------------------------------------
	bool VBulletPhysics::update(float time){
		mWorld->stepSimulation(time);
		for (int i = 0; i < mRagdolls.GetSize(); i++){
			if (mRagdolls[i])
				mRagdolls[i]->update();
		}
		return true;
	} 
	//-----------------------------------------------------------------------------
	void VBulletPhysics::addActorToList(Ragdoll* _actor)	{
		mRagdolls.AddElement(&_actor);
	}
	//-----------------------------------------------------------------------------
	void VBulletPhysics::deleteActor(Ragdoll*_actor)	{
	}
	//-----------------------------------------------------------------------------
	void VBulletPhysics::deleteActorFromList(Ragdoll*_actor)	{
	}
}