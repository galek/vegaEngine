/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"

#include "camera\CCSFreeCameraMode.h"
namespace vega{
	bool FreeCameraMode::init()
	{
		CameraMode::init();

		mCameraCS->setFixedYawAxis(true, mFixedAxis);

		mLongitudinalDisplacement = 0;
		mLateralDisplacement = 0;
		mVerticalDisplacement = 0;

		if (mSwitchingMode == CURRENT_STATE)
		{
			mCameraPosition = mCameraCS->getOgreCamera()->getRealPosition();

			Ogre::Vector3 cameraDirection = mCameraCS->getOgreCamera()->getRealDirection();
			if (mCameraCS->isAutoTrackingTarget())
			{
				cameraDirection = mCameraCS->getCameraTargetPosition() - mCameraPosition;
			}
			cameraDirection.normalise();

			Ogre::Vector2 yawandpitch = getYawAndPitch(cameraDirection);
			mRotX = yawandpitch.x - Ogre::Math::HALF_PI;
			mRotY = yawandpitch.y;
		}
		else if (mSwitchingMode == LAST_STATE)
		{
			mRotX = mLastRotationX;
			mRotY = mLastRotationY;
			mCameraPosition = mLastPosition;
		}
		else
		{
			reset();
		}

		mCameraCS->setAutoTrackingTarget(false);

		instantUpdate();

		return true;
	}

	void FreeCameraMode::update(const float &timeSinceLastFrame)
	{
		Ogre::Vector3 dirVector = mCameraCS->getOgreCamera()->getRealDirection();
		Ogre::Vector3 lateralVector = dirVector.crossProduct(mFixedAxis).normalisedCopy();
		Ogre::Vector3 upVector = -dirVector.crossProduct(lateralVector).normalisedCopy();

		Ogre::Vector3 displacement = ((dirVector * mLongitudinalDisplacement)
			+ (upVector * mVerticalDisplacement)
			+ (lateralVector * mLateralDisplacement)) * timeSinceLastFrame * mMoveFactor;

		if (mCollisionsEnabled)
		{
			mCameraPosition = collisionDelegate(mCameraPosition, mCameraPosition + displacement);
		}
		else
		{
			mCameraPosition += displacement;
		}

		Ogre::Quaternion offsetX(mRotY, Ogre::Vector3::UNIT_X);
		Ogre::Quaternion offsetY(mRotX, mFixedAxis);

		mCameraOrientation = offsetY * offsetX;

		mLongitudinalDisplacement = 0;
		mLateralDisplacement = 0;
		mVerticalDisplacement = 0;

		mLastRotationX = mRotX;
		mLastRotationY = mRotY;
		mLastPosition = mCameraPosition;
	}

	void FreeCameraMode::instantUpdate()
	{
		Ogre::Quaternion offsetX(mRotY, Ogre::Vector3::UNIT_X);
		Ogre::Quaternion offsetY(mRotX, mFixedAxis);

		mCameraOrientation = offsetY * offsetX;

		mLongitudinalDisplacement = 0;
		mLateralDisplacement = 0;
		mVerticalDisplacement = 0;

		mLastRotationX = mRotX;
		mLastRotationY = mRotY;
		mLastPosition = mCameraPosition;
	}

	void FreeCameraMode::reset()
	{
		mCameraPosition = mInitialPosition;
		mRotX = mInitialRotationX;
		mRotY = mInitialRotationY;
	}

	Ogre::Vector2 FreeCameraMode::getYawAndPitch(const Ogre::Vector3& direction)
	{
		// Adapted from http://www.ogre3d.org/forums/viewtopic.php?f=2&t=27086

		Ogre::Vector2 yawandpitch(0, 0); // yawandpitch.x = yaw, yawandpitch.y = pitch

		if (direction.x == 0.0f && direction.z == 0.0f)
		{
			yawandpitch.x = 0.0f;

			if (direction.y > 0.0f)
			{
				yawandpitch.y = 1.5f * Ogre::Math::PI;
			}
			else
			{
				yawandpitch.y = Ogre::Math::HALF_PI;
			}
		}
		else
		{
			yawandpitch.x = Ogre::Math::ATan2(-direction.z, direction.x).valueRadians();

			if (yawandpitch.x < 0.0f)
			{
				yawandpitch.x += 2.0f * Ogre::Math::PI;
			}

			float forward = Ogre::Math::Sqrt(direction.x * direction.x + direction.z * direction.z);

			yawandpitch.y = Ogre::Math::ATan2(direction.y, forward).valueRadians();

			if (yawandpitch.y < 0.0f)
			{
				yawandpitch.y += 2.0f * Ogre::Math::PI;
			}
		}

		return yawandpitch;
	}

	Ogre::Vector3 FreeCameraMode::AboveGroundCollisionDetectionFunction(Ogre::Vector3 cameraTargetPosition, Ogre::Vector3 cameraPosition)
	{
		Ogre::Vector3 origin = cameraPosition + (mFixedAxis * 100000);
		Ogre::Vector3 direction = -mFixedAxis;

		Ogre::Vector3 projectedCameraPosition = getFirstRealHit(origin, direction);
		projectedCameraPosition.y += mMargin;
		if (projectedCameraPosition.y < cameraPosition.y)
		{
			return cameraPosition;
		}
		else
		{
			return projectedCameraPosition;
		}
	}

	Ogre::Vector3 FreeCameraMode::ConstantHeightCollisionDetectionFunction(Ogre::Vector3 cameraTargetPosition, Ogre::Vector3 cameraPosition)
	{
		Ogre::Vector3 origin = cameraPosition + (mFixedAxis * 100000);
		Ogre::Vector3 direction = -mFixedAxis;

		Ogre::Vector3 projectedCameraPosition = getFirstRealHit(origin, direction);

		return projectedCameraPosition + (mFixedAxis * mMargin);
	}

	Ogre::Vector3 FreeCameraMode::getFirstRealHit(Ogre::Vector3 origin, Ogre::Vector3 direction)
	{
		Ogre::Vector3 hit = origin;
		float minDistance = std::numeric_limits<float>::max();

		Ogre::RaySceneQuery *raySceneQuery = mCameraCS2->getSceneManager()->createRayQuery(Ogre::Ray(origin, direction));

		Ogre::RaySceneQueryResult &result = raySceneQuery->execute();
		Ogre::RaySceneQueryResult::iterator itr = result.begin();

		bool intersect = false;
		while (itr != result.end() /*&& !intersect*/) //! @todo are the results ordered ?? if so, uncomment (optimization)
		{
			if (itr->distance < minDistance // take the shorter
				&& itr->movable->getParentSceneNode() != mCameraCS2->getCameraSceneNode()
				&& itr->movable->getParentSceneNode() != mCameraCS2->getTargetSceneNode()
				&& mIgnoreList.find(itr->movable) == mIgnoreList.end())
			{
				minDistance = itr->distance;
				intersect = true;
				if (itr->worldFragment)
				{
					hit = itr->worldFragment->singleIntersection;
				}
				else //if(itr->movable)
				{
					hit = origin + (direction * itr->distance);
				}
			}
			itr++;
		}

		mCameraCS2->getSceneManager()->destroyQuery(raySceneQuery);

		return hit;
	}
}