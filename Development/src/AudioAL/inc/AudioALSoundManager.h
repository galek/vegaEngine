#ifndef _OGREAL_SOUND_MANAGER_H_
#define _OGREAL_SOUND_MANAGER_H_

#include <algorithm>
#include <map>
#include <vector>

#include "OgreException.h"
#include "OgreIteratorWrappers.h"
#include "OgreString.h"

#include "AudioALListener.h"
#include "AudioALPrereqs.h"
#include "AudioALSound.h"


#include "..\..\Common\inc\iAudioDevice.h"
#ifndef __SINGLETON_H__
#include "..\..\Common\inc\Singleton.h"
#endif

namespace vega {
	/**
	 * FormatData.
	 * @remark This is a storage class to hold data about different
	 * OpenAL buffer formats.
	 */
	class FormatData
	{
	public:
		FormatData(BufferFormat alEnum, const std::string& alChar, const std::string& description)
		{
			formatEnum = alEnum;
			formatName = alChar;
			formatDescription = description;
		}
		/// OpenAL buffer type
		BufferFormat formatEnum;
		/// OpenAL buffer type name
		std::string formatName;
		/// Buffer type description
		std::string formatDescription;
	};

	typedef std::map<std::string, Sound*> SoundMap;
	typedef SoundMap::iterator SoundMapIter;
	typedef std::pair<SoundMapIter, SoundMapIter> SoundMapIterPair;
	typedef std::map<Sound*, float> SoundGainMap;
	typedef std::vector<Sound*> SoundList;
	typedef std::map<AudioFormat, FormatData*> FormatMap;
	typedef Ogre::MapIterator<FormatMap> FormatMapIterator;

	/**
	 * SoundManager.
	 * @remark This class is responsible for creating and managing all of the
	 *     sounds and listeners used in the application.  Sounds  and Listeners
	 *	   should only ever be created using the creation methods provided in this class.
	 *
	 * @see Ogre::Singleton
	 * @see Ogre::FrameListener
	 */
	class SoundManager : public iAudioDevice, public Singleton < SoundManager >
	{
	public:
		/**
		 * Constructor.
		 * @param deviceName An optional parameter that allows the user to suggest which device to use.
		 *		The list of valid devices can be obtained by calling SoundManager::getDeviceList()
		 */
		SoundManager(const std::string& deviceName = "", int maxNumSources = 100);
		/** Standard Destructor. */
		virtual ~SoundManager();

		void updateSounds();
		/**
		 * Creates a sound.  This is the only way sounds should be instantiated
		 * @param name The name used to refer to the sound
		 * @param fileName The name of the sound file to load
		 * @param loop Should the sound be looped once it has finished playing
		 * @param stream Should the sound be streamed or should the whole file be loaded into memory at once
		 * @return Returns a pointer to the newly created sound
		 */
		virtual iSound* createSound(const std::string& name, const std::string& fileName, bool loop = false, bool stream = false);
		/** Returns the requested sound object. @param name The name of the sound to return */
		virtual Sound* getSound(const std::string& name) const;
		/** Returns true if the specified sound is loaded in the SoundManager. @param name The name of the sound to check for */
		virtual bool hasSound(const std::string& name) const;
		/**
		   @brief Retrieve an iterator pair over all sounds.

		   @return A pair of iterators (beginning, end) over all sounds.
		   */
		SoundMapIterPair getSoundIterator();

		/** Destroys the specified sound. @param name The name of the sound to destroy */
		virtual void destroySound(const std::string& name);
		/** Destroys the specified sound. @param sound A pointer to the sound to destroy */
		virtual void destroySound(Sound* sound);
		/** Destroys all sounds loaded in the SoundManager */
		virtual void destroyAllSounds();
		/** Pauses all sounds that are currently playing. */
		virtual void pauseAllSounds();
		/** Resumes all sounds that were paused with the previous call to pauseAllSounds(). */
		virtual void resumeAllSounds();
		/** This is how you should get a pointer to the listener object. */
		virtual Listener* getListener() const;
		/**
		 * Sets the Doppler factor.
		 * The Doppler factor is a simple scaling factor of the source and listener
		 * velocities to exaggerate or deemphasize the Doppler (pitch) shift resulting
		 * from the Doppler Effect Calculations.
		 * @note Negative values will be ignored.
		 */
		void setDopplerFactor(float dopplerFactor);
		/** Returns the Doppler factor */
		float getDopplerFactor() const { return mDopplerFactor; }
		/**
		 * Sets the speed of sound used in the Doppler calculations.
		 * This sets the propagation speed used in the Doppler calculations.
		 * The default is 343.3 m/s (Speed of sound through air), but the
		 * units can be assigned to anything that you want as long as the
		 * velocities of the Sounds and Listener are expressed in the same units.
		 * @note Negative values will be ignored.
		 */
		void setSpeedOfSound(float speedOfSound);
		/** Returns the speed of sound */
		float getSpeedOfSound() const { return mSpeedOfSound; }

