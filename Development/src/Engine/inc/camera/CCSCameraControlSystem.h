/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#ifndef _CameraControlSystem_H_
#define _CameraControlSystem_H_


#include "CCSFastDelegate.h"

namespace vega
{
    class ENGINE_API CameraControlSystem 
	{
	public:

		/**
		 * @brief Abstract common base class of all camera modes
		 *
		 * Common base class of all camera modes. All derived camera modes
		 * should implement, at least, the "update" and "instantUpdate" methods
		 */
        class CameraMode
        {
        public:

			/**
			 * @brief Constructor
			 * @param cam a reference to a valid CCS
			 */
            CameraMode(CameraControlSystem* cam) : mCameraCS(cam)
                , mCameraPosition(Ogre::Vector3::ZERO)
                , mCameraOrientation(Ogre::Quaternion::IDENTITY) {};

			/**
			 * @brief Destructor
			 */
			virtual ~CameraMode() {};

			/**
			 * @brief Called when the camera mode is activated
			 *
			 * This method is called each time the camera mode is activated.
			 * All the initializations the camera mode needs in order to work
			 * should be performed here.
			 *
			 * @return true if no errors found
			 */
            virtual bool init()
			{ 
				mCameraPosition = mCameraCS->getCameraPosition(); 
				mCameraOrientation = mCameraCS->getCameraOrientation();
				return true; 
			};

			/**
			 * @brief Called when the camera mode is deactivated
			 *
			 * This method is called each time the camera mode is deactivated.
			 * All the actions performed in the init method which affect to other 
			 * camera modes should be resersed here.
			 */
            virtual void stop(){ };

			/**
			 * @brief Update camera position
			 *
			 * This method is called once per frame. The camera position and
			 * orientation variable members should be modified in this method.
			 */
            virtual void update(const float &timeSinceLastFrame) = 0;

			/**
			 * @brief Update camera position instantaneously
			 *
			 * This method is usually called just after the init method. The camera 
			 * position and orientation variable members should be modified in this
			 * method.
			 */
            virtual void instantUpdate() = 0;

            Ogre::Vector3 getCameraPosition() { return mCameraPosition; }
            Ogre::Quaternion getCameraOrientation() { return mCameraOrientation; }

        protected:
            CameraControlSystem* mCameraCS;
            Ogre::Vector3 mCameraPosition;
            Ogre::Quaternion mCameraOrientation;
        };

		/**
		 * @brief Abstract base camera mode with tightness
		 */
        class CameraModeWithTightness : public CameraMode
        {
        public:

			/**
			 * @brief Constructor
			 * @param cam a reference to a valid CCS
			 */
            CameraModeWithTightness(CameraControlSystem* cam) 
				: CameraMode(cam)
				, mTightness(1)
			{};

			/**
			 * @brief Destructor
			 */
            virtual ~CameraModeWithTightness() {};

            FORCEINLINE void setCameraTightness(float value) { mTightness = value; }
            FORCEINLINE float getCameraTightness() { return mTightness; }

        protected:
            float mTightness;
        };

		/**
		 * @brief Base class for collidable camera modes
		 */
        class CollidableCamera
        {
        public:

			/**
			 * @brief Constructor
			 * @param cam a reference to a valid CCS
			 */
			CollidableCamera(CameraControlSystem* cam, float margin = 0.1f) 				
				: mCameraCS2(cam)
				, mCollisionsEnabled(true)
				, mMargin(margin)
			{ };

			/**
			 * @brief Destructor
			 */
            virtual ~CollidableCamera() {};

            FORCEINLINE virtual void setCollisionsEnabled(bool value) { mCollisionsEnabled = value; }
            FORCEINLINE virtual bool getCollisionsEnabled() { return mCollisionsEnabled; }

			/**
			 * @brief Margin value for the collision delegates
			 * 
			 * @param margin the distance 
			 */
			FORCEINLINE void setMargin(float margin){ mMargin = margin; }

			FORCEINLINE float getMargin(){ return mMargin; }

			// A CollisionDelegate takes as params the position of the target and the camera, and returns the 
			// the new camera position.
			typedef fastdelegate::FastDelegate2<Ogre::Vector3, Ogre::Vector3, Ogre::Vector3> CollisionDelegate;
			
			CollisionDelegate collisionDelegate;

			template <class X, class Y>
			static CollisionDelegate newCollisionDelegate(Y* x, Ogre::Vector3 (X::*func)(Ogre::Vector3 targetPosition, Ogre::Vector3 cameraPosition)) { 
				return fastdelegate::MakeDelegate(x, func);
			}

