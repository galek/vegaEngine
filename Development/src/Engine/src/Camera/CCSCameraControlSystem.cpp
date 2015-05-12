/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#include "camera\CCSCameraControlSystem.h"

namespace vega
{
	CameraControlSystem::CameraControlSystem(Ogre::SceneManager* sm, Ogre::String name, Ogre::Camera* camera
		, bool recalculateOnTargetMoving)
		: mSceneMgr(sm)
        , mName(name)
        , mTargetNode(NULL)
		, mTargetNodeListener(NULL)
		, mRecalculateOnTargetMoving(recalculateOnTargetMoving)
		, mCurrentCameraMode(NULL)
		
	{
        mCameraNode = mSceneMgr->getRootSceneNode ()->createChildSceneNode (mName+"SceneNode");

        // Create our camera if it wasn't passed as a parameter
	    if (camera == NULL)
		{
		    mCamera = mSceneMgr->createCamera (mName);
		    mOwnCamera = true;
	    }
	    else 
        {
		    mCamera = camera;
		    mOwnCamera = false;
	    }

        // reset to default parameters
        mCamera->setPosition(Ogre::Vector3::ZERO);
        mCamera->setOrientation(Ogre::Quaternion::IDENTITY);

	    // ... and attach the Ogre camera to the camera node
	    mCameraNode->attachObject (mCamera);
	}

	CameraControlSystem::CameraControlSystem(Ogre::SceneManager* sm, Ogre::String name, Ogre::SceneNode* customCameraSceneNode
		, bool recalculateOnTargetMoving)
		: mSceneMgr(sm)
        , mName(name)
        , mTargetNode(NULL)
		, mTargetNodeListener(NULL)
		, mRecalculateOnTargetMoving(recalculateOnTargetMoving)
		, mCurrentCameraMode(NULL)
	{
        mCameraNode = mSceneMgr->getRootSceneNode ()->createChildSceneNode (mName+"SceneNode");
		mOwnCamera = false;	    
	    mCameraNode->addChild(customCameraSceneNode);
	}

	CameraControlSystem::~CameraControlSystem()
	{
		if(mTargetNode && mTargetNode->getListener())
		{
			delete mTargetNode->getListener();
			mTargetNode->setListener(NULL);
		}

		mCameraNode->setAutoTracking(false);
		mCameraNode->removeAllChildren();
	    mCameraNode->detachAllObjects();
	    if (mOwnCamera)
	    	mSceneMgr->destroyCamera(mCamera);
	    mSceneMgr->destroySceneNode (mCameraNode);
	}

	void CameraControlSystem::deleteAllCameraModes()
	{
		std::map<Ogre::String, CameraMode*>::const_iterator it;
        for(it = mCameraModes.begin(); it != mCameraModes.end(); ++it)
        {
            delete (*it).second;
        }
        mCameraModes.clear();
        mCurrentCameraMode = NULL;
	}

	void CameraControlSystem::update(const float& timeSinceLastFrame)
	{
		mTimeSinceLastFrameLastUpdate = timeSinceLastFrame;

		if(mCurrentCameraMode)
        {
            mCurrentCameraMode->update(timeSinceLastFrame);
            mCameraNode->setPosition(mCurrentCameraMode->getCameraPosition());
            mCameraNode->setOrientation(mCurrentCameraMode->getCameraOrientation());
        }
	}

	void CameraControlSystem::setCameraTarget(Ogre::SceneNode* targetNode)
	{		
		if(mTargetNode && mTargetNode->getListener())
		{
			delete mTargetNode->getListener();
			mTargetNode->setListener(NULL);
		}

		if(mCurrentCameraMode)
        {
            mCurrentCameraMode->stop();
        }

        mTargetNode = targetNode;

		if(mCurrentCameraMode)
        {
            mCurrentCameraMode->init();
        }

		if(mRecalculateOnTargetMoving && mTargetNode)
		{
			mTargetNodeListener = new NodeListener(this);
			mTargetNode->setListener(mTargetNodeListener);
		}
	}

    void CameraControlSystem::setCurrentCameraMode(CameraMode* cameraMode)
    {
        if(mCurrentCameraMode)
        {
            mCurrentCameraMode->stop();
        }
        mCurrentCameraMode = cameraMode;
        mCurrentCameraMode->init();
        mCameraNode->setPosition(mCurrentCameraMode->getCameraPosition());
        mCameraNode->setOrientation(mCurrentCameraMode->getCameraOrientation());
    }
	
    CameraControlSystem::CameraMode* CameraControlSystem::getCameraMode(Ogre::String name)
    {
        if(mCameraModes.find(name) != mCameraModes.end())
        {
            return mCameraModes[name]; 
        }
        return NULL;
    }

    Ogre::String CameraControlSystem::getCameraModeName(CameraControlSystem::CameraMode* camMode)
    {
        std::map<Ogre::String, CameraMode*>::const_iterator it;
        for(it = mCameraModes.begin(); it != mCameraModes.end(); ++it)
        {
            if((*it).second == camMode)
            {
                return (*it).first;
            }
        }

        return "";
    }

    CameraControlSystem::CameraMode* CameraControlSystem::getNextCameraMode()
    {
		if(mCameraModes.empty())
		{
			return NULL;
		}

        std::map<Ogre::String, CameraMode*>::const_iterator it;
        for(it = mCameraModes.begin(); it != mCameraModes.end(); ++it)
        {
            if(mCurrentCameraMode == (*it).second)
            {               
                it++;
                if(it != mCameraModes.end())
                {
                    return (*it).second;
                }
                else
                {
                    break;
                }
            }
        }

        it = mCameraModes.begin();
        return (*it).second;
    }

	void CameraControlSystem::setAutoTrackingTarget(bool autoTracking)
    {
        if(mTargetNode)
        {
            mCameraNode->setAutoTracking(autoTracking, mTargetNode);
        }
        else
        {
            mCameraNode->setAutoTracking(autoTracking);
        }
    }

	bool CameraControlSystem::isAutoTrackingTarget()
    {
        if(mTargetNode)
        {
            return (mCameraNode->getAutoTrackTarget() != NULL);
        }
        else
        {
            return false;
        }
    }

	Ogre::Vector3 CameraControlSystem::CollidableCamera::DefaultCollisionDetectionFunction(Ogre::Vector3 cameraTargetPosition, Ogre::Vector3 cameraPosition)
	{
//		funciona con atenea y con barril pero no con el suelo
		Ogre::Vector3 finalCameraPosition (cameraPosition.x, cameraPosition.y, cameraPosition.z);

		Ogre::Vector3 origin = cameraTargetPosition;		
		Ogre::Vector3 direction = (cameraPosition - cameraTargetPosition);
		float maxDistance = direction.length();
		direction = direction.normalisedCopy();
		Ogre::RaySceneQuery *raySceneQuery = mCameraCS2->getSceneManager()->createRayQuery(Ogre::Ray(origin, direction));

		Ogre::RaySceneQueryResult &result = raySceneQuery->execute();
		Ogre::RaySceneQueryResult::iterator itr = result.begin();

		bool intersect = false;
        float minDistance = std::numeric_limits<float>::max();
		while (itr != result.end() && !intersect)//Nick:Расскометил & !intersect
		{
			if(itr->distance < maxDistance
                && itr->distance < minDistance // take the shorter
				&& itr->movable->getParentSceneNode() != mCameraCS2->getCameraSceneNode()
				&& itr->movable->getParentSceneNode() != mCameraCS2->getTargetSceneNode()
				&& mIgnoreList.find(itr->movable) == mIgnoreList.end() )
			{
                minDistance = itr->distance;
				intersect = true;
				if(itr->worldFragment)
				{
					finalCameraPosition = itr->worldFragment->singleIntersection;
				}
				else //if(itr->movable)
				{					
					finalCameraPosition = origin + (direction * (itr->distance - mMargin)); // if margin = 0 the floor in the demo always collides with the camera once it has been touched
				}
			}
			itr++;
		}

		mCameraCS2->getSceneManager()->destroyQuery(raySceneQuery);

		return finalCameraPosition;
	}

	void CameraControlSystem::addToIgnoreList(Ogre::MovableObject* object)
	{
		std::map<Ogre::String, CameraMode*>::const_iterator it;
        for(it = mCameraModes.begin(); it != mCameraModes.end(); ++it)
        {
            if(dynamic_cast<CollidableCamera*>((*it).second))
			{
				dynamic_cast<CollidableCamera*>((*it).second)->addToIgnoreList(object);
			}
        }
	}

	void CameraControlSystem::_setCameraPosition(Ogre::Vector3& position)
	{
		if(mCameraNode)
		{
			mCameraNode->setPosition(position);
		}
	}

	void CameraControlSystem::_setCameraOrientation(Ogre::Quaternion& orientation)
	{
		if(mCameraNode)
		{
			mCameraNode->setOrientation(orientation);
		}
	}
}
