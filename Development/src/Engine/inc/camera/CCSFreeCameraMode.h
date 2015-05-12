/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#ifndef _FreeCameraMode_H_
#define _FreeCameraMode_H_


#include "CCSCameraControlSystem.h"

namespace vega
{
	/**
	 * In this mode the camera is controlled by the user. 
     * The camera orientation is fixed by a yaw axis.
	 */
    class FreeCameraMode : public CameraControlSystem::CameraMode, public CameraControlSystem::CollidableCamera
	{
	public:

		enum SWITCHING_MODE { INITIAL_STATE = 0, CURRENT_STATE, LAST_STATE };

		/**
		 * @param margin Collision margin
		 * @param switchingMode Determine the state of the camera when switching to this camera mode from another
		 */
        FreeCameraMode(CameraControlSystem* cam
			, Ogre::Vector3 initialPosition = Ogre::Vector3::ZERO
			, Ogre::Degree initialRotationX = Ogre::Degree(0)
			, Ogre::Degree initialRotationY = Ogre::Degree(0)
			, SWITCHING_MODE switchingMode = CURRENT_STATE
			, float margin = 0.1f) 
            : CameraControlSystem::CameraMode(cam)
            , CameraControlSystem::CollidableCamera(cam, margin)
			, mFixedAxis(Ogre::Vector3::UNIT_Y)
            , mMoveFactor(1)
            , mRotationFactor(0.13)
			, mRotX(initialRotationX)
			, mRotY(initialRotationY)
			, mInitialPosition(initialPosition)
			, mInitialRotationX(initialRotationX)
			, mInitialRotationY(initialRotationY)
			, mLastRotationX(initialRotationX)
			, mLastRotationY(initialRotationY)
			, mLastPosition(initialPosition)
			, mSwitchingMode(switchingMode)

        {
			mCameraPosition = initialPosition;

			this->collisionDelegate = newCollisionDelegate(this
				, &CollidableCamera::DefaultCollisionDetectionFunction);
		}

        virtual ~FreeCameraMode(){};

        virtual bool init();

        virtual void update(const float &timeSinceLastFrame);

        virtual void instantUpdate();
        // Specific methods

		/**
		 * @brief Set the position of the camera
		 * 
		 * @param position position of the camera
		 */
		inline virtual void setCameraPosition(Ogre::Vector3 position){ mCameraPosition = position; }

		/**
		 * @brief Set the orientation of the camera
		 * 
		 * @param orientation orientation of the camera
		 */
		inline virtual void setCameraOrientation(Ogre::Quaternion orientation) { mCameraOrientation = orientation; }

		/**
		 * @brief Set the moving speed factor
		 * 
		 * @param unitsPerSecond the units the camera will be translated per second
		 */
        inline virtual void setMoveFactor(float unitsPerSecond){ mMoveFactor = unitsPerSecond; }

		/**
		 * @brief Get the moving speed factor
		 */
        inline virtual float getMoveFactor(){ return mMoveFactor; }

		/**
		 * @brief Set the rotating speed factor
		 * 
		 * @param radiansPerSecond the radians the camera will be rotated per second
		 */
        inline virtual void setRotationFactor(float radiansPerSecond){ mRotationFactor = radiansPerSecond; }

		/**
		 * @brief Tell the camera to go forward
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void goForward(float percentage = 1){ mLongitudinalDisplacement += mMoveFactor * percentage; }

		/**
		 * @brief Tell the camera to go backward
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void goBackward(float percentage = 1){ goForward(-percentage); }

		/**
		 * @brief Tell the camera to go right (laterally)
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void goRight(float percentage = 1){ mLateralDisplacement += mMoveFactor * percentage; }

		/**
		 * @brief Tell the camera to go left (laterally)
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void goLeft(float percentage = 1){ goRight(-percentage); }

		/**
		 * @brief Tell the camera to go up
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void goUp(float percentage = 1){ mVerticalDisplacement += mMoveFactor * percentage; }

		/**
		 * @brief Tell the camera to go down
		 * 
		 * @param percentage the relative speed of the movement acording to the move factor (1: 100%, 0: 0%, -1: -100%)
		 */
        inline virtual void goDown(float percentage = 1){ goUp(-percentage); }

		/**
		 * @brief Tell the camera to look right
		 * 
		 * @param val the amount of rotation (use negative values to look left)
		 */
        inline virtual void yaw(float val){ mRotX += Ogre::Degree(-val * mRotationFactor); }

        /**
		 * @brief Tell the camera to look down
		 * 
		 * @param val the amount of rotation (use negative values to look up)
		 */
        inline virtual void pitch(float val){ mRotY += Ogre::Degree(-val * mRotationFactor); }

		/**
		 * @brief Set the rotation value (right/left)
		 * 
		 * @param val the amount of rotation (use negative values to look left)
		 */
        inline virtual void setYaw(float val){ mRotX = Ogre::Degree(-val); }

        /**
		 * @brief Set the rotation value (up/down)
		 * 
		 * @param val the amount of rotation (use negative values to look up)
		 */
        inline virtual void setPitch(float val){ mRotY = Ogre::Degree(-val); }

		void reset();

		/**
		 * @brief Set the switching mode
		 * 
		 * @param mode INITIAL_STATE for resetting the camera to its initial state; CURRENT_STATE for mantaining the current camera position and orientation; LAST_STATE for returning to the last known camera state when this camera mode was actived.
		 */
		void setSwitchingMode(SWITCHING_MODE mode){ mSwitchingMode = mode; }

		SWITCHING_MODE getSwitchingMode(){ return mSwitchingMode; }

        /**
		 * @brief Collision delegate that mantains the camera always above the ground. A margin can be stablished using the 'setMargin' method.
		 */
        Ogre::Vector3 AboveGroundCollisionDetectionFunction(Ogre::Vector3 cameraTargetPosition, Ogre::Vector3 cameraPosition);

        /**
		 * @brief Collision delegate that mantains a constant distance to the ground. The distance can be stablished using the 'setMargin' method.
		 */
        Ogre::Vector3 ConstantHeightCollisionDetectionFunction(Ogre::Vector3 cameraTargetPosition, Ogre::Vector3 cameraPosition);

    protected:
        
        Ogre::Vector3 getFirstRealHit(Ogre::Vector3 origin, Ogre::Vector3 direction);

		Ogre::Vector2 getYawAndPitch(const Ogre::Vector3& direction);
		
    protected:

        Ogre::Vector3 mFixedAxis;
        float mMoveFactor;
        float mRotationFactor;
        float mLongitudinalDisplacement;
        float mLateralDisplacement;
        float mVerticalDisplacement;
        //Ogre::Vector3 mStartingPosition;
        Ogre::Radian mRotX;
	    Ogre::Radian mRotY;
        //Ogre::Quaternion mStartingOrientation;
		Ogre::Degree mInitialRotationX;
		Ogre::Degree mInitialRotationY;
		Ogre::Vector3 mInitialPosition;
		Ogre::Degree mLastRotationX;
		Ogre::Degree mLastRotationY;
		Ogre::Vector3 mLastPosition;
		SWITCHING_MODE mSwitchingMode;
	};

}

#endif
