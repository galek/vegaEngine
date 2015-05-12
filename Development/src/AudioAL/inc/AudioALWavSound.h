#ifndef _OGREAL_WAV_SOUND_H_
#define _OGREAL_WAV_SOUND_H_

#include <string>
#include <iostream>

#include "AudioALPrereqs.h"
#include "AudioALSound.h"

namespace vega {
	/**
	 * WavSound.
	 * @note
	 * This object is only to be instantiated using the SoundManager::createSound
	 * method.
	 * @remark This is a sound that plays Wav files
	 *
	 * @see Sound
	 */
	class WavSound : public Sound
	{
	protected:
		/*
		** Constructor is protected to enforce the use of the 
		** factory via SoundManager::createSound
		*/
		/**
		 * Constructor.
		 * @param name The name used to reference this sound
		 * @param soundStream Ogre::DataStreamPtr for the sound resource
		 * @param loop Should the sound loop once it has played
		 * @param stream Should the sound be streamed or all loaded into memory at once
		 */
		WavSound(SoundManager*_mSoundManager,const std::string& name, const Ogre::DataStreamPtr& soundStream, bool loop, bool stream);

	public:
		/** Standard Destructor. */
		virtual ~WavSound();

		/** Sets the offset within the audio stream in seconds */
		virtual void setSecondOffset(float seconds);
		/** Returns the current offset within the audio stream in seconds */
		virtual float getSecondOffset();

	protected:
		/// This is called each frame to update the position, direction, etc
		virtual bool updateSound();
		/// Loads the buffers to be played.  Returns whether the buffer is loaded.
		virtual bool loadBuffers();
		/// Unloads the buffers.  Returns true if the buffers are still loaded.
		virtual bool unloadBuffers();

	private:
		Buffer bufferData(Ogre::DataStreamPtr dataStream, int size);
		size_t mDataStart;
		unsigned long mDataSize;

		friend class SoundFactory;
	};
} // Namespace
#endif
