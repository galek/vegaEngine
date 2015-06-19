/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#ifndef _BasicCameraModes_H_
#define _BasicCameraModes_H_


#include "CCSCameraControlSystem.h"

namespace vega
{
	/**
	 * In this mode the camera position and orientation never change.
	 */
	class FixedCameraMode : public CameraControlSystem::CameraMode
	{
	public:

		FixedCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y)
			: CameraControlSystem::CameraMode(cam)
			, mFixedAxis(fixedAxis)
			, mLastPosition(Ogre::Vector3::ZERO)
			, mLastOrientation(Ogre::Quaternion::IDENTITY)

		{ };

		virtual ~FixedCameraMode(){};

		virtual bool init()
		{
			CameraMode::init();

			mCameraCS->setFixedYawAxis(true, mFixedAxis);
			mCameraCS->setAutoTrackingTarget(false);

			instantUpdate();

			return true;
		}

		virtual void update(const float &timeSinceLastFrame){};
		virtual void instantUpdate()
		{
			mCameraPosition = mLastPosition;
			mCameraOrientation = mLastOrientation;
		};

		// Specific methods

		EFORCEINLINE virtual void setCameraPosition(Ogre::Vector3 pos)
		{
			mLastPosition = pos;
			mCameraPosition = pos;
		}

		EFORCEINLINE virtual void setCameraOrientation(Ogre::Quaternion orient)
		{
			mLastOrientation = orient;
			mCameraOrientation = mLastOrientation;
		}

		EFORCEINLINE virtual void setCameraOrientation(const Ogre::Radian roll,
			const Ogre::Radian yaw, const Ogre::Radian pitch)
		{
			mLastOrientation = Ogre::Quaternion(roll, Ogre::Vector3::UNIT_Z)
				* Ogre::Quaternion(yaw, Ogre::Vector3::UNIT_Y)
				* Ogre::Quaternion(pitch, Ogre::Vector3::UNIT_X);

			mCameraOrientation = mLastOrientation;
		}

	protected:
		Ogre::Vector3 mFixedAxis;
		Ogre::Vector3 mLastPosition;
		Ogre::Quaternion mLastOrientation;
	};

	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * In this mode the camera position is fixed and the camera always
	 * points to the target.
	 */
	class FixedTrackingCameraMode : public FixedCameraMode
	{
	public:

		FixedTrackingCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y)
			: FixedCameraMode(cam, fixedAxis)
		{ };

		virtual ~FixedTrackingCameraMode(){};

		virtual bool init()
		{
			CameraMode::init();

			mCameraCS->setFixedYawAxis(true, mFixedAxis);
			mCameraCS->setAutoTrackingTarget(true);

			instantUpdate();

			return true;
		}

	};

	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * In this mode the camera follows the target. The second parameter is the relative position
	 * to the target. The orientation of the camera is fixed by a yaw axis (UNIT_Y by default).
	 */
	class ChaseCameraMode : public CameraControlSystem::CameraModeWithTightness, public CameraControlSystem::CollidableCamera
	{
	public:

		/**
		 * @param fixedStep true to try to avoid camera jittering (experimental)
		 */
		ChaseCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &relativePositionToCameraTarget
			, float margin = 0.1f
			, bool fixedStep = false, float delta = 0.001
			, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y)
			: CameraControlSystem::CameraModeWithTightness(cam)
			, CameraControlSystem::CollidableCamera(cam, margin)
			, mFixedAxis(fixedAxis)
			, mRelativePositionToCameraTarget(relativePositionToCameraTarget)
			, mFixedStep(fixedStep)
			, mDelta(delta)
		{
				mTightness = 0.01;
				mRemainingTime = 0;
		};

		virtual ~ChaseCameraMode(){};

		virtual bool init()
		{
			CameraMode::init();

			mDelta = 0;
			mRemainingTime = 0;

			setFixedYawAxis(true, mFixedAxis);
			mCameraCS->setAutoTrackingTarget(true);

			this->collisionDelegate = newCollisionDelegate(this
				, &CollidableCamera::DefaultCollisionDetectionFunction);

			instantUpdate();

			return true;
		}

		virtual void update(const float &timeSinceLastFrame)
		{
			if (mCameraCS->getTargetSceneNode())
			{
				Ogre::Vector3 cameraCurrentPosition = mCameraCS->getCameraPosition();
				Ogre::Vector3 cameraFinalPositionIfNoTightness = mCameraCS->getCameraTargetPosition()
					+ mCameraCS->getCameraTargetOrientation() * mRelativePositionToCameraTarget;

				if (!mFixedStep)
				{
					Ogre::Vector3 diff = (cameraFinalPositionIfNoTightness - cameraCurrentPosition) * mTightness;
					mCameraPosition += diff;
					//! @todo mCameraPosition += diff * timeSinceLastFrame; this makes the camera mode time independent but it also make impossible to get a completely rigid link (tightness = 1)
				}
				else
				{
					mRemainingTime += timeSinceLastFrame;
					int steps = (int)(mRemainingTime / mDelta);
					float mFinalTime = steps * mDelta;
					Ogre::Quaternion cameraCurrentOrientation = mCameraCS->getCameraOrientation();
					float finalPercentage = mFinalTime / mRemainingTime;
					Ogre::Vector3 cameraFinalPosition = cameraCurrentPosition + ((cameraFinalPositionIfNoTightness - cameraCurrentPosition) * finalPercentage);
					Ogre::Quaternion cameraFinalOrientation = Ogre::Quaternion::Slerp(finalPercentage, cameraCurrentOrientation
						, mCameraCS->getCameraTargetOrientation());

					Ogre::Vector3 cameraIntermediatePosition = cameraCurrentPosition;
					Ogre::Quaternion cameraIntermediateOrientation = cameraCurrentOrientation;
					for (int i = 0; i < steps; i++)
					{
						float percentage = ((i + 1) / (float)steps);

						Ogre::Vector3 intermediatePositionIfNoTightness = cameraCurrentPosition + ((cameraFinalPositionIfNoTightness - cameraCurrentPosition) * percentage);

						Ogre::Vector3 diff = (intermediatePositionIfNoTightness - cameraCurrentPosition) * mTightness;
						mCameraPosition += diff;
					}
				}

				if (mCollisionsEnabled)
				{
					mCameraPosition = collisionDelegate(mCameraCS->getCameraTargetPosition(), mCameraPosition);
				}
			}
		}

		virtual void instantUpdate()
		{
			if (mCameraCS->getTargetSceneNode())
			{
				mCameraPosition = mCameraCS->getCameraTargetPosition()
					+ mCameraCS->getCameraTargetOrientation() * mRelativePositionToCameraTarget;

				if (mCollisionsEnabled)
				{
					mCameraPosition = collisionDelegate(mCameraCS->getCameraTargetPosition(), mCameraPosition);
				}
			}
		}

		// Specific methods

		EFORCEINLINE virtual void setCameraRelativePosition(Ogre::Vector3 posRelativeToCameraTarget)
		{
			mRelativePositionToCameraTarget = posRelativeToCameraTarget;
			instantUpdate();
		}

		EFORCEINLINE virtual void setFixedYawAxis(bool useFixedAxis, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y)
		{
			mFixedAxis = fixedAxis;
			mCameraCS->setFixedYawAxis(true, mFixedAxis);
		}

	protected:
		Ogre::Vector3 mFixedAxis;
		Ogre::Vector3 mRelativePositionToCameraTarget;

		bool mFixedStep;
		float mDelta;
		float mRemainingTime;
	};

	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * This mode is similar to "Chase" camera mode but the camera orientation is not fixed by
	 * a yaw axis. The camera orientation will be the same as the target.
	 */
	class ChaseFreeYawAxisCameraMode : public ChaseCameraMode
	{
	public:

		ChaseFreeYawAxisCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &relativePositionToCameraTarget
			, Ogre::Quaternion rotation, float collisionmargin = 0.1f)
			: ChaseCameraMode(cam, relativePositionToCameraTarget, collisionmargin)
		{
			mRotationOffset = rotation;
		}

		ChaseFreeYawAxisCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &relativePositionToCameraTarget
			, const Ogre::Radian roll, const Ogre::Radian yaw, const Ogre::Radian pitch, float collisionmargin = 0.1f)
			: ChaseCameraMode(cam, relativePositionToCameraTarget, collisionmargin)
		{
			mRotationOffset = Ogre::Quaternion(roll, Ogre::Vector3::UNIT_Z)
				* Ogre::Quaternion(yaw, Ogre::Vector3::UNIT_Y)
				* Ogre::Quaternion(pitch, Ogre::Vector3::UNIT_X);
		}

		virtual ~ChaseFreeYawAxisCameraMode(){};

		virtual bool init()
		{
			CameraMode::init();

			ChaseCameraMode::setFixedYawAxis(false);
			mCameraCS->setAutoTrackingTarget(false);

			this->collisionDelegate = newCollisionDelegate(this
				, &CollidableCamera::DefaultCollisionDetectionFunction);

			instantUpdate();

			return true;
		}

		virtual void update(const float &timeSinceLastFrame)
		{
			// Update camera position
			ChaseCameraMode::update(timeSinceLastFrame);

			// Update camera orientation
			mCameraOrientation = mCameraCS->getCameraTargetOrientation() * mRotationOffset;
		}

		virtual void instantUpdate()
		{
			if (mCameraCS->getTargetSceneNode())
			{
				// Update camera position
				ChaseCameraMode::instantUpdate();

				// Update camera orientation
				mCameraOrientation = mCameraCS->getCameraTargetOrientation() * mRotationOffset;
			}
		}

		// Specific methods

		EFORCEINLINE virtual void setCameraRelativePosition(Ogre::Vector3 posRelativeToCameraTarget
			, Ogre::Quaternion rotation)
		{
			mRelativePositionToCameraTarget = posRelativeToCameraTarget;

			mRotationOffset = rotation;

			instantUpdate();
		}

		EFORCEINLINE virtual void setCameraRelativePosition(Ogre::Vector3 posRelativeToCameraTarget
			, const Ogre::Radian roll, const Ogre::Radian yaw, const Ogre::Radian pitch)
		{
			mRelativePositionToCameraTarget = posRelativeToCameraTarget;

			mRotationOffset = Ogre::Quaternion(roll, Ogre::Vector3::UNIT_Z)
				* Ogre::Quaternion(yaw, Ogre::Vector3::UNIT_Y)
				* Ogre::Quaternion(pitch, Ogre::Vector3::UNIT_X);

			instantUpdate();
		}

	protected:
		Ogre::Quaternion mRotationOffset;
	};

	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * In this mode the camera node is attached to the target node as a child.
	 */
	class AttachedCameraMode : public CameraControlSystem::CameraMode
	{
	public:

		AttachedCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &relativePositionToCameraTarget
			, Ogre::Quaternion rotation)
			: CameraMode(cam)
			, mRotation(rotation)
			, mRelativePositionToCameraTarget(relativePositionToCameraTarget)
		{ }

		AttachedCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &relativePositionToCameraTarget,
			const Ogre::Radian roll, const Ogre::Radian yaw, const Ogre::Radian pitch)
			: CameraMode(cam)
			, mRelativePositionToCameraTarget(relativePositionToCameraTarget)
		{
			mRotation = Ogre::Quaternion(roll, Ogre::Vector3::UNIT_Z)
				* Ogre::Quaternion(yaw, Ogre::Vector3::UNIT_Y)
				* Ogre::Quaternion(pitch, Ogre::Vector3::UNIT_X);
		}

		virtual ~AttachedCameraMode(){};

		virtual bool init()
		{
			CameraMode::init();

			mCameraCS->setFixedYawAxis(false);
			mCameraCS->setAutoTrackingTarget(false);

			mCameraPreviousParentSceneNode = mCameraCS->getCameraSceneNode()->getParent();
			mCameraPreviousParentSceneNode->removeChild(mCameraCS->getCameraSceneNode());
			if (mCameraCS->getTargetSceneNode())
				mCameraCS->getTargetSceneNode()->addChild(mCameraCS->getCameraSceneNode());

			mCameraPosition = mRelativePositionToCameraTarget;

			mCameraOrientation = mRotation;

			instantUpdate();

			return true;
		}

		virtual void stop()
		{
			mCameraCS->getCameraSceneNode()->getParent()->removeChild(mCameraCS->getCameraSceneNode());
			mCameraPreviousParentSceneNode->addChild(mCameraCS->getCameraSceneNode());
		}

		virtual void update(const float &timeSinceLastFrame) { }
		virtual void instantUpdate() { }

	protected:
		Ogre::Quaternion mRotation;
		Ogre::Vector3 mRelativePositionToCameraTarget;
		Ogre::Node* mCameraPreviousParentSceneNode;
	};

	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * This is basically an attached camera mode with the posibility of hide
	 * or show the focus of the scene
	 */
	class FirstPersonCameraMode : public AttachedCameraMode
	{
	public:

		FirstPersonCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &relativePositionToCameraTarget
			, Ogre::Quaternion rotation)
			: AttachedCameraMode(cam, relativePositionToCameraTarget, rotation)
			, mCharacterVisible(true)
		{ }

		FirstPersonCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &relativePositionToCameraTarget
			, const Ogre::Radian roll, const Ogre::Radian yaw, const Ogre::Radian pitch)
			: AttachedCameraMode(cam, relativePositionToCameraTarget, roll, yaw, pitch)
			, mCharacterVisible(true)
		{ }

		virtual ~FirstPersonCameraMode(){};

		virtual bool init()
		{
			AttachedCameraMode::init();

			if (mCameraCS->getTargetSceneNode())
				mCameraCS->getTargetSceneNode()->setVisible(mCharacterVisible);

			return true;
		}

		virtual void stop()
		{
			AttachedCameraMode::stop();

			if (mCameraCS->getTargetSceneNode())
				mCameraCS->getTargetSceneNode()->setVisible(true);
		}

		// Specific methods

		void setCharacterVisible(bool value)
		{
			mCharacterVisible = value;
			if (mCameraCS->getTargetSceneNode())
				mCameraCS->getTargetSceneNode()->setVisible(mCharacterVisible);
		}

		bool getCharacterVisible(){ return mCharacterVisible; }

	protected:
		bool mCharacterVisible;
	};

	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * In this mode the camera is constrained to the limits of a plane.
	 * The camera always points to the target, perpendicularly to the plane.
	 */
	class PlaneBindedCameraMode : public CameraControlSystem::CameraModeWithTightness
	{
	public:

		PlaneBindedCameraMode(CameraControlSystem* cam, Ogre::Plane &plane, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y)
			: CameraControlSystem::CameraModeWithTightness(cam)
			, mFixedAxis(fixedAxis)
			, mPlane(plane)
		{
				mTightness = 1;
		}

		virtual ~PlaneBindedCameraMode(){};

		virtual bool init()
		{
			CameraMode::init();

			mCameraCS->setFixedYawAxis(true, mFixedAxis);
			mCameraCS->setAutoTrackingTarget(true);

			instantUpdate();

			return true;
		}

		virtual void update(const float &timeSinceLastFrame)
		{
			float distance = mPlane.getDistance(mCameraCS->getCameraTargetPosition());

			Ogre::Vector3 cameraCurrentPosition = mCameraCS->getCameraPosition();
			Ogre::Vector3 cameraFinalPositionIfNoTightness = mCameraCS->getCameraTargetPosition() -
				mPlane.normal.normalisedCopy() * distance;

			Ogre::Vector3 diff = (cameraFinalPositionIfNoTightness - cameraCurrentPosition) * mTightness;
			mCameraPosition += diff;
		}

		virtual void instantUpdate()
		{
			float distance = mPlane.getDistance(mCameraCS->getCameraTargetPosition());
			mCameraPosition = mCameraCS->getCameraTargetPosition() -
				mPlane.normal.normalisedCopy() * distance;
		}

	protected:
		Ogre::Vector3 mFixedAxis;
		Ogre::Plane mPlane;
	};

	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * In this mode the camera points to a given position (the "focus")
	 * throuh the target. The camera orientation is fixed by a yaw axis.
	 */
	class ThroughTargetCameraMode : public CameraControlSystem::CameraMode
	{
	public:

		ThroughTargetCameraMode(CameraControlSystem* cam, float margin, bool inverse = false
			, Ogre::Vector3 focusPos = Ogre::Vector3::ZERO, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y)
			: CameraControlSystem::CameraMode(cam)
			, mFixedAxis(fixedAxis)
			, mMargin(margin)
			, mInverse(inverse)
			, mFocusPos(focusPos)
		{ }

		virtual ~ThroughTargetCameraMode(){}

		virtual bool init()
		{
			CameraMode::init();

			mCameraCS->setFixedYawAxis(true, mFixedAxis);
			mCameraCS->setAutoTrackingTarget(true);

			instantUpdate();

			return true;
		}

		virtual void update(const float &timeSinceLastFrame) {
			instantUpdate();
		}

		virtual void instantUpdate()
		{
			if (mCameraCS->hasCameraTarget())
			{
				if (!mInverse)
				{
					Ogre::Vector3 focusToTarget = mCameraCS->getCameraTargetPosition() - mFocusPos;
					mCameraPosition = mCameraCS->getCameraTargetPosition() +
						focusToTarget.normalisedCopy() * mMargin;
				}
				else
				{
					Ogre::Vector3 focusToTarget = mCameraCS->getCameraTargetPosition() - mFocusPos;
					mCameraPosition = mFocusPos -
						focusToTarget.normalisedCopy() * mMargin;
				}
			}
		}

		// Specific methods

		EFORCEINLINE virtual void setCameraFocusPosition(Ogre::Vector3 pos) {
			mFocusPos = pos;
			init();
		}

		void setInverse(bool value){ mInverse = value; }
		bool getInverse(){ return mInverse; }

	protected:
		Ogre::Vector3 mFixedAxis;
		Ogre::Vector3 mFocusPos;
		float mMargin;
		bool mInverse;
	};

	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * In this camera mode the position of the camera is chosen to be
	 * the closest to the target of a given list. The camera orientation
	 * is fixed by a yaw axis.
	 */
	class ClosestToTargetCameraMode : public FixedTrackingCameraMode
	{
	public:

		ClosestToTargetCameraMode(CameraControlSystem* cam, float timeInterval = 1
			, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y)
			: FixedTrackingCameraMode(cam, fixedAxis)
			, mTimeInterval(timeInterval)
			, mTime(timeInterval)
		{ };

		virtual ~ClosestToTargetCameraMode(){};

		virtual void update(const float &timeSinceLastFrame)        {
			instantUpdate();
		}

		virtual void instantUpdate()
		{
			if (mCameraCS->hasCameraTarget())
			{
				float minDistance = std::numeric_limits<float>::max();
				Ogre::Vector3 targetPosition = mCameraCS->getCameraTargetPosition();

				std::vector<Ogre::Vector3>::const_iterator it;
				std::vector<Ogre::Vector3>::const_iterator closest;
				for (it = mPositionsList.begin(); it != mPositionsList.end(); ++it)
				{
					Ogre::Vector3 vector_i = ((Ogre::Vector3)(*it));
					float distance = (vector_i - targetPosition).length();
					if (distance < minDistance)
					{
						closest = it;
						minDistance = distance;
					}
				}

				mCameraPosition = (Ogre::Vector3)(*closest);
			}
		};

		// Specific methods

		EFORCEINLINE virtual void addCameraPosition(Ogre::Vector3 &pos)
		{
			mPositionsList.push_back(pos);
			init();
		}

	protected:
		std::vector<Ogre::Vector3> mPositionsList;
		float mTimeInterval;
		float mTime;
		bool mInverse;
	};

	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * In this mode the camera is controlled by the user. In this mode the
	 * target is always seen from the same point of view.
	 */
	class FixedDirectionCameraMode : public CameraControlSystem::CameraModeWithTightness
	{
	public:

		FixedDirectionCameraMode(CameraControlSystem* cam, const Ogre::Vector3 &direction, const float distance
			, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y)
			: CameraControlSystem::CameraModeWithTightness(cam)
			, mFixedAxis(fixedAxis)
			, mDistance(distance)
			, mDirection(direction.normalisedCopy())
		{
				mTightness = 1;
		};

		virtual ~FixedDirectionCameraMode(){};

		virtual bool init()
		{
			CameraMode::init();

			mCameraCS->setFixedYawAxis(true, mFixedAxis);
			mCameraCS->setAutoTrackingTarget(true);

			instantUpdate();

			return true;
		}

		virtual void update(const float &timeSinceLastFrame)
		{
			Ogre::Vector3 cameraCurrentPosition = mCameraCS->getCameraPosition();
			Ogre::Vector3 cameraFinalPositionIfNoTightness = mCameraCS->getCameraTargetPosition()
				- mDirection * mDistance;

			Ogre::Vector3 diff = (cameraFinalPositionIfNoTightness - cameraCurrentPosition) * mTightness;
			mCameraPosition += diff;
		}

		virtual void instantUpdate()
		{
			mCameraPosition = mCameraCS->getCameraTargetPosition()
				- mDirection * mDistance;
		}

		// Specific methods

		EFORCEINLINE virtual void setDirection(Ogre::Vector3 direction)
		{
			mDirection = direction.normalisedCopy();
			instantUpdate();
		}

		EFORCEINLINE virtual void setDistance(float distance)
		{
			mDistance = distance;
			instantUpdate();
		}

	protected:
		Ogre::Vector3 mFixedAxis;
		float mDistance;
		Ogre::Vector3 mDirection;
	};

	//--------------------------------------------------------------------------------------------------------------------
	/**
	 * In this mode the camera is constrained to the limits of a plane.
	 * This is the typical camera you can found in any Real Time Strategy game.
	 */
	class RTSCameraMode : public CameraControlSystem::CameraMode
	{
	public:

		RTSCameraMode(CameraControlSystem* cam
			, const Ogre::Vector3 &initialPosition = Ogre::Vector3::ZERO
			, const Ogre::Vector3 &upAxis = Ogre::Vector3::NEGATIVE_UNIT_Z
			, const Ogre::Vector3 &leftAxis = Ogre::Vector3::NEGATIVE_UNIT_X
			, const Ogre::Radian cameraPitch = Ogre::Radian(Ogre::Degree(90))
			, float minZoom = 0, float maxZoom = 0)
			: CameraControlSystem::CameraMode(cam)
			, mUpAxis(upAxis.normalisedCopy())
			, mLeftAxis(leftAxis.normalisedCopy())
			, mMoveFactor(1)
			, mCameraPitch(cameraPitch)
			, mMinZoom(minZoom)
			, mMaxZoom(maxZoom)
			, mZoom(0)
		{
				mRotation = Ogre::Quaternion(cameraPitch, leftAxis);
				mHeightAxis = -upAxis.crossProduct(leftAxis).normalisedCopy();
				mCameraPosition = initialPosition;

				mHeightDisplacement = 0;
				mLateralDisplacement = 0;
				mVerticalDisplacement = 0;
		};

		virtual ~RTSCameraMode(){};

		virtual bool init()
		{
			CameraMode::init();

			mCameraCS->setFixedYawAxis(false);
			mCameraCS->setAutoTrackingTarget(false);

			mCameraOrientation = mRotation;

			instantUpdate();

			return true;
		}

		virtual void update(const float &timeSinceLastFrame)
		{
			Ogre::Vector3 displacement = ((mUpAxis * mVerticalDisplacement)
				+ (mHeightAxis * mHeightDisplacement)
				+ (mLeftAxis * mLateralDisplacement)) * timeSinceLastFrame * mMoveFactor;

			mCameraPosition += displacement;

			mHeightDisplacement = 0;
			mLateralDisplacement = 0;
			mVerticalDisplacement = 0;

			mCameraOrientation = mRotation;
		}

		virtual void instantUpdate()
		{
			Ogre::Vector3 displacement = ((mUpAxis * mVerticalDisplacement)
				+ (mHeightAxis * mHeightDisplacement)
				+ (mLeftAxis * mLateralDisplacement));

			mCameraPosition += displacement;

			mHeightDisplacement = 0;
			mLateralDisplacement = 0;
			mVerticalDisplacement = 0;

			mCameraOrientation = mRotation;
		}

		// Specific methods

		void setCameraPitch(Ogre::Radian cameraPitch)
		{
			mCameraPitch = cameraPitch;
			mRotation = Ogre::Quaternion(mCameraPitch, mLeftAxis);
		}

		Ogre::Radian getCameraPitch(){ return mCameraPitch; };

		/**
		 * @brief Set the moving speed factor (increase the height)
		 *
		 * @param unitsPerSecond the units the camera will be translated per second
		 */
		EFORCEINLINE virtual void setMoveFactor(float unitsPerSecond){ mMoveFactor = unitsPerSecond; }

		void setZoom(float zoom)
		{
			float desiredDisplacement = zoom - mZoom;

			//float desiredZoom = mZoom + desiredDisplacement;

			if (zoom <= mMaxZoom && zoom >= mMinZoom)
			{
				mHeightDisplacement = desiredDisplacement;
				mZoom = zoom;
			}
			else if (zoom > mMaxZoom)
			{
				mHeightDisplacement = mMaxZoom - mZoom;
				mZoom = mMaxZoom;
			}
			else if (zoom < mMinZoom)
			{
				mHeightDisplacement = mMinZoom - mZoom;
				mZoom = mMinZoom;
			}

			instantUpdate();
		}

		float getZoom(){ return mZoom; };

		/**
		 * @brief Tell the camera to zoom in (reduce the height)
		 *
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
		EFORCEINLINE virtual void zoomIn(float percentage = 1)
		{
			float desiredDisplacement = mMoveFactor * percentage;
			float desiredZoom = mZoom + desiredDisplacement;

			setZoom(desiredZoom);

			/*if(desiredZoom <= mMaxZoom && desiredZoom >= mMinZoom)
			{
			mHeightDisplacement = desiredDisplacement;
			mZoom = desiredZoom;
			}
			else if(desiredZoom > mMaxZoom)
			{
			mHeightDisplacement = mMaxZoom - mZoom;
			mZoom = mMaxZoom;
			}
			else if(desiredZoom < mMinZoom)
			{
			mHeightDisplacement = mMinZoom - mZoom;
			mZoom = mMinZoom;
			}*/
		}

		/**
		 * @brief Tell the camera to zoom out
		 *
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
		EFORCEINLINE virtual void zoomOut(float percentage = 1){ zoomIn(-percentage); }

		/**
		 * @brief Tell the camera to go right (along the leftAxis)
		 *
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
		EFORCEINLINE virtual void goRight(float percentage = 1){ mLateralDisplacement -= mMoveFactor * percentage; }

		/**
		 * @brief Tell the camera to go left (along the leftAxis)
		 *
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
		EFORCEINLINE virtual void goLeft(float percentage = 1){ goRight(-percentage); }

		/**
		 * @brief Tell the camera to go up (along the upAxis)
		 *
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
		EFORCEINLINE virtual void goUp(float percentage = 1){ mVerticalDisplacement += mMoveFactor * percentage; }

		/**
		 * @brief Tell the camera to go down (along the upAxis)
		 *
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
		EFORCEINLINE virtual void goDown(float percentage = 1){ goUp(-percentage); }

	protected:

		Ogre::Quaternion mRotation;
		Ogre::Radian mCameraPitch;
		Ogre::Vector3 mUpAxis;
		Ogre::Vector3 mLeftAxis;
		Ogre::Vector3 mHeightAxis;
		float mMoveFactor;
		float mHeightDisplacement;
		float mLateralDisplacement;
		float mVerticalDisplacement;
		float mZoom;
		float mMinZoom;
		float mMaxZoom;
	};
}

#endif
