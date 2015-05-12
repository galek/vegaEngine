#ifndef _OGREAL_SOUND_H_
#define _OGREAL_SOUND_H_

#include "AudioALPrereqs.h"
#include "AudioALUtil.h"
#include "..\..\Common\inc\iAudioDevice.h"

namespace vega {
	class SoundManager;
	/**
	 * Sound.
	 * @remarks
	 * Sounds are created using the SoundManager::createSound method
	 * and then can be attached to Ogre::SceneNodes, placed at a point
	 * in space or placed at an offset relative to the listener.
	 *
	 * @see Ogre::MovableObject
	 */
	class Sound : public iSound, public Ogre::MovableObject
	{
	public:
	  // This listener iterface allows for notifications about certain sound
	  // events. It is the unified listener interface for the sound class.
	  class Listener {
	  public:
	    // Should the listener functions for sound stopping or looping be
	    // called. Set these flags according to your desire to be notified.
	    bool stops, loops;
	    // Add offsets to this list if you wish to be notified of any. If no
	    // offsets are present in the list, the corresponding notification
	    // function will never be called. This list acts as a flag.
	    typedef std::list<float> OffsetList;
	    OffsetList offsets;
	    
	    // Constructor initialises such that nothing is notified about.
	    Listener();
	    virtual ~Listener();

	    // The sound has stopped (not looping).
	    virtual void soundStopped(Sound* sound);
	    // The sound has looped (reached the end and looping).
	    virtual void soundLooped(Sound* sound);
	    // The sound has reached a requested offset. Because OpenAL is
	    // running on a separate thread, you've Buckley's chance of actually
	    // getting notified at the precise offset you want. vega updates
	    // only once per frame, and so we'll notify you as close as possible.
	    virtual void soundAtOffset(Sound* sound, float requestedOffset, float actualOffset);
	  };

	protected:
		/*
		** Constructors are protected to enforce the use of the 
		** factory via SoundManager::createSound
		*/
		/** Default Constructor. */
		Sound(SoundManager*_mSoundManager);
		/** Constructor. Should not be called directly! Use SoundManager::createSound(). */
		Sound(SoundManager*_mSoundManager,const std::string& name, const std::string& fileName, bool stream);
		/** Constructor. Should not be called directly! SoundManager::createSound().
		    To be called if BufferRef exists. */
		Sound(SoundManager*_mSoundManager,BufferRef buffer, const std::string& name, const std::string& fileName, bool loop = false);
		SoundManager*mSoundManager;

	public:
		/** Standard Destructor */
		virtual ~Sound();

		

		/**
		   @brief Retrieve the category number.
		   
		   This is a user-defined number that can be assigned to a 
		   sound, but it's use is intended as a method of classifying
		   sounds into groups. The meaning of this value is user defined.
		   
		   @return The user-assigned category number, or 0 by default.
		 */
		unsigned int getCategory() const;
		
		/**
		   @brief Set the category number.
		   
		   @see getCategory
		   @param category The category number to assign to the sound.
		*/
		void setCategory(unsigned int category);