			/**
			 * @brief Collisions detection based on the basic Ogre raycasting 
			 * 
			 * @param cameraTargetPosition the position of the camera target
			 * @param cameraPosition the position of the camera
			 *
			 * @return the first hit between the camera target position and the camera position.
			 */
			Ogre::Vector3 DefaultCollisionDetectionFunction(Ogre::Vector3 cameraTargetPosition, Ogre::Vector3 cameraPosition);

			/**
			* @brief Ignore the object in collision delegates that use Ogre's ray scene queries.
			*
			* @param object The object that shoud be ignored
			*/
			void addToIgnoreList(Ogre::MovableObject* object){ mIgnoreList.insert(object); };

		protected:
			CameraControlSystem* mCameraCS2;
			bool mCollisionsEnabled;
			float mMargin;

			std::set<Ogre::MovableObject*> mIgnoreList;
		};

		/** 
		 * @brief Contructor 
		 *
		 * @param sm Ogre Scene manager pointer
		 * @param name Name of the instance
         * @param camera Ogre camera. If null it will be created by the CCS
		 * @param recalculateOnTargetMoving If true the camera position will be recalculated at the end of the frame if the target scene node is changed within the current frame.
		 */
		CameraControlSystem(Ogre::SceneManager* sm, Ogre::String name, Ogre::Camera *camera = NULL, bool recalculateOnTargetMoving = true);

		/** 
		 * @brief Contructor
		 *
		 * @param sm Ogre Scene manager pointer
		 * @param name Name of the instance
         * @param customCameraSceneNode External scenenode holding the Ogre camera. 
		 * @param recalculateOnTargetMoving If true the customCameraSceneNode position will be recalculated at the end of the frame if the target scene node is changed within the current frame.
		 */
		CameraControlSystem(Ogre::SceneManager* sm, Ogre::String name, Ogre::SceneNode* customCameraSceneNode, bool recalculateOnTargetMoving = true);

		/**
		 * @brief Destructor
		 */
		~CameraControlSystem();

		/**
		 * @brief Add a new camera mode
		 *
		 * Add a new named camera mode instance to the CCS. Multiple instances of the
		 * same camera mode, with the same or different parameters, can be registered 
		 * in the same CCS if their instance names are differents. 
		 *
		 * @param name the name to assign to the camera mode
		 * @param cameraMode cameraMode the camera mode to be registered
		 */
        FORCEINLINE void registerCameraMode(Ogre::String name, CameraMode* cameraMode)
        {
            mCameraModes[name] = cameraMode;
        }

		/**
		 * @brief Remove all camera modes associated to the CCS. 
		 */
		void deleteAllCameraModes();

		/**
		 * @brief Get the camera mode instance by name
		 *
		 * @param name the name of the camera mode to search for
		 *
		 * @return the cameraMode or null if not found
		 */
        CameraMode* getCameraMode(Ogre::String name);

		/**
		 * @brief Get the next camera mode instance (alphabetic order)
		 *
		 * @return the next cameraMode instance or the first if the current 
		 * is the latest one
		 */
        CameraMode* getNextCameraMode();

		/**
		 * @brief Get the name of a camera mode instance
		 *
		 * @param camMode the camera mode instance
		 *
		 * @return the name of the camera mode instance or an empty string
		 * if the camera instance has not been found in the CCS
		 */
        Ogre::String getCameraModeName(CameraMode* camMode);

		/**
		 * @brief Get the current camera mode instance
		 *
		 * @return the current camera mode instance
		 */
        FORCEINLINE CameraMode* getCurrentCameraMode(){ return mCurrentCameraMode; };

		/**
		 * @brief Set the current camera mode instance
		 *
		 * @param cameraMode the camera mode instance to be set
		 */
        void setCurrentCameraMode(CameraMode* cameraMode);

		/**
		 * @brief Update the CCS
		 *
		 * This method should be called once per frame
		 *
		 * @param timeSinceLastFrame time elapsed since last frame
		 */
        void update(const float &timeSinceLastFrame);
		
		/**
		 * @brief Get the ogre::camera associated to the CCS
		 *
		 * @return the ogre::camera
		 */
		FORCEINLINE Ogre::Camera* getOgreCamera(){ return mCamera; }

		/**
		 * @brief Set the ogre::camera associated to the CCS
		 */
		FORCEINLINE void setOgreCamera(Ogre::Camera* camera){ mCamera = camera; }

		/**
		 * @brief Set the focus of the scene
		 *
		 * This does not mean that the camera will always look to the focus. It
		 * depends on the camera mode
		 *
		 * @param targetNode the focus 
		 */
        void setCameraTarget(Ogre::SceneNode* targetNode);

		/**
		 * @brief Get wheter the CCS has a scene focus defined
		 *
		 * @return true if it has or false in other case
		 */
        FORCEINLINE bool hasCameraTarget(){ return (mTargetNode)?true:false; };

