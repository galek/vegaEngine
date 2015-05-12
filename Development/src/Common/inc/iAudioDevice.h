/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once


namespace vega
{
	struct iSound
	{
		iSound():
		mLengthInSeconds(0),
		mManualRestart(false),
		mManualStop(false),
		mPreviousOffset(0),
		mBufferSize(0),
		mBuffersLoaded(false),
		mBuffersQueued(false),
		mPriority(NORMAL),
		mStartTime(0),
		mStream(false),
		mCategory(0u),
		mPitch(1.0),
		mGain(1.0),
		mMaxGain(1.0),
		mMinGain(0.0),
		mGainScale(1.0),
		mMaxDistance(3400.0),
		mRolloffFactor(1.0),
		mReferenceDistance(150.0),
		mOuterConeGain(0.0),
		mInnerConeAngle(360.0),
		mOuterConeAngle(360.0),
		mPosition(Ogre::Vector3::ZERO),
		mVelocity(Ogre::Vector3::ZERO),
		mDirection(Ogre::Vector3::NEGATIVE_UNIT_Z),
		mFileName(""),
		mStateCached(false),
		mDerivedPosition(Ogre::Vector3::ZERO),
		mDerivedDirection(Ogre::Vector3::NEGATIVE_UNIT_Z),
		mLastParentOrientation(Ogre::Quaternion::IDENTITY),
		mLastParentPosition(Ogre::Vector3::ZERO)
		{};
		enum Priority
		{
			LOW,
			NORMAL,
			HIGH
		};
		/**
		@brief Retrieve the category number.

		This is a user-defined number that can be assigned to a 
		sound, but it's use is intended as a method of classifying
		sounds into groups. The meaning of this value is user defined.

		@return The user-assigned category number, or 0 by default.
		*/
		virtual unsigned int getCategory() const=0;

		/**
		@brief Set the category number.

		@see getCategory
		@param category The category number to assign to the sound.
		*/
		virtual void setCategory(unsigned int category)=0;