		/** Plays the sound. */
		bool play();
		/** Returns true if the sound is playing, otherwise false. */
		virtual bool isPlaying() const;
		/** Pauses the sound. @note Upon calling play again, the sound will resume where it left off */
		virtual bool pause();
		/** Returns true if the sound is paused, otherwise false. */
		virtual bool isPaused() const;
		/** Stops the sound. @note Upon calling play again, the sound will resume from the begining */
		bool stop();
		/** Returns true if the sound is stopped, otherwise false. */
		virtual bool isStopped() const;
		/** Returns true if the source does not have a state yet, otherwise false */
		virtual bool isInitial() const;
		/** Returns true if the sound is currently in the process of fading. */
		bool isFading() const;
		/** Starts playing the song while fading in.*/
		bool fadeIn(float fadeTime);
		/** Fades out, but keeps playing at volume 0, so it can be faded in again.*/
		bool fadeOut(float fadeTime);
		/** Cancel a processing fade operation. This leaves the gain at where the 
		    fade operation was cancelled and does not touch it. */
		bool cancelFade();
		/**
		 * Sets the pitch multiplier.
		 * @param pitch The new pitch multiplier
		 * @note pitch must always be positive non-zero, all other values will be ignored
		 */
		void setPitch(float pitch);
		/** Returns the pitch multiplier. */
		float getPitch() const {return mPitch;}
		/** 
		 * Sets the gain. 
		 * @param gain The gain where 1.0 is full volume and 0.0 is off
		 * @note Negative values will be ignored
		 */
		void setGain(float gain);
		/** Returns the gain. */
		float getGain() const {return mGain;}
		/** 
		 * Sets the max gain. 
		 * @param maxGain The maximum amount of gain allowed for this source
		 * @note Valid range is [0.0 - 1.0] all other values will be ignored
		 */
		void setMaxGain(float maxGain);
		/** Returns the max gain. */
		float getMaxGain() const {return mMaxGain;}
		/** 
		 * Sets the min gain. 
		 * @param minGain The minimum amount of gain allowed for this source.
		 * @note Valid range is [0.0 - 1.0] all other values will be ignored
		 */
		void setMinGain(float minGain);
		/** Returns the gain. */
		float getMinGain() const {return mMinGain;}
		/**
		   Retrieve the gain scale.
		   
		   @return The value of the gain scale in the range [0,1].
		*/
		float getGainScale() const;
		/**
		   @brief Set the gain scale.
		   
		   Gain scale is multiplied by the gain to yield final gain. 
		   @param gainScale The gain scale value in the range [0,1].
		 */
		void setGainScale(float gainScale);
		/**
		 * Set the min max and current gain.
		 * @param maxGain The maximum amount of gain allowed for this source
		 * @param minGain The minimum amount of gain allowed for this source
		 * @param gain The current gain for this source
		 * @param gainScale The gain scale factor in the range [0,1].
		 * @note Gain should be positive
		 */
		void setGainValues(float maxGain, float minGain, 
				   float gain, float gainScale = 1.0);
		/**
		 * Sets the Max Distance.
		 * @param maxDistance The max distance used in the Inverse Clamped Distance Model
		 * @note This is the distance where there will be no further attenuation of the source
		 * @note Negative values will be ignored
		 */
		void setMaxDistance(float maxDistance);
		/** Returns the max distance */
		float getMaxDistance() const {return mMaxDistance;}
		/**
		 * Sets the rolloff factor.
		 * @param rolloffFactor The rolloff rate for the source 
		 * @note Used for distance attenuation
		 * @note Negative values will be ignored
		 */
		void setRolloffFactor(float rolloffFactor);
		/** Returns the rolloff factor */
		float getRolloffFactor() const {return mRolloffFactor;}
		/**
		 * Sets the reference distance.
		 * @param refDistance The reference distance used in attenuation calculations.
		 * @note This is the distance under which the volume for the
		 *     source would normally drop by half
		 * @note Negative values will be ignored
		 */
		void setReferenceDistance(float refDistance);
		/** Returns the reference distance. */
		float getReferenceDistance() const {return mReferenceDistance;}
		/**
		 * Sets the variables used in the distance attenuation calculation.
		 * @param maxDistance The max distance used in the Inverse Clamped Distance Model
		 * @param rolloffFactor The rolloff rate for the source
		 * @param refDistance The reference distance used in attenuation calculations
		 */
		void setDistanceValues(float maxDistance, float rolloffFactor, float refDistance);
		/**
		 * Sets the velocity of the sound.
		 * @param x The x part of the velocity vector
		 * @param y The y part of the velocity vector
		 * @param z The z part of the velocity vector
		 */
		void setVelocity(float x, float y, float z);
		/** Sets the velocity of the sound. @param vec The velocity vector */
		void setVelocity(const Ogre::Vector3& vec);
		/** Returns the velocity of the sound. */
		const Ogre::Vector3& getVelocity() const {return mVelocity;}
		/** 
		 * Sets the source relation to the listner.
		 * @param relative Whether the sound position is
		 *    relative to the listener or not.
		 * @note If this is set to true then the position of the
		 * sound will be relative to the listener.  However,
		 * setting this to true will have no effect if the sound
		 * is connected to a scene node.
		 */
		void setRelativeToListener(bool relative);
		/** Returns true if the sound is relative to the listener, otherwise false. */
		bool isRelativeToListener() const {return mSourceRelative==AL_TRUE?true:false;}
		/**
		 * Sets the position of the sound.
		 * @param x The x part of the position
		 * @param y The y part of the position
		 * @param z The z part of the position
		 * @note In the case that this sound is attached to a SceneNode this
		 *     position become the offset from the parent scene node
		 */
		void setPosition(float x, float y, float z);
		/**
		 * Sets the position of the sound.
		 * @param vec The new postion for the sound.
		 * @note In the case that this sound is attached to a SceneNode this
		 *     position become the offset from the parent scene node
		 */
		void setPosition(const Ogre::Vector3& vec);
		/** Returns the position of the sound. */
		const Ogre::Vector3& getPosition() const {return mPosition;}
		/**
		 * Sets the direction of the sound.
		 * @param x The x part of the direction vector
		 * @param y The y part of the direction vector
		 * @param z The z part of the direction vector
		 * @note In the case that this sound is attached to a SceneNode this
		 *     directions becomes relative to the parent's direction
		 */
		void setDirection(float x, float y, float z);
		/**
		 * Sets the direction of the sound.
		 * @param vec The direction vector.
		 * @note In the case that this sound is attached to a SceneNode this
		 *     directions becomes relative to the parent's direction
		 */
		void setDirection(const Ogre::Vector3& vec);
		/** Returns the direction of the sound. */
		const Ogre::Vector3& getDirection() const {return mDirection;}
		/**
		 * Sets the gain outside the sound cone of a directional sound.
		 * @param outerConeGain The gain outside the directional cone
		 * @note Each directional source has three zones:<ol><li>The inner zone as defined by the 
		 *     <i>setInnerConeAngle</i> where the gain is constant and is set by <i>setGain</i></li>
		 *     <li>The outer zone which is set by <i>setOuterConeAngle</i> and the gain is a linear 
		 *     transition between the gain and the outerConeGain</li><li>Outside of the sound cone.  
		 *     The gain in this zone is set by <i>setOuterConeGain</i></li></ol>
		 * @note Valid range is [0.0 - 1.0] all other values will be ignored
		 */
		void setOuterConeGain(float outerConeGain);
		/** Returns the outerConeGain */
		float getOuterConeGain() const {return mOuterConeGain;}
		/**
		 * Sets the inner angle of the sound cone for a directional sound.
		 * @param innerConeAngle The angle that defines the inner cone of a directional sound.  Valid values are [0 - 360]
		 * @note Each directional source has three zones:<ol><li>The inner zone as defined by the 
		 *     <i>setInnerConeAngle</i> where the gain is constant and is set by <i>setGain</i></li>
		 *     <li>The outer zone which is set by <i>setOuterConeAngle</i> and the gain is a linear 
		 *     transition between the gain and the outerConeGain</li><li>Outside of the sound cone.  
		 *     The gain in this zone is set by <i>setOuterConeGain</i></li></ol>
		 */
		void setInnerConeAngle(float innerConeAngle);
		/** Return the innerConeAngle */
		float getInnerConeAngle() const {return mInnerConeAngle;}
		/**
		 * Sets the outer angle of the sound cone for a directional sound.
		 * @param outerConeAngle The angle that defines the outer cone of a directional sound.  Valid values are [0 - 360]
		 * @note Each directional source has three zones:<ol><li>The inner zone as defined by the 
		 *     <i>setInnerConeAngle</i> where the gain is constant and is set by <i>setGain</i></li>
		 *     <li>The outer zone which is set by <i>setOuterConeAngle</i> and the gain is a linear 
		 *     transition between the gain and the outerConeGain</li><li>Outside of the sound cone.  
		 *     The gain in this zone is set by <i>setOuterConeGain</i></li></ol>
		 */
		void setOuterConeAngle(float outerConeAngle);
		/** Returns the outerConeAngle */
		float getOuterConeAngle() {return mOuterConeAngle;}
		/** Sets the looping state */
		virtual void setLoop(bool loop);
		/** Returns looping state */
		bool isLooping() const {return mLoop==AL_TRUE?true:false;}
		/** Returns streaming state */
		bool isStreaming() const {return mStream==AL_TRUE?true:false;}
		/** Sets the priority of the sound */
		void setPriority(Priority priority) {mPriority = priority;}
		/** Returns the current Priority for the sound */
		Priority getPriority() const {return mPriority;}
		/** Returns the duration of the audio in seconds */
		float getSecondDuration() {return mLengthInSeconds;}
		/** Sets the offset within the audio stream in seconds. @note Negative values will be ignored */
		virtual void setSecondOffset(float seconds);
		/** Returns the current offset within the audio stream in seconds */
		virtual float getSecondOffset();
		/** Returns the position of the sound including any transform from nodes it is attached to. */
		const Ogre::Vector3& getDerivedPosition() const {return mDerivedPosition;}
		/** Returns the direction of the sound including any transform from nodes it is attached to. */
		const Ogre::Vector3& getDerivedDirection() const {return mDerivedDirection;}
		/** Returns the name of the file used to create this Sound. */
		const std::string& getFileName() const {return mFileName;}

		/** Sets a callback to alert the user when the Sound has stopped playing */
		template<typename T>
		void addSoundFinishedHandler(T *object, void(T::*function)(Sound *sound))
		{
			mFinishedCallback = new MemberFunctionPointer<T>(function, object);
		}
		template<typename T>
		void addSoundLoopedHandler(T *object, void(T::*function)(Sound *sound))
		{
			mLoopedCallback = new MemberFunctionPointer<T>(function, object);
		}

		// Add and remove sound listeneder.
		// Add a sound listener. If the listener already exists in the list,
		// it will not be added a second time.
		void addListener(Listener* listener);
		// Remove a sound listener. If the listener does not exist in the list,
		// nothing happens. It is assumed that it is removed.
		void removeListener(Listener* listener);

		/** Overridden from MovableObject */
		const std::string& getMovableType() const;
		/** Overridden from MovableObject */
		const Ogre::AxisAlignedBox& getBoundingBox() const;
		/** Overridden from MovableObject */
		float getBoundingRadius() const {return 0; /* Not Visible */} 
		/** Overridden from MovableObject */
		void _updateRenderQueue(Ogre::RenderQueue* queue);
		/** Notifies the sound when it is attached to a node */
		void _notifyAttached(Ogre::Node *parent, bool isTagPoint = false);
	#if(OGRE_VERSION_MAJOR == 1 && OGRE_VERSION_MINOR >= 6)
		/** Overridden from MovableObject */
		virtual void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false){}
	#endif