		/**
		 * @brief Get the scene node which holds the focus of the scene
		 *
		 * @return the scene node holding the scene focus 
		 */
        FORCEINLINE Ogre::SceneNode* getTargetSceneNode(){ return mTargetNode; }

		/**
		 * @brief Get the scene node the camera is attached to
		 *
		 * @return the scene node the camera is attached to
		 */
        FORCEINLINE Ogre::SceneNode* getCameraSceneNode(){ return mCameraNode; }

		/**
		 * @brief Get the derived focus position
		 *
		 * @return the position in global coordinate system
		 */
        FORCEINLINE Ogre::Vector3 getCameraTargetPosition(){ return mTargetNode->_getDerivedPosition(); }

		/**
		 * @brief Get the derived camera position
		 *
		 * @return the position in global coordinate system
		 */
        FORCEINLINE Ogre::Vector3 getCameraPosition(){ return mCameraNode->_getDerivedPosition(); }

		/**
		 * @brief Get the derived focus orientation
		 *
		 * @return the orientation in global coordinate system
		 */
        FORCEINLINE Ogre::Quaternion getCameraTargetOrientation(){ return mTargetNode->_getDerivedOrientation(); }

		/**
		 * @brief Get the derived camera orientation
		 *
		 * @return the orientation in global coordinate system
		 */
        FORCEINLINE Ogre::Quaternion getCameraOrientation(){ return mCameraNode->_getDerivedOrientation(); }

		/**
		 * @brief set wheter the camera will always points to the scene focus or not
		 *
		 * The method setFixedYawAxis should be used in combination with this one.
		 *
		 * @param autoTracking if true the camera will always automatically points
		 * to the scene focus
		 */
        void setAutoTrackingTarget(bool autoTracking);

		bool isAutoTrackingTarget();

		/**
		 * @brief Set the fixed yaw axis
		 *
		 * @param useFixedAxis if true, the next parameter is considered
		 * @param fixedAxis the axis which will be used to completely 
		 * define the rotation when using autotracking
		 */
        FORCEINLINE void setFixedYawAxis(bool useFixedAxis, const Ogre::Vector3 &fixedAxis = Ogre::Vector3::UNIT_Y )
        {
            mCameraNode->setFixedYawAxis(useFixedAxis,fixedAxis);
        }

		/**
		 * @brief Get the last timeSinceLastFrame value when the CCS was updated.
		 * 
		 * @return The timeSinceLastFrame value
		 */
		FORCEINLINE float getTimeSinceLastFrameLastUpdate(){ return mTimeSinceLastFrameLastUpdate; };

		FORCEINLINE Ogre::SceneManager* getSceneManager(){ return mSceneMgr; };

		/**
		 * @brief Ignore the object in collision delegates that use Ogre's ray scene queries FOR ALL THE REGISTERED CAMERA MODES.
		 *
		 * @param object The object that shoud be ignored
		 */
		void addToIgnoreList(Ogre::MovableObject* object);

		/**
		 * @brief Set camera position manually. Use it with care: the position of the camera will be overridden by any camera mode (if set).
		 *
		 * @param position The position of the camera node
		 */
		void _setCameraPosition(Ogre::Vector3& position);

		/**
		 * @brief Set camera orientation manually. Use it with care: the orientation of the camera will be overridden by any camera mode (if set).
		 *
		 * @param orientation The orientation of the camera node
		 */
		void _setCameraOrientation(Ogre::Quaternion& orientation);

	protected:

		/**
		 * @brief This class is internally used to notify the CameraControlSystem class the target scene node has been moved within the current frame
		 */
		class NodeListener : public Ogre::Node::Listener
		{
		public:
			NodeListener(CameraControlSystem* cam) : mCameraCS(cam){};
			~NodeListener(){};
			void nodeUpdated(const Ogre::Node *nod){ mCameraCS->update( mCameraCS->getTimeSinceLastFrameLastUpdate() ); };
		protected:
			float mTimeSinceLastFrame;
			CameraControlSystem* mCameraCS;
		};

        Ogre::SceneManager *mSceneMgr;
		Ogre::String mName;

		Ogre::Camera* mCamera; // ogre camera
        Ogre::SceneNode* mCameraNode; // Where the camera is attached to
        bool mOwnCamera; // true if the ogre camera has been created by this class

        Ogre::SceneNode* mTargetNode; // The scene focus
		NodeListener* mTargetNodeListener; // To recalculate the camera position if the target scenenode has been changed within the current frame
		float mTimeSinceLastFrameLastUpdate; // Time value passed to the last call of the method "update"
		bool mRecalculateOnTargetMoving;

        CameraMode* mCurrentCameraMode;
        std::map<Ogre::String, CameraMode*> mCameraModes; // The list of camera mode instances
		
	};
}

#endif