		/** Plays the sound. */
		virtual bool play()=0;
		/** Returns true if the sound is playing, otherwise false. */
		virtual bool isPlaying() const=0;
		/** Pauses the sound. @note Upon calling play again, the sound will resume where it left off */
		virtual bool pause()=0;
		/** Returns true if the sound is paused, otherwise false. */
		virtual bool isPaused() const=0;
		/** Stops the sound. @note Upon calling play again, the sound will resume from the begining */
		virtual bool stop()=0;
		/** Returns true if the sound is stopped, otherwise false. */
		virtual bool isStopped() const=0;
		/** Returns true if the source does not have a state yet, otherwise false */
		virtual bool isInitial() const=0;
		/** Returns true if the sound is currently in the process of fading. */
		virtual bool isFading() const=0;
		/** Starts playing the song while fading in.*/
		virtual bool fadeIn(float fadeTime)=0;
		/** Fades out, but keeps playing at volume 0, so it can be faded in again.*/
		virtual bool fadeOut(float fadeTime)=0;
		/** Cancel a processing fade operation. This leaves the gain at where the 
		fade operation was cancelled and does not touch it. */
		virtual bool cancelFade()=0;
		/**
		* Sets the pitch multiplier.
		* @param pitch The new pitch multiplier
		* @note pitch must always be positive non-zero, all other values will be ignored
		*/
		virtual void setPitch(float pitch)=0;
		/** Returns the pitch multiplier. */
		float getPitch() const {return mPitch;}
		/** 
		* Sets the gain. 
		* @param gain The gain where 1.0 is full volume and 0.0 is off
		* @note Negative values will be ignored
		*/
		virtual void setGain(float gain)=0;
		/** Returns the gain. */
		float getGain() const {return mGain;}
		/** 
		* Sets the max gain. 
		* @param maxGain The maximum amount of gain allowed for this source
		* @note Valid range is [0.0 - 1.0] all other values will be ignored
		*/
		virtual void setMaxGain(float maxGain)=0;
		/** Returns the max gain. */
		float getMaxGain() const {return mMaxGain;}
		/** 
		* Sets the min gain. 
		* @param minGain The minimum amount of gain allowed for this source.
		* @note Valid range is [0.0 - 1.0] all other values will be ignored
		*/
		virtual void setMinGain(float minGain)=0;
		/** Returns the gain. */
		float getMinGain() const {return mMinGain;}
		/**
		Retrieve the gain scale.

		@return The value of the gain scale in the range [0,1].
		*/
		virtual float getGainScale() const=0;
		/**
		@brief Set the gain scale.

		Gain scale is multiplied by the gain to yield final gain. 
		@param gainScale The gain scale value in the range [0,1].
		*/
		virtual void setGainScale(float gainScale)=0;
		/**
		* Set the min max and current gain.
		* @param maxGain The maximum amount of gain allowed for this source
		* @param minGain The minimum amount of gain allowed for this source
		* @param gain The current gain for this source
		* @param gainScale The gain scale factor in the range [0,1].
		* @note Gain should be positive
		*/
		virtual void setGainValues(float maxGain, float minGain, 
			float gain, float gainScale = 1.0)=0;
		/**
		* Sets the Max Distance.
		* @param maxDistance The max distance used in the Inverse Clamped Distance Model
		* @note This is the distance where there will be no further attenuation of the source
		* @note Negative values will be ignored
		*/
		virtual void setMaxDistance(float maxDistance)=0;
		/** Returns the max distance */
		float getMaxDistance() const {return mMaxDistance;}
		/**
		* Sets the rolloff factor.
		* @param rolloffFactor The rolloff rate for the source 
		* @note Used for distance attenuation
		* @note Negative values will be ignored
		*/
		virtual void setRolloffFactor(float rolloffFactor)=0;
		/** Returns the rolloff factor */
		float getRolloffFactor() const {return mRolloffFactor;}
		/**
		* Sets the reference distance.
		* @param refDistance The reference distance used in attenuation calculations.
		* @note This is the distance under which the volume for the
		*     source would normally drop by half
		* @note Negative values will be ignored
		*/
		virtual void setReferenceDistance(float refDistance)=0;
		/** Returns the reference distance. */
		float getReferenceDistance() const {return mReferenceDistance;}
		/**
		* Sets the variables used in the distance attenuation calculation.
		* @param maxDistance The max distance used in the Inverse Clamped Distance Model
		* @param rolloffFactor The rolloff rate for the source
		* @param refDistance The reference distance used in attenuation calculations
		*/
		virtual void setDistanceValues(float maxDistance, float rolloffFactor, float refDistance)=0;
		/**
		* Sets the velocity of the sound.
		* @param x The x part of the velocity vector
		* @param y The y part of the velocity vector
		* @param z The z part of the velocity vector
		*/
		virtual void setVelocity(float x, float y, float z)=0;
		/** Sets the velocity of the sound. @param vec The velocity vector */
		virtual void setVelocity(const Ogre::Vector3& vec)=0;
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
		virtual void setRelativeToListener(bool relative)=0;
		/**
		* Sets the position of the sound.
		* @param x The x part of the position
		* @param y The y part of the position
		* @param z The z part of the position
		* @note In the case that this sound is attached to a SceneNode this
		*     position become the offset from the parent scene node
		*/
		virtual void setPosition(float x, float y, float z)=0;
		/**
		* Sets the position of the sound.
		* @param vec The new postion for the sound.
		* @note In the case that this sound is attached to a SceneNode this
		*     position become the offset from the parent scene node
		*/
		virtual void setPosition(const Ogre::Vector3& vec)=0;
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
		virtual void setDirection(float x, float y, float z)=0;
		/**
		* Sets the direction of the sound.
		* @param vec The direction vector.
		* @note In the case that this sound is attached to a SceneNode this
		*     directions becomes relative to the parent's direction
		*/
		virtual void setDirection(const Ogre::Vector3& vec)=0;
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
		virtual void setOuterConeGain(float outerConeGain)=0;
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
		virtual void setInnerConeAngle(float innerConeAngle)=0;
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
		virtual void setOuterConeAngle(float outerConeAngle)=0;
		/** Returns the outerConeAngle */
		float getOuterConeAngle() {return mOuterConeAngle;}
		/** Sets the looping state */
		virtual void setLoop(bool loop)=0;
		/** Returns streaming state */
		bool isStreaming() const {return mStream==1?true:false;}
		/** Sets the priority of the sound */
		void setPriority(Priority priority) {mPriority = priority;}
		/** Returns the current Priority for the sound */
		Priority getPriority() const {return mPriority;}
		/** Returns the duration of the audio in seconds */
		float getSecondDuration() {return mLengthInSeconds;}
		/** Sets the offset within the audio stream in seconds. @note Negative values will be ignored */
		virtual void setSecondOffset(float seconds)=0;
		/** Returns the current offset within the audio stream in seconds */
		virtual float getSecondOffset()=0;
		/** Returns the position of the sound including any transform from nodes it is attached to. */
		const Ogre::Vector3& getDerivedPosition() const {return mDerivedPosition;}
		/** Returns the direction of the sound including any transform from nodes it is attached to. */
		const Ogre::Vector3& getDerivedDirection() const {return mDerivedDirection;}
		/** Returns the name of the file used to create this Sound. */
		const std::string& getFileName() const {return mFileName;}

		


