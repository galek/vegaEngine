#ifndef _OGREAL_LISTENER_H_
#define _OGREAL_LISTENER_H_

#include "AudioALPrereqs.h"

namespace vega {
	/**
	 * Listener.
	 * @remarks
	 * There is only ever one listener in the scene and it is created
	 * when the SoundManager is initialized.  To get the reference to
	 * the listener use SoundManager::getListener.  The listener can 
	 * be attached to an Ogre::SceneNode or placed at some point in space.
	 *
	 * @see Ogre::MovableObject
	 * @see Ogre::Singleton
	 */
	class Listener : public Ogre::MovableObject, public Ogre::Singleton<Listener>
	{
	protected:
		/*
		** Constructors are protected to enforce the use of the 
		** factory via SoundManager::createListener
		*/
		/** Default Constructor. */
		Listener();
		/** Normal Constructor. Should not be called directly! Use SoundManager::getListener */
		Listener(const std::string& name);

	public:
		/** Standard Destructor */
		virtual ~Listener();

		/** Returns the Listener singleton object */
		static Listener& getSingleton();
		/** Returns a pointer to the Listener singleton object */
		static Listener* getSingletonPtr();

		/** 
		 * Sets the gain. 
		 * @param gain The gain where 1.0 is full volume and 0.0 is off
		 * @note Gain should be positive
		 */
		void setGain(float gain);
		/** Returns the gain. */
		float getGain() const {return mGain;}
		/**
		 * Sets the position of the listener.
		 * @param x The x part of the position
		 * @param y The y part of the position
		 * @param z The z part of the position
		 * @note The position will be overridden if the listener is attached to a SceneNode
		 */
		void setPosition(float x, float y, float z);
		/**
		 * Sets the position of the listener.
		 * @param vec The new postion for the listener.
		 * @note The position will be overridden if the listener is attached to a SceneNode
		 */
		void setPosition(const Ogre::Vector3& vec);
		/** Returns the position of the sound. */
		const Ogre::Vector3& getPosition() const {return mPosition;}
		/**
		 * Sets the direction of the listener.
		 * @param x The x part of the direction vector
		 * @param y The y part of the direction vector
		 * @param z The z part of the direction vector
		 * @note The direction will be overridden if the listener is attached to a SceneNode
		 */
		void setDirection(float x, float y, float z);
		/**
		 * Sets the direction of the listener.
		 * @param vec The direction vector.
		 * @note The direction will be overridden if the listener is attached to a SceneNode
		 */
		void setDirection(const Ogre::Vector3& vec);
		/** Returns the direction of the sound. */
		const Ogre::Vector3& getDirection() const {return mDirection;}
		/**
		 * Sets the velocity of the listener.
		 * @param x The x part of the velocity vector
		 * @param y The y part of the velocity vector
		 * @param z The z part of the velocity vector
		 */
		void setVelocity(float x, float y, float z);
		/**
		 * Sets the velocity of the listener.
		 * @param vec The velocity vector.
		 */
		void setVelocity(const Ogre::Vector3& vec);
		/** Returns the velocity of the sound. */
		const Ogre::Vector3& getVelocity() const {return mVelocity;}
		/** Returns the position of the sound including any transform from nodes it is attached to. */
		const Ogre::Vector3& getDerivedPosition() const;
		/** Returns the direction of the sound including any transform from nodes it is attached to. */
		const Ogre::Vector3& getDerivedDirection() const;

		/** Overridden from MovableObject */
		const std::string& getMovableType() const;
		/** Overridden from MovableObject */
		const Ogre::AxisAlignedBox& getBoundingBox() const;
		/** Overridden from MovableObject */
		float getBoundingRadius() const {return 0; /* Not Visible */} 
		/** Overridden from MovableObject */
		void _updateRenderQueue(Ogre::RenderQueue* queue);
		/** Overridden from MovableObject */
		void _notifyAttached(Ogre::Node* parent, bool isTagPoint = false);
	#if OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR >= 6
		/** Overridden from MovableObject */
		virtual void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false){}
	#endif

	protected:
		/// Internal method for synchronising with parent node (if any)
		virtual void update() const;
		/// This is called each frame to update the position, direction, etc
		void updateListener();
		/// Convienance method to reset the sound state
		void initListener();

		/// Postion taking into account the parent node
		mutable Ogre::Vector3 mDerivedPosition;
		/// Direction taking into account the parent node
		mutable Ogre::Vector3 mDerivedDirection;
		/// Stored versions of parent orientation
		mutable Ogre::Quaternion mLastParentOrientation;
		/// Stored versions of parent position
		mutable Ogre::Vector3 mLastParentPosition;

		float mGain;
		Ogre::Vector3 mPosition;
		Ogre::Vector3 mDirection;
		Ogre::Vector3 mVelocity;
		Ogre::Vector3 mUp;
		ALfloat mOrientation[6];
		/// Is the local transform dirty?
		mutable bool mLocalTransformDirty;

		friend class SoundManager;
		friend class ListenerFactory;
	};

	/** Factory object for creating the listener */
	class ListenerFactory : public Ogre::MovableObjectFactory
	{
	public:
		ListenerFactory() {}
		~ListenerFactory() {}

		static std::string FACTORY_TYPE_NAME;

		const std::string& getType() const;
		void destroyInstance(Ogre::MovableObject* obj);

	protected:
		Ogre::MovableObject* createInstanceImpl(const std::string& name, 
			const Ogre::NameValuePairList* params = 0);
	};
} // Namespace
#endif