		/**
		   \brief Get the sound culling distance.

		   \return The sound culling distance. Values greater than 0 indicate that
		   sound culling is enabled. Values at or lower than 0 indicate culling is
		   disabled.
		   */
		float getCullDistance() const;

		/**
		   \brief Enable or disable sound culling.

		   This function enables or disables sound culling. Sound culling is the
		   cutting of gain after a certain distance. After the specified distance
		   between the listener and the given sound source, a sound's gain will be
		   stored and set to zero. This will render the sound as still playing in
		   the background, but inaudible. Given the listener return so as to be in
		   range of the sound (within the cull distance), the stored gain of the
		   sound will be restored.

		   \param distance The distance between the listener and and sound within
		   the system after which to cull sounds. Set this to any positive value
		   to enable culling. Set it to zero or any negative value to disable
		   the feature.
		   */
		void setCullDistance(float distance);


		/**
		 * Returns a list of all posible sound devices on the system.
		 * @note Versions of OpenAL prior to 1.0 do not support this feature
		 */
		static Ogre::StringVector getDeviceList();
		/** Returns an iterator for the list of supported buffer formats */
		FormatMapIterator getSupportedFormatIterator();
		/**
		 * Returns the FormatData containing information for the specified buffer format
		 * or NULL if the specified format is not found.
		 */
		const FormatData* retrieveFormatData(AudioFormat format) const;
		/** Returns the maximum number of sources allowed by the hardware */
		int maxSources() const { return mMaxNumSources; }
		/**
		 * Returns the version of EAX that is supported
		 * @note There is nothing implemented that uses EAX yet, I am just planning ahead.
		 */
		int eaxSupport() const { return mEAXVersion; }
		/** Returns true if X-Ram is supported, otherwise false */
		bool xRamSupport() const { return mXRAMSupport; }
		/** Sets the mode to be used by sound buffers if X-Ram is supported */
		ALboolean eaxSetBufferMode(ALsizei numBuffers, BufferRef *buffers, EAXMode bufferMode);
		/** Returns the eaxBufferMode if X-Ram is supported */
		ALenum eaxGetBufferMode(BufferRef buffer, ALint *reserved = 0);
		/** Removes a BufferRef from the BufferMap */
		void _removeBufferRef(const std::string& bufferName);
		/** Adds a BufferRef to the BufferMap to be used later */
		void _addBufferRef(const std::string& bufferName, BufferRef buffer);
		/** Used by the fading: returns the time since last frame. */
		float _getLastDeltaTime() const { return mLastDeltaTime; }
		/** Requests a dynamically allocated Source. */
		SourceRef _requestSource(Sound *sound);
		/** Releases a dynamically allocated Source. */
		SourceRef _releaseSource(Sound *sound);


		static const std::string FILE_TYPE;
		static const std::string OGG;
		static const std::string WAV;
		static const std::string SOUND_FILE;
		static const std::string LOOP_STATE;
		static const std::string STREAM;
		static const std::string AUDIO_FORMAT;

		static const BufferFormat xRamAuto;
		static const BufferFormat xRamHardware;
		static const BufferFormat xRamAccessible;

	protected:
		/// Frame Update callback
		bool Update(float _time);
		/// Translate the OpenAL error code to a string
		const std::string errorToString(int code) const;

		SoundFactory *mSoundFactory;
		ListenerFactory *mListenerFactory;

		bool mEAXSupport;
		int mEAXVersion;
		bool mXRAMSupport;
		bool mEFXSupport;
		int mSendsPerSource;

		float mDopplerFactor;
		float mSpeedOfSound;
		// Sound culling distance.
		float mCullDistance;

	private:
		int createSourcePool();
		void createListener();
		void initializeDevice(const std::string& deviceName);
		void checkFeatureSupport();
		void updateSourceAllocations();
		void performDeleteQueueCycle();

		// Sound culling
		void performSoundCull(Sound* sound);
		void cullSound(Sound* sound);
		void uncullSound(Sound* sound);
		void uncullSound(SoundGainMap::iterator& soundItr);

		struct UpdateSound;
		struct SortLowToHigh;
		struct SortHighToLow;

		int mMaxNumSources;
		FormatMap mSupportedFormats;

		int mMajorVersion;
		int mMinorVersion;

		SoundMap mSoundMap;
		// Sound culling map. Maps Sound* to stored Gain.
		SoundGainMap mSoundGainMap;
		SoundList mPauseResumeAll;
		SoundList mSoundsToDestroy;

		SourcePool mSourcePool;
		SoundList mActiveSounds;
		SoundList mQueuedSounds;

		ALCcontext *mContext;
		ALCdevice *mDevice;

		float mLastDeltaTime;

		// Mutex so we can protect against corruption
		OGREAL_AUTO_MUTEX;

		// XRAM Funtions Pointers
		ALsizei mXRamSize;
		ALsizei mXRamFree;

		EAXSetBufferMode mSetXRamMode;
		EAXGetBufferMode mGetXRamMode;
	};
} // Namespace
#endif