		/** Overridden from MovableObject */
		virtual const std::string& getMovableType() const=0;
		/** Overridden from MovableObject */
		virtual const Ogre::AxisAlignedBox& getBoundingBox() const=0;
		/** Overridden from MovableObject */
		float getBoundingRadius() const {return 0; /* Not Visible */} 
		/** Overridden from MovableObject */
		virtual void _updateRenderQueue(Ogre::RenderQueue* queue)=0;
		/** Notifies the sound when it is attached to a node */
		virtual void _notifyAttached(Ogre::Node *parent, bool isTagPoint = false)=0;

		/** Overridden from MovableObject */
		virtual void visitRenderables(Ogre::Renderable::Visitor* visitor, bool debugRenderables = false){}
		/// Updates the sound if need be
		virtual bool updateSound()=0;
		/// Convienance method to reset the sound state
		virtual void initSource()=0;
		/// Convienance method for creating buffers
		virtual void generateBuffers()=0;
		/// Calculates format info for the sounds. @note requires mFreq, mChannels and mBPS;
		virtual void calculateFormatInfo()=0;
		/// Queues buffers to be played
		virtual void queueBuffers()=0;
		/// Empties any queues that may still be active in the sound
		virtual void unqueueBuffers()=0;
		/// Loads the buffers to be played.  Returns whether the buffer is loaded.
		virtual bool loadBuffers() {return true;}
		/// Unloads the buffers.  Returns true if the buffers are still loaded.
		virtual bool unloadBuffers() {return true;}

		/// Lock states for sorting purposes. When sounds are being sorted in their
		/// lists, a comparator is used. This comparator utilises the sounds' states
		/// in order to compare pairs of sounds. To perform sorting, std::sort() is
		/// used, which requires that elements not change while sorting! Since by
		/// default, functions like isStopped() return the live state from OpenAL, it
		/// is possible (and has happened) that a SIGSEGV is the result of a sound
		/// stopping while the sound list is being sorted. We place a lock on sound
		/// states that makes them used cached values instead of querying OpenAL.
		virtual bool isStateCached() const=0;
		virtual void setStateCached(bool stateCached)=0;
		/// Update the cached state. Does nothing if cached states are used. In other
		/// words, this function will only update the cached state if cached states
		/// are *not* in effect (being used [mStateCached == false]).
		virtual void updateStateCache() const=0;

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

		int mNumBuffers;
		int mBufferSize;
		bool mStream;
		bool mBuffersLoaded;
		bool mBuffersQueued;
		float mLengthInSeconds;

		bool mManualRestart;
		bool mManualStop;
		float mPreviousOffset;

		Priority mPriority;

		// This allows us to start a sound back where is should be after being sacrificed
		time_t mStartTime;
	};
	struct iAudioDevice
	{
		//Update Function
		virtual void updateSounds()=0;
		virtual iSound* createSound(const std::string& name, const std::string& fileName, bool loop = false, bool stream = false)=0;
		/** Returns the requested sound object. @param name The name of the sound to return */
		virtual iSound* getSound(const std::string& name) const=0;
		/** Returns true if the specified sound is loaded in the SoundManager. @param name The name of the sound to check for */
		virtual bool hasSound(const std::string& name) const=0;
		/** Destroys all sounds loaded in the SoundManager */
		virtual void destroyAllSounds() = 0;
		/** Pauses all sounds that are currently playing. */
		virtual void pauseAllSounds() = 0;
		/** Resumes all sounds that were paused with the previous call to pauseAllSounds(). */
		virtual void resumeAllSounds() = 0;
	};
};