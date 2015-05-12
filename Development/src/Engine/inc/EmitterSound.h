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
	class ENGINE_API EmitterSound : public Actor
	{
	public:
		EmitterSound(const std::string& name, const std::string& fileName, bool loop = false, bool stream = false);
		virtual ~EmitterSound();

		/** Plays the sound. */
		bool play();
		/** Returns true if the sound is playing, otherwise false. */
		bool isPlaying() const;
		/** Pauses the sound. @note Upon calling play again, the sound will resume where it left off */
		bool pause();
		/** Returns true if the sound is paused, otherwise false. */
		bool isPaused() const;
		/** Stops the sound. @note Upon calling play again, the sound will resume from the begining */
		bool stop();
		/** Returns true if the sound is stopped, otherwise false. */
		bool isStopped() const;
		/** Returns true if the source does not have a state yet, otherwise false */
		bool isInitial() const;
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
		float getPitch() const;
		/**
		* Sets the gain.
		* @param gain The gain where 1.0 is full volume and 0.0 is off
		* @note Negative values will be ignored
		*/
		void setGain(float gain);
		/** Returns the gain. */
		float getGain() const;
		/**
		* Sets the max gain.
		* @param maxGain The maximum amount of gain allowed for this source
		* @note Valid range is [0.0 - 1.0] all other values will be ignored
		*/
		void setMaxGain(float maxGain);
		/** Returns the max gain. */
		float getMaxGain() const;
		/**
		* Sets the min gain.
		* @param minGain The minimum amount of gain allowed for this source.
		* @note Valid range is [0.0 - 1.0] all other values will be ignored
		*/
		void setMinGain(float minGain);
		/** Returns the gain. */
		float getMinGain() const;
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
		float getMaxDistance() const;
		/**
		* Sets the rolloff factor.
		* @param rolloffFactor The rolloff rate for the source
		* @note Used for distance attenuation
		* @note Negative values will be ignored
		*/
		void setRolloffFactor(float rolloffFactor);
		/** Returns the rolloff factor */
		float getRolloffFactor() const;
		/**
		* Sets the reference distance.
		* @param refDistance The reference distance used in attenuation calculations.
		* @note This is the distance under which the volume for the
		*     source would normally drop by half
		* @note Negative values will be ignored
		*/
		void setReferenceDistance(float refDistance);
		/** Returns the reference distance. */
		float getReferenceDistance() const;
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
		const Ogre::Vector3& getVelocity() const;
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
		const Ogre::Vector3& getPosition() const;
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
		const Ogre::Vector3& getDirection() const;
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
		float getOuterConeGain() const;
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
		float getInnerConeAngle() const;
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
		float getOuterConeAngle();
		/** Sets the looping state */
		void setLoop(bool loop);
		/** Returns streaming state */
		bool isStreaming() const;
		/** Sets the priority of the sound */
		void setPriority(int priority);
		/** Returns the current Priority for the sound */
		int getPriority() const;
		/** Returns the duration of the audio in seconds */
		float getSecondDuration();
		/** Sets the offset within the audio stream in seconds. @note Negative values will be ignored */
		void setSecondOffset(float seconds);
		/** Returns the current offset within the audio stream in seconds */
		float getSecondOffset();
		/** Returns the position of the sound including any transform from nodes it is attached to. */
		const Ogre::Vector3& getDerivedPosition() const;
		/** Returns the direction of the sound including any transform from nodes it is attached to. */
		const Ogre::Vector3& getDerivedDirection() const;
		/** Returns the name of the file used to create this Sound. */
		const std::string& getFileName() const;
	private:
		iSound *sound;
	};
}