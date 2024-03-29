#include "AudioALException.h"
#include "AudioALListener.h"

template<> vega::Listener* Ogre::Singleton<vega::Listener>::msSingleton = 0;

namespace vega {
	Listener::Listener() :
		mGain(1.0),
		mPosition(Ogre::Vector3::ZERO),
		mDirection(Ogre::Vector3::NEGATIVE_UNIT_Z),
		mVelocity(Ogre::Vector3::ZERO),
		mUp(Ogre::Vector3::UNIT_Y),
		mDerivedPosition(Ogre::Vector3::ZERO),
		mDerivedDirection(Ogre::Vector3::NEGATIVE_UNIT_Z)
	{
		mParentNode = NULL;
		mLocalTransformDirty = false;
		initListener();
	}

	Listener::Listener(const std::string& name) :
		Ogre::MovableObject(name),
		mGain(1.0),
		mPosition(Ogre::Vector3::ZERO),
		mDirection(Ogre::Vector3::NEGATIVE_UNIT_Z),
		mVelocity(Ogre::Vector3::ZERO),
		mUp(Ogre::Vector3::UNIT_Y),
		mDerivedPosition(Ogre::Vector3::ZERO),
		mDerivedDirection(Ogre::Vector3::NEGATIVE_UNIT_Z)
	{
		mParentNode = NULL;
		initListener();
	}
	
	Listener::~Listener()
	{}

	Listener* Listener::getSingletonPtr(void)
	{
		return msSingleton;
	}

	Listener& Listener::getSingleton(void)
	{  
		assert(msSingleton);  return (*msSingleton);  
	}

	void Listener::setGain(float gain)
	{
		mGain = gain;

		alListenerf(AL_GAIN, mGain);
		CheckError(alGetError(), "Failed to set Gain");
	}
	
	void Listener::setPosition(float x, float y, float z)
	{
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;
		mLocalTransformDirty = true;
	}

	void Listener::setPosition(const Ogre::Vector3& vec)
	{
		mPosition = vec;
		mLocalTransformDirty = true;
	}

	void Listener::setDirection(float x, float y, float z)
	{
		mDirection.x = x;
		mDirection.y = y;
		mDirection.z = z;
		mLocalTransformDirty = true;
	}

	void Listener::setDirection(const Ogre::Vector3& vec)
	{
		mDirection = vec;
		mLocalTransformDirty = true;
	}

	void Listener::setVelocity(float x, float y, float z)
	{
		mVelocity.x = x;
		mVelocity.y = y;
		mVelocity.z = z;

		alListener3f(AL_VELOCITY, mVelocity.x, mVelocity.y, mVelocity.z);
		CheckError(alGetError(), "Failed to set Velocity");
	}

	void Listener::setVelocity(const Ogre::Vector3& vec)
	{
		setVelocity(vec.x, vec.y, vec.z);
	}

	const Ogre::Vector3& Listener::getDerivedPosition(void) const
	{
		update();
		return mDerivedPosition;
	}

	const Ogre::Vector3& Listener::getDerivedDirection(void) const
	{
		update();
		return mDerivedDirection;
	}

	void Listener::initListener()
	{
		mOrientation[0]= mDirection.x; // Forward.x
		mOrientation[1]= mDirection.y; // Forward.y
		mOrientation[2]= mDirection.z; // Forward.z

		mOrientation[3]= mUp.x; // Up.x
		mOrientation[4]= mUp.y; // Up.y
		mOrientation[5]= mUp.z; // Up.z

		alListener3f(AL_POSITION, mPosition.x, mPosition.y, mPosition.z);
		CheckError(alGetError(), "Failed to set Position");

		alListenerfv(AL_ORIENTATION, mOrientation);
		CheckError(alGetError(), "Failed to set Orientation");

		alListenerf (AL_GAIN, 1.0f);
		CheckError(alGetError(), "Failed to set Gain");

		alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
		CheckError(alGetError(), "Failed to set Velocity");
	}

	void Listener::update() const
	{
		if (mParentNode)
		{
		    if (!(mParentNode->_getDerivedOrientation() == mLastParentOrientation &&
			mParentNode->_getDerivedPosition() == mLastParentPosition)
			|| mLocalTransformDirty)
		    {
			// Ok, we're out of date with SceneNode we're attached to
			mLastParentOrientation = mParentNode->_getDerivedOrientation();
			mLastParentPosition = mParentNode->_getDerivedPosition();
			mDerivedDirection = mLastParentOrientation * mDirection;
			mDerivedPosition = (mLastParentOrientation * mPosition) + mLastParentPosition;
		    }
		}
		else
		{
		    mDerivedPosition = mPosition;
		    mDerivedDirection = mDirection;
		}

		mLocalTransformDirty = false;
	}

	void Listener::updateListener()
	{
		update();
		if(mParentNode)
		{
			mPosition = mLastParentPosition;
			mDirection = mLastParentOrientation.zAxis();
			mUp = mLastParentOrientation.yAxis();
		}
		alListener3f(AL_POSITION, mPosition.x, mPosition.y, mPosition.z);
		CheckError(alGetError(), "Failed to set Position");

		mOrientation[0]= -mDirection.x; // Forward.x
		mOrientation[1]= -mDirection.y; // Forward.y
		mOrientation[2]= -mDirection.z; // Forward.z

		mOrientation[3]= mUp.x; // Up.x
		mOrientation[4]= mUp.y; // Up.y
		mOrientation[5]= mUp.z; // Up.z

		alListenerfv(AL_ORIENTATION, mOrientation); 
		CheckError(alGetError(), "Failed to set Orientation");
	}

	const std::string& Listener::getMovableType() const
	{
		return ListenerFactory::FACTORY_TYPE_NAME;
	}

	const Ogre::AxisAlignedBox& Listener::getBoundingBox() const
	{
		// Null, Sounds are not visible
		static Ogre::AxisAlignedBox box;
		return box;
	}

	void Listener::_updateRenderQueue(Ogre::RenderQueue* queue)
	{
		// Do Nothing
	}

	void Listener::_notifyAttached(Ogre::Node* parent, bool isTagPoint)
	{
		mParentNode = parent;
	}

	std::string ListenerFactory::FACTORY_TYPE_NAME = "vega_Listener";

	const std::string& ListenerFactory::getType(void) const
	{
		return FACTORY_TYPE_NAME;
	}

	Ogre::MovableObject* ListenerFactory::createInstanceImpl(const std::string& name, 
				const Ogre::NameValuePairList* params)
	{
		Listener *listener = Listener::getSingletonPtr();
		if(listener)
			return listener;
		else
			return new Listener(name);
	}

	void ListenerFactory::destroyInstance(Ogre::MovableObject* obj)
	{
		delete obj;
	}
}