	protected:
		/// Updates the sound if need be
		virtual bool updateSound();
		/// Sets the source for the sound
		void setSource(SourceRef source) {mSource = source;}
		/// Returns the SourceRef
		SourceRef getSource() {return mSource;}
		/// Convienance method to reset the sound state
		void initSource();
		/// Convienance method for creating buffers
		void generateBuffers();
		/// Calculates format info for the sounds. @note requires mFreq, mChannels and mBPS;
		void calculateFormatInfo();
		/// Queues buffers to be played
		void queueBuffers();
		/// Empties any queues that may still be active in the sound
		void unqueueBuffers();
		/// Loads the buffers to be played.  Returns whether the buffer is loaded.
		virtual bool loadBuffers() {return true;}
		/// Unloads the buffers.  Returns true if the buffers are still loaded.
		virtual bool unloadBuffers() {return true;}

		/// Returns the BufferRef for this sounds
		BufferRef getBufferRef() const {return mBuffers[0];}

		/// Lock states for sorting purposes. When sounds are being sorted in their
		/// lists, a comparator is used. This comparator utilises the sounds' states
		/// in order to compare pairs of sounds. To perform sorting, std::sort() is
		/// used, which requires that elements not change while sorting! Since by
		/// default, functions like isStopped() return the live state from OpenAL, it
		/// is possible (and has happened) that a SIGSEGV is the result of a sound
		/// stopping while the sound list is being sorted. We place a lock on sound
		/// states that makes them used cached values instead of querying OpenAL.
		bool isStateCached() const;
		void setStateCached(bool stateCached);
		/// Update the cached state. Does nothing if cached states are used. In other
		/// words, this function will only update the cached state if cached states
		/// are *not* in effect (being used [mStateCached == false]).
		void updateStateCache() const;

		/// Postion taking into account the parent node
		mutable Ogre::Vector3 mDerivedPosition;
		/// Direction taking into account the parent node
		mutable Ogre::Vector3 mDerivedDirection;
		/// Stored versions of parent orientation
		mutable Ogre::Quaternion mLastParentOrientation;
		/// Stored versions of parent position
		mutable Ogre::Vector3 mLastParentPosition;

		// User-defined category number.
		unsigned int mCategory;

		float mPitch;
		float mGain;
		float mMaxGain;
		float mMinGain;
		float mGainScale;
		float mMaxDistance;
		float mRolloffFactor;
		float mReferenceDistance;
		float mOuterConeGain;
		float mInnerConeAngle;
		float mOuterConeAngle;
		Ogre::Vector3 mPosition;
		Ogre::Vector3 mVelocity;
		Ogre::Vector3 mDirection;
		std::string mFileName;

		// State cache is being used?
		bool mStateCached;
		// Cached source state if we have a source. It is mutable to preserve the
		// const nature of the API. If it was not, all of the is(Playing/Stopped/...)
		// functions would have to have their ") const {" removed.
		mutable State mState;

		ALboolean mSourceRelative;
		ALboolean mLoop;
		mutable bool mLocalTransformDirty;

		Ogre::DataStreamPtr mSoundStream;

		SourceRef mSource;
		BufferRef *mBuffers;
		
		ALsizei mFreq;
		ALsizei mSize;
		ALsizei mChannels;
		ALsizei mBPS;
		BufferFormat mFormat;


		// Callbacks to alert the user of specific events
		MemberFunctionSlot *mFinishedCallback;
		MemberFunctionSlot *mLoopedCallback;
		// Sound listener list.
		typedef std::list<Listener*> ListenerList;
		ListenerList mListeners;

		// This allows us to start a sound back where is should be after being sacrificed
		time_t mStartTime;
		
		friend class SoundManager;
		friend class SoundFactory;

	private:
		/// Perform an update.
		void _update() const;
		/// Perform an update of any active fades.
		void _updateFading();

		enum FadeMode
		{
			FADE_NONE,
			FADE_IN,
			FADE_OUT
		};

		FadeMode    mFadeMode;
		float  mFadeTime;
		float  mRunning;
	};

	/** Factory object for creating sounds */
	class SoundFactory : public Ogre::MovableObjectFactory
	{
	protected:
	  // This class allows for buffer information storage and usage tracking on a
	  // per-buffer, shared buffer, basis.
	  class BufferInfo {
	  public:
	    // The buffer reference.
	    BufferRef reference;

	    // The number of buffer users.
	    // The previous system relied on each sound attempting to destroy the buffer
	    // upon destruction and, if successful, telling the factory to remove the 
	    // buffer reference. The problem with this approach was that buffer deletions
	    // are delayed by being stored in a queue and only being performed in the 
	    // next cycle. What can happen is a sound gets deleted that is the last to
	    // use that buffer, and gets queued in the deletion queue. Just after that, 
	    // but before the next update, a sound is created that uses the buffer. Sure
	    // enough, it piggy backs off of the existing buffer but does not yet queue
	    // it into the source; that happens at play(). Because the new sound did not
	    // queue the buffers, it is considered unused and at the next frame update
	    // the deletion of the buffer is successful. When the new sound attempts to
	    // be played, the buffer is no longer valid and an error ensues. To solve this
	    // we keep our own track of buffer usage.
	    unsigned int refCount;
	    
	    // Constructors and destructor.
	    BufferInfo();
	    BufferInfo(BufferRef reference, unsigned int refCount = 0u);
	    BufferInfo(const BufferInfo& bufferInfo);
	  };

	public:
		SoundFactory(SoundManager*_mSM);
		~SoundFactory();

		static std::string FACTORY_TYPE_NAME;

		const std::string& getType() const;
		void destroyInstance(Ogre::MovableObject* obj);

		void _removeBufferRef(const std::string& bufferName);
		void _addBufferRef(const std::string& bufferName, BufferRef buffer);
	private:
		SoundManager*mSM;

	protected:
		typedef std::map<std::string, BufferInfo> BufferMap;
		BufferMap mBufferMap;

		Ogre::MovableObject* createInstanceImpl(const std::string& name, const Ogre::NameValuePairList* params = 0);
	};
} // Namespace
#endif
