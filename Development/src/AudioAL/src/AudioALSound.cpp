#include "AudioALException.h"
#include "AudioALSound.h"
#include "AudioALOggSound.h"
#include "AudioALWavSound.h"
#include "AudioALSoundManager.h"

namespace vega 
{
	Sound::Listener::Listener() : stops(false), loops(false) {

	}

	Sound::Listener::~Listener() {

	}

	void Sound::Listener::soundStopped(Sound* sound) {

	}

	void Sound::Listener::soundLooped(Sound* sound) {

	}

	void Sound::Listener::soundAtOffset(Sound* sound, float requestedOffset, float actualOffset) {

	}

	Sound::Sound(SoundManager*_mSoundManager) :
	mSoundManager(_mSoundManager),
	mSource(0),
		mFormat(0),
		mFreq(0),
		mSize(0),
		mBPS(0),
		mChannels(0),
		mBuffers(0),
		mFinishedCallback(0),
		mLoopedCallback(0),
		mStartTime(0),
		mCategory(0u),
		mPitch(1.0),
		mGain(1.0),
		mMaxGain(1.0),
		mMinGain(0.0),
		mGainScale(1.0),
		mFadeMode(FADE_NONE),
		mFadeTime(0.0),
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
		mState(AL_INITIAL),
		mSourceRelative(AL_FALSE),
		mDerivedPosition(Ogre::Vector3::ZERO),
		mDerivedDirection(Ogre::Vector3::NEGATIVE_UNIT_Z),
		mLastParentOrientation(Ogre::Quaternion::IDENTITY),
		mLastParentPosition(Ogre::Vector3::ZERO)
	{
		mParentNode = NULL;
		mStream=false;
		mBuffersLoaded=false;
		mManualStop=false;
		mManualRestart=false;
		mBufferSize=0;
		mPreviousOffset=0;
		mLengthInSeconds=0;
	}

	Sound::Sound(SoundManager*_mSoundManager,const std::string &name, const std::string& fileName, bool stream) :
		mSoundManager(_mSoundManager),
			mSource(0),
		mFormat(0),
		mFreq(0),
		mSize(0),
		mBPS(0),
		mChannels(0),
		mBuffers(0),
		mFinishedCallback(0),
		mLoopedCallback(0),
		mStartTime(0),
		MovableObject(name),
		mCategory(0u),
		mPitch(1.0), mGain(1.0),
		mMaxGain(1.0),
		mMinGain(0.0),
		mGainScale(1.0),
		mFadeMode(FADE_NONE),
		mFadeTime(0.0),
		mMaxDistance(3400.0),
		mRolloffFactor(1.0),
		mReferenceDistance(150.0),
		mOuterConeGain(0.0),
		mInnerConeAngle(360.0),
		mOuterConeAngle(360.0),
		mPosition(Ogre::Vector3::ZERO),
		mVelocity(Ogre::Vector3::ZERO),
		mDirection(Ogre::Vector3::NEGATIVE_UNIT_Z),
		mFileName(fileName),
		mStateCached(false),
		mState(AL_INITIAL),
		mSourceRelative(AL_FALSE),
		mDerivedPosition(Ogre::Vector3::ZERO),
		mDerivedDirection(Ogre::Vector3::NEGATIVE_UNIT_Z),
		mLastParentOrientation(Ogre::Quaternion::IDENTITY),
		mLastParentPosition(Ogre::Vector3::ZERO)
	{
		mParentNode = NULL;
		mBuffersLoaded=false;
		mBufferSize=0;
		mPreviousOffset=0;
		mStream=stream;
		mNumBuffers=stream?2:1;
		mManualStop=false;
		mManualRestart=false;
		mLengthInSeconds=0;
	}


	Sound::Sound(SoundManager*_mSoundManager,BufferRef buffer, const std::string& name, const std::string& fileName, bool loop) :
	mSoundManager(_mSoundManager),
		mSource(0),
		mFormat(0),
		mFreq(0),
		mSize(0),
		mBPS(0),
		mChannels(0),
		mFinishedCallback(0),
		mLoopedCallback(0),
		mStartTime(0),
		MovableObject(name),
		mLoop(loop?AL_TRUE:AL_FALSE),
		mCategory(0u),
		mPitch(1.0), mGain(1.0),
		mMaxGain(1.0), mMinGain(0.0),
		mGainScale(1.0),
		mFadeMode(FADE_NONE),
		mFadeTime(0.0),
		mMaxDistance(3400.0),
		mRolloffFactor(1.0),
		mReferenceDistance(150.0),
		mOuterConeGain(0.0),
		mInnerConeAngle(360.0),
		mOuterConeAngle(360.0),
		mPosition(Ogre::Vector3::ZERO),
		mVelocity(Ogre::Vector3::ZERO),
		mDirection(Ogre::Vector3::NEGATIVE_UNIT_Z),
		mFileName(fileName),
		mStateCached(false),
		mState(AL_INITIAL),
		mSourceRelative(AL_FALSE),
		mDerivedPosition(Ogre::Vector3::ZERO),
		mDerivedDirection(Ogre::Vector3::NEGATIVE_UNIT_Z),
		mLastParentOrientation(Ogre::Quaternion::IDENTITY),
		mLastParentPosition(Ogre::Vector3::ZERO)
	{
		mParentNode = NULL;		
		mStream=false;
		mBuffersLoaded=true;
		mBufferSize=0;
		mPreviousOffset=0;
		mNumBuffers=1;
		mManualStop=false;
		mManualRestart=false;
		mLengthInSeconds=0;

		// Allocate buffer entry.
		mBuffers = new BufferRef[mNumBuffers];
		mBuffers[0] = buffer;
		// Notify the sound manager of our use of this buffer.
		mSoundManager->_addBufferRef(mFileName, buffer);

		alGetBufferi(mBuffers[0], AL_FREQUENCY, &mFreq);
		alGetBufferi(mBuffers[0], AL_BITS, &mBPS);
		alGetBufferi(mBuffers[0], AL_CHANNELS, &mChannels);
		alGetBufferi(mBuffers[0], AL_SIZE, &mSize);

		calculateFormatInfo();

		// mBufferSize is equal to 1/4 of a second of audio
		mLengthInSeconds = (float)mSize / (float)(mBufferSize * 4);
	}

	Sound::~Sound()
	{
		// We need to be stopped in order for the buffer to be releasable.
		stop();

		// The sound manager needs to also release the source of this sound
		// object if it was valid (it will figure that out). Put it back into
		// the source pool and all that.
		mSoundManager->_releaseSource(this);

		// Attempting to release buffers now that we've possibly decremented
		// the reference count by setting a null buffer on our source.
		try
		{
			// If we have buffers to deallocate, do so.
			if(mBuffers != NULL) {
				// Buffer deletion is dependent on streaming.
				if(mStream) {
					// We are streaming, so this buffer is all ours. In this case
					// we are responsible for the cleanup.
					alDeleteBuffers(mNumBuffers, mBuffers);
					CheckError(alGetError(), "Failed to delete a streamed buffer.");

				}else{
					// We are using a non-streamed buffer, so this buffer could very
					// well be shared among a number of sounds. We leave proper 
					// destruction and deletion to the sound factory.
					mSoundManager->_removeBufferRef(mFileName);
				}
			}
		}
		catch(...)
		{
			// Don't die because of this.
		}


		delete[] mBuffers;

	}

	unsigned int Sound::getCategory() const {
		return mCategory;
	}

	void Sound::setCategory(unsigned int category) {
		mCategory = category;
	}

	bool Sound::play()
	{
		// If we're already playing, bail out.
		if(isPlaying()) return true;

		if(!isPaused())
		{
			mManualRestart = false;
			mManualStop = false;		
			mPreviousOffset = 0;
		}

		if(mStartTime == 0)
		{
			time(&mStartTime);
		}
		else if(!isPaused())
		{
			time_t currentTime;
			time(&currentTime);

			setSecondOffset(currentTime - mStartTime);
		}

		if(mSource != AL_NONE || (mSource = mSoundManager->_requestSource(this)) != AL_NONE)
		{
			if(!mBuffersLoaded)
			{
				mBuffersLoaded = loadBuffers();
			}

			if(!mBuffersQueued)
			{
				// Unqueue any buffers that may be left over
				unqueueBuffers();
				queueBuffers();
			}

			initSource();

			alSourcePlay(mSource);
			CheckError(alGetError(), "Failed to play sound");
		}

		return false;
	}

	bool Sound::isPlaying() const
	{
		if(mSource == AL_NONE)
		{
			return false;
		}

		// Perform a cached state refresh. No refresh will be done if using cache.
		updateStateCache();

		return (mState == AL_PLAYING);
	}

	bool Sound::pause()
	{
		if(!isPlaying()) return true;

		if(mSource != AL_NONE)
		{
			alSourcePause(mSource);
			CheckError(alGetError(), "Failed to pause sound");
		}

		return false;
	}

	bool Sound::isPaused() const
	{
		if(mSource == AL_NONE)
		{
			return false;
		}

		// Perform a cached state refresh. No refresh will be done if using cache.
		updateStateCache();

		return (mState == AL_PAUSED);
	}

	bool Sound::stop()
	{
		if(isStopped())
		{
			if(mSource != AL_NONE)
			{
				mSource = mSoundManager->_releaseSource(this);
				return true;
			}
		}
		else if(mSource != AL_NONE)
		{
			mManualStop = true;

			// Stop the source
			alSourceStop(mSource);
			CheckError(alGetError(), "Failed to stop sound");

			unqueueBuffers();

			mBuffersLoaded = unloadBuffers();
		}

		mSource = mSoundManager->_releaseSource(this);

		mStartTime = 0;

		return true;
	}

	bool Sound::isStopped() const
	{
		if(mSource == AL_NONE)
		{
			return true;
		}

		// Perform a cached state refresh. No refresh will be done if using cache.
		updateStateCache();

		return (mState == AL_STOPPED);
	}

	bool Sound::isInitial() const
	{
		if(mSource == AL_NONE)
		{
			return true;
		}

		// Perform a cached state refresh. No refresh will be done if using cache.
		updateStateCache();

		return (mState == AL_INITIAL);
	}

	bool Sound::isFading() const {
		return mFadeMode != FADE_NONE;
	}

	bool Sound::fadeIn(float fadeTime) {
		// If we're playing, then we're going to fade-in from the current gain
		// value. This means that in order to perform the fade-in, we must also
		// not already be at the maximum gain. Also, this only makes sense while
		// we're fading out or not fading at all. I have not thought of a 
		// semantic where calling a fade-in while fading in is worthwhile.
		if(isPlaying() &&
			mGain != mMaxGain &&
			(mFadeMode == FADE_NONE || mFadeMode == FADE_OUT)) {
				// Swap fade direction.
				mFadeMode = FADE_IN;

				// What we're going to do is use the existing updateFading code by
				// modifying the fadeTime and running times so that, when the caller
				// has asked for a fade-in from the current gain to the max gain over
				// fadeTime, we create such a fadeTime and running time as to already
				// have arrived at these values.
				//
				// A                   B          C
				// |===================|----------|
				//
				// Consider A as our min-gain, B as our current gain and C as our
				// max-gain. If we are to fade in from B to C over fadeTime, then
				// this is essentially the same as fadeTime being scaled by the
				// ratio of AC/BC and running time being placed at B.
				mFadeTime = ((mMaxGain - mMinGain) / (mMaxGain - mGain)) * fadeTime;
				mRunning = ((mGain - mMinGain) / (mMaxGain - mMinGain)) * mFadeTime;

				return true;

		}else{
			// Not playing. Can safely assume the caller wants the sound faded in 
			// from minimum gain, rather than bursting it in and going from the
			// currently applied gain; this was the default behaviour.
			mFadeMode = FADE_IN;
			mFadeTime = fadeTime;
			mRunning = 0.0;
			// Start at min gain..
			setGain(mMinGain);
			// ..and play
			return play();
		}


		// A fade-in has been requested either:
		//
		//  * When a fade-in is already in progress.
		//  * When a sound is playing but is already at max-gain.
		//  * When a fade-out is in progress but we're at max-gain.
		//
		return false;
	}

	bool Sound::fadeOut(float fadeTime) {
		// The constructs in this fade-out code are extremely similar to that of
		// the fade-in code. For details about operation, consult the respective
		// function. The difference with fade outs is it only makes sense to do
		// one iff:
		//
		//  * We're playing the sound.
		//  * We're not at min-gain already.
		//  * A fade-out is not already in progress.
		//
		if(isPlaying() &&
			mGain != mMinGain &&
			(mFadeMode == FADE_NONE || mFadeMode == FADE_IN)) {
				// Swap fade direction.
				mFadeMode = FADE_OUT;

				// Construct fadeTime and running times so that they interpolate nicely
				// across the supplied time interval. This time we go from B to A.
				mFadeTime = ((mMaxGain - mMinGain) / (mGain - mMinGain)) * fadeTime;
				mRunning = ((mMaxGain - mGain) / (mMaxGain - mMinGain)) * mFadeTime;

				return true;

		}

		// A fade-out was requested while:
		//
		//  * The sound is not playing.
		//  * The sound is already at the minimum gain.
		//  * A fade-out is already in progress.
		return false;
	}

	bool Sound::cancelFade() {
		// If we have a running fade, turn it off.
		if(isFading()) {
			mFadeMode = FADE_NONE;
			return true;
		}

		// No fading was occuring.
		return false;
	}

	void Sound::setPitch(float pitch)
	{
		if(pitch <= 0) return;

		mPitch = pitch;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_PITCH, mPitch);
			CheckError(alGetError(), "Failed to set Pitch");
		}
	}

	void Sound::setGain(float gain)
	{
		if(gain < 0) return;

		mGain = gain;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_GAIN, mGain * mGainScale);
			CheckError(alGetError(), "Failed to set Gain");
		}
	}

	float Sound::getGainScale() const {
		return mGainScale;
	}

	void Sound::setGainScale(float gainScale) {
		// Set the new gain scale, clamping to [0,1].
		mGainScale = Ogre::Math::Clamp(gainScale, float(0.0), float(1.0));

		// Set the actual gain based on the current gain scale.
		setGain(mGain);
	}

	void Sound::setGainValues(float maxGain, float minGain, 
		float gain, float gainScale) {
			// Order is important here. First set up the boundary values.
			setMinGain(minGain);
			setMaxGain(maxGain);
			// Then set up the scaling factor.
			setGainScale(gainScale);
			// Now set the gain, which will be adjusted by the scaling factor.
			setGain(gain);
	}


	void Sound::setMaxGain(float maxGain)
	{
		if(maxGain < 0 || maxGain > 1) return;

		mMaxGain = maxGain;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_MAX_GAIN, mMaxGain);
			CheckError(alGetError(), "Failed to set Max Gain");
		}
	}

	void Sound::setMinGain(float minGain)
	{
		if(minGain < 0 || minGain > 1) return;

		mMinGain = minGain;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_MIN_GAIN, mMinGain);
			CheckError(alGetError(), "Failed to set Min Gain");
		}
	}

	void Sound::setMaxDistance(float maxDistance)
	{
		if(maxDistance < 0) return;

		mMaxDistance = maxDistance;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_MAX_DISTANCE, mMaxDistance);
			CheckError(alGetError(), "Failed to set Max Distance");
		}
	}

	void Sound::setRolloffFactor(float rolloffFactor)
	{
		if(rolloffFactor < 0) return;

		mRolloffFactor = rolloffFactor;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_ROLLOFF_FACTOR, mRolloffFactor);
			CheckError(alGetError(), "Failed to set Rolloff Factor");
		}
	}

	void Sound::setReferenceDistance(float refDistance)
	{
		if(refDistance < 0) return;

		mReferenceDistance = refDistance;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_REFERENCE_DISTANCE, mReferenceDistance);
			CheckError(alGetError(), "Failed to set Reference Distance");
		}
	}

	void Sound::setDistanceValues(float maxDistance, float rolloffFactor, float refDistance)
	{
		setMaxDistance(maxDistance);
		setRolloffFactor(rolloffFactor);
		setReferenceDistance(refDistance);
	}

	void Sound::setVelocity(float x, float y, float z)
	{
		mVelocity.x = x;
		mVelocity.y = y;
		mVelocity.z = z;

		if(mSource != AL_NONE)
		{
			alSource3f(mSource, AL_VELOCITY, mVelocity.x, mVelocity.y, mVelocity.z);
			CheckError(alGetError(), "Failed to set Velocity");
		}
	}

	void Sound::setVelocity(const Ogre::Vector3& vec)
	{
		setVelocity(vec.x, vec.y, vec.z);
	}

	void Sound::setRelativeToListener(bool relative)
	{
		// Do not set to relative if it's connected to a node
		if(mParentNode) return;

		mSourceRelative = relative;

		if(mSource != AL_NONE)
		{
			alSourcei(mSource, AL_SOURCE_RELATIVE, mSourceRelative);
			CheckError(alGetError(), "Failed to set Source Relative");
		}
	}

	void Sound::setOuterConeGain(float outerConeGain)
	{
		if(outerConeGain < 0 || outerConeGain > 1) return;

		mOuterConeGain = outerConeGain;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_CONE_OUTER_GAIN, mOuterConeGain);
			CheckError(alGetError(), "Failed to set Outer Cone Gain");
		}
	}

	void Sound::setInnerConeAngle(float innerConeAngle)
	{
		mInnerConeAngle = innerConeAngle;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_CONE_INNER_ANGLE, mInnerConeAngle);
			CheckError(alGetError(), "Failed to set Inner Cone Angle");
		}
	}

	void Sound::setOuterConeAngle(float outerConeAngle)
	{
		mOuterConeAngle = outerConeAngle;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_CONE_OUTER_ANGLE, mOuterConeAngle);
			CheckError(alGetError(), "Failed to set Outer Cone Angle");
		}
	}

	void Sound::setLoop(bool loop)
	{
		mLoop = loop?AL_TRUE:AL_FALSE;

		if(mSource != AL_NONE && !mStream)
		{
			alSourcei(mSource, AL_LOOPING, mLoop);
			CheckError(alGetError(), "Failed to set Looping");
		}
	}

	void Sound::setSecondOffset(float seconds)
	{
		if(seconds < 0) return;

		if(mSource != AL_NONE)
		{
			alSourcef(mSource, AL_SEC_OFFSET, seconds);
			CheckError(alGetError(), "Failed to set offset");
		}
	}

	float Sound::getSecondOffset()
	{
		float offset = 0;

		if(mSource != AL_NONE)
		{
			alGetSourcef(mSource, AL_SEC_OFFSET, &offset);
			CheckError(alGetError(), "Failed to set Looping");
		}

		return offset;
	}

	void Sound::addListener(Listener* listener) {
		// Attempt to find the listener in the list of listeners.
		ListenerList::iterator i = std::find(mListeners.begin(), mListeners.end(), listener);

		// Add the listener only if it's not already registered.
		if(i == mListeners.end()) {
			mListeners.push_back(listener);
		}
	}

	void Sound::removeListener(Listener* listener) {
		// Attempt to find the listener in the list of listeners.
		ListenerList::iterator i = std::find(mListeners.begin(), mListeners.end(), listener);

		// If it exists, remove it from the list.
		if(i != mListeners.end()) {
			mListeners.erase(i);
		}
	}


	void Sound::setPosition(float x, float y, float z)
	{
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;
		mLocalTransformDirty = true;
	}

	void Sound::setPosition(const Ogre::Vector3& vec)
	{
		mPosition = vec;
		mLocalTransformDirty = true;
	}

	void Sound::setDirection(float x, float y, float z)
	{
		mDirection.x = x;
		mDirection.y = y;
		mDirection.z = z;
		mLocalTransformDirty = true;
	}

	void Sound::setDirection(const Ogre::Vector3& vec)
	{
		mDirection = vec;
		mLocalTransformDirty = true;
	}

	void Sound::_update() const
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

	void Sound::_updateFading()
	{
		if(mFadeMode != FADE_NONE)
		{   
			mRunning += mSoundManager->_getLastDeltaTime();
			// Calculate volume between min and max Gain over fade time
			float delta = mMaxGain - mMinGain;
			float gain;

			if(mFadeMode == FADE_IN)
			{
				gain = mMinGain + (delta * mRunning / mFadeTime);
				// Clamp & stop if needed
				if (gain > mMaxGain)
				{
					gain = mMaxGain;
					mFadeMode = FADE_NONE;
				}
			}
			else if(mFadeMode == FADE_OUT)
			{
				gain = mMaxGain - (delta * mRunning / mFadeTime);
				// Clamp & stop if needed
				if(gain < mMinGain)
				{
					gain = mMinGain;
					mFadeMode = FADE_NONE;
				}
			}

			// Set the adjusted gain
			setGain(gain);
		}
	}

	bool Sound::updateSound()
	{
		_update();

		if(mSource != AL_NONE)
		{
			// If the sound isn't playing, but we have a source and the sound has
			// ended naturally (was not stopped by a call to stop()).
			if(isStopped() && !mManualStop) {
				// If we have a callback, notify the callback of the sound's halt.
				if(mFinishedCallback) {
					mFinishedCallback->execute(this);
				}

				// We have ended. Release the source for reuse.
				mSource = mSoundManager->_releaseSource(this);

				// To prevent constant re-firing of this event due to
				// the per-frame update nature of the Sound, we flip the
				// manual-stop flag. This flag is only used for callback
				// purposes.
				mManualStop = true;

				// The rest of the code concerns fades, which we don't need to 
				// process since we're stopped and now have no source to play from.
				return true;
			}

			// Get the current offset.
			float currOffset = getSecondOffset();
			// If we have looped around, because our current offset is less than
			// our previous, then we call the callback if we have one.
			if(isLooping() && currOffset < mPreviousOffset && !mManualRestart && mLoopedCallback) {
				mLoopedCallback->execute(this);
			}

			// Go through all listeners.
			for(ListenerList::iterator i = mListeners.begin(); i != mListeners.end(); ++i) {
				// Go through all registered offsets.
				for(Listener::OffsetList::iterator j = (*i)->offsets.begin(); j != (*i)->offsets.end(); ++j) {
					// Grab the requested offset.
					float reqOffset = *j;

					// Here's a bit of a hairy one. If the current offset is greater than
					// our previous offset, then we have simply advanced in the file normally.
					if(currOffset >= mPreviousOffset) {
						if(reqOffset > mPreviousOffset && reqOffset <= currOffset) {
							// Good to go. Notify the listener that we have reached or passed the
							// offset.
							(*i)->soundAtOffset(this, reqOffset, currOffset);
						}

						// Otherwise our current offset is less than the previous, in which
						// case we probably looped. Whatever the cause, we notify listeners
						// at or beyond the previous offset up until the duration, and from
						// the zero offset to the current; A and B in the diagram.
						//
						//   _____________________
						//  |                     |
						// |---->..............----|
						// AAAAAA              BBBBB
						//
					}else{
						if((reqOffset >= 0.0 && reqOffset <= currOffset) || // A
							(reqOffset > mPreviousOffset && reqOffset <= getSecondDuration())) { // B
								// We looped around. The logic of figuring our that we've looped
								// is up to the listener.
								(*i)->soundAtOffset(this, reqOffset, currOffset);
						}
					}

				}
			}

			// Done with these comparisons. Last offset becomes current for next frame.
			mPreviousOffset = currOffset;

			alSource3f(mSource, AL_POSITION, mDerivedPosition.x, mDerivedPosition.y, mDerivedPosition.z);
			CheckError(alGetError(), "Failed to set Position");

			alSource3f(mSource, AL_DIRECTION, mDerivedDirection.x, mDerivedDirection.y, mDerivedDirection.z);
			CheckError(alGetError(), "Failed to set Direction");

			// Fading
			_updateFading();
		}

		return true;
	}

	void Sound::initSource()
	{
		if(mSource == AL_NONE)
		{
			return;
		}

		alSourcef (mSource, AL_PITCH,				mPitch);
		alSourcef (mSource, AL_GAIN,				mGain * mGainScale);
		alSourcef (mSource, AL_MAX_GAIN,			mMaxGain);
		alSourcef (mSource, AL_MIN_GAIN,			mMinGain);
		alSourcef (mSource, AL_MAX_DISTANCE,		mMaxDistance);
		alSourcef (mSource, AL_ROLLOFF_FACTOR,		mRolloffFactor);
		alSourcef (mSource, AL_REFERENCE_DISTANCE,	mReferenceDistance);
		alSourcef (mSource, AL_CONE_OUTER_GAIN,		mOuterConeGain);
		alSourcef (mSource, AL_CONE_INNER_ANGLE,	mInnerConeAngle);
		alSourcef (mSource, AL_CONE_OUTER_ANGLE,	mOuterConeAngle);
		alSource3f(mSource, AL_POSITION,			mDerivedPosition.x, mDerivedPosition.y, mDerivedPosition.z);
		alSource3f(mSource, AL_VELOCITY,			mVelocity.x, mVelocity.y, mVelocity.z);
		alSource3f(mSource, AL_DIRECTION,			mDerivedDirection.x, mDerivedDirection.y, mDerivedDirection.z);
		alSourcei (mSource, AL_SOURCE_RELATIVE,		mSourceRelative);
		// There is an issue with looping and streaming, so we will
		// disable looping and deal with it on our own.
		alSourcei (mSource, AL_LOOPING,				mStream ? AL_FALSE : mLoop);
		CheckError(alGetError(), "Failed to initialize source");
	}

	void Sound::generateBuffers()
	{
		// Create the buffers.
		mBuffers = new BufferRef[mNumBuffers];
		alGenBuffers(mNumBuffers, mBuffers);
		CheckError(alGetError(), "Could not generate buffer");

		// If we are not streaming.
		if(!mStream) {
			// Notify the sound manager of our use of this buffer so that it can be shared.
			mSoundManager->_addBufferRef(mFileName, mBuffers[0]);
		}

		if(mSoundManager->xRamSupport())
		{
			mSoundManager->eaxSetBufferMode(mNumBuffers, mBuffers, SoundManager::xRamHardware);
		}
	}

	void Sound::calculateFormatInfo()
	{
		switch(mChannels)
		{
		case 1:
			if(mBPS == 8)
			{
				mFormat = AL_FORMAT_MONO8;
				// Set BufferSize to 250ms (Frequency divided by 4 (quarter of a second))
				mBufferSize = mFreq / 4;
			}
			else
			{
				mFormat = AL_FORMAT_MONO16;
				// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
				mBufferSize = mFreq >> 1;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				mBufferSize -= (mBufferSize % 2);
			}
			break;
		case 2:
			if(mBPS == 8)
			{
				mFormat = AL_FORMAT_STEREO16;
				// Set BufferSize to 250ms (Frequency * 2 (8bit stereo) divided by 4 (quarter of a second))
				mBufferSize = mFreq >> 1;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				mBufferSize -= (mBufferSize % 2);
			}
			else
			{
				mFormat = AL_FORMAT_STEREO16;
				// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
				mBufferSize = mFreq;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				mBufferSize -= (mBufferSize % 4);
			}
			break;
		case 4:
			mFormat = alGetEnumValue("AL_FORMAT_QUAD16");
			// Set BufferSize to 250ms (Frequency * 8 (16bit 4-channel) divided by 4 (quarter of a second))
			mBufferSize = mFreq * 2;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 8);
			break;
		case 6:
			mFormat = alGetEnumValue("AL_FORMAT_51CHN16");
			// Set BufferSize to 250ms (Frequency * 12 (16bit 6-channel) divided by 4 (quarter of a second))
			mBufferSize = mFreq * 3;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 12);
			break;
		case 7:
			mFormat = alGetEnumValue("AL_FORMAT_61CHN16");
			// Set BufferSize to 250ms (Frequency * 16 (16bit 7-channel) divided by 4 (quarter of a second))
			mBufferSize = mFreq * 4;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 16);
			break;
		case 8:
			mFormat = alGetEnumValue("AL_FORMAT_71CHN16");
			// Set BufferSize to 250ms (Frequency * 20 (16bit 8-channel) divided by 4 (quarter of a second))
			mBufferSize = mFreq * 5;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 20);
			break;
		default:
			// Couldn't determine buffer format so log the error and default to mono
			Ogre::LogManager::getSingleton().logMessage("!!WARNING!! Could not determine buffer format!  Defaulting to MONO");

			mFormat = AL_FORMAT_MONO16;
			// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
			mBufferSize = mFreq >> 1;
			// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
			mBufferSize -= (mBufferSize % 2);
			break;
		}
	}

	void Sound::queueBuffers()
	{
		alSourceQueueBuffers(mSource, mNumBuffers, mBuffers);
		CheckError(alGetError(), "Failed to bind Buffer");
	}

	void Sound::unqueueBuffers()
	{
		/*
		** If the sound doesn't have a state yet it causes an
		** error when you try to unqueue the buffers! :S  In 
		** order to get around this we stop the source even if
		** it wasn't playing.
		*/
		alSourceStop(mSource);
		CheckError(alGetError(), "Failed to stop sound");

		int queued;
		alGetSourcei(mSource, AL_BUFFERS_QUEUED, &queued);
		CheckError(alGetError(), "Failed to get Source");

		alSourceUnqueueBuffers(mSource, queued, mBuffers);
		CheckError(alGetError(), "Failed to unqueue Buffers");


		mBuffersQueued = false;
	}

	bool Sound::isStateCached() const {
		return mStateCached;
	}

	void Sound::setStateCached(bool stateCached) {
		mStateCached = stateCached;
	}

	void Sound::updateStateCache() const {
		// If we are not to use the cached state, in other words, if the cached state is
		// free to be used, modified and updated.
		if(!mStateCached) {
			// Perform a state cache update.
			alGetSourcei(mSource, AL_SOURCE_STATE, &mState);
			CheckError(alGetError(), "Failed to get State");
		}
	}

	const std::string& Sound::getMovableType(void) const
	{
		return SoundFactory::FACTORY_TYPE_NAME;
	}

	const Ogre::AxisAlignedBox& Sound::getBoundingBox(void) const
	{
		// Null, Sounds are not visible
		static Ogre::AxisAlignedBox box;
		return box;
	}

	void Sound::_updateRenderQueue(Ogre::RenderQueue* queue)
	{
		// Sounds are not visible so do nothing
	}

	void Sound::_notifyAttached(Ogre::Node *parent, bool isTagPoint)
	{
		// Set the source not relative to the listener if it's attached to a node
		if(mSourceRelative)
		{
			mSourceRelative = false;
			if(mSource != AL_NONE)
			{
				alSourcei(mSource, AL_SOURCE_RELATIVE, AL_FALSE);
				CheckCondition(alGetError() == AL_NO_ERROR, 13, "Inalid Value");
			}
		}
		mParentNode = parent;
		_update();
	}

	//-----------------SoundFactory-----------------//

	SoundFactory::BufferInfo::BufferInfo() : reference(AL_NONE),
		refCount(0u) {

	}

	SoundFactory::BufferInfo::BufferInfo(BufferRef reference, unsigned int refCount) : reference(reference),
		refCount(refCount) {

	}

	SoundFactory::BufferInfo::BufferInfo(const BufferInfo& bufferInfo) : reference(bufferInfo.reference),
		refCount(bufferInfo.refCount) {

	}

	// Default sound factory name for ogre registration and by-name factory retrieval.
	std::string SoundFactory::FACTORY_TYPE_NAME = "vega_Sound";

	SoundFactory::SoundFactory(SoundManager*_mSM):mSM(_mSM) {

	}

	SoundFactory::~SoundFactory()
	{
		BufferMap::iterator bufferItr = mBufferMap.begin();
		while(bufferItr != mBufferMap.end()) {
			alDeleteBuffers(1, &bufferItr->second.reference);
			++bufferItr;
		}

		mBufferMap.clear();
	}

	const std::string& SoundFactory::getType(void) const
	{
		return FACTORY_TYPE_NAME;
	}

	Ogre::MovableObject* SoundFactory::createInstanceImpl(const std::string& name, 
		const Ogre::NameValuePairList* params) {
			// Get the file name of the requested sound file to be sourced.
			std::string fileName = params->find(SoundManager::SOUND_FILE)->second;
			// Loop flag.
			bool loop = Ogre::StringConverter::parseBool(params->find(SoundManager::LOOP_STATE)->second);
			// Are we streaming or loading the whole thing?
			bool stream = Ogre::StringConverter::parseBool(params->find(SoundManager::STREAM)->second);

			// Check to see if we can just piggy back another buffer.
			if(!stream) {
				// Attempt to find the buffer by name.
				BufferMap::iterator bufferItr = mBufferMap.find(fileName);
				// If we have found the buffer.
				if(bufferItr != mBufferMap.end()) {
					// We have this buffer loaded already!
					return new Sound(mSM,(BufferRef)bufferItr->second.reference, name, fileName, loop);
				}
			}

			// By this stage we are either streaming and need to load a new copy of the file or
			// we are not streaming but do not have the buffer. Open the file.
			Ogre::ResourceGroupManager *groupManager = Ogre::ResourceGroupManager::getSingletonPtr();
			std::string group = groupManager->findGroupContainingResource(fileName);
			Ogre::DataStreamPtr soundData = groupManager->openResource(fileName, group);

			// If we identify the file as an OGG sound, create an OGG instance.
			if(fileName.find(".ogg") != std::string::npos || fileName.find(".OGG") != std::string::npos) {
				return new OggSound(mSM,name, soundData, loop, stream);

				// If we identify the file as a wave file. , create a WAV instance.
			}else if(fileName.find(".wav") != std::string::npos || fileName.find(".WAV") != std::string::npos) {
				return new WavSound(mSM,name, soundData, loop, stream);

				// Unknown file type.
			}else{
				throw Ogre::Exception(Ogre::Exception::ERR_INVALIDPARAMS,
					"Sound file '" + fileName + "' is of an unsupported file type, ",
					"SoundManager::_createSound");
			}
	}

	void SoundFactory::destroyInstance(Ogre::MovableObject* obj) {
		delete obj;
	}

	void SoundFactory::_removeBufferRef(const std::string& bufferName) {
		// Attempt to find the buffer requested.
		BufferMap::iterator bufferItr = mBufferMap.find(bufferName);
		// If the buffer information exists in our map.
		if(bufferItr != mBufferMap.end()) {
			// The reference count is unsigned, we have to take care not to flip it.
			if(bufferItr->second.refCount > 0) {
				// Above zero. Buffer unused, decrement count.
				bufferItr->second.refCount--;
			}else{
				// That's not good! Reference count zero and we got a decrement =/.
				Ogre::LogManager::getSingleton().logMessage("vega: Internal Error: Reference count decrement on zero buffer.");
			}

			// If our reference count is zero, delete the buffer.
			if(bufferItr->second.refCount == 0u) {
				// First delete it OpenAL style.
				alDeleteBuffers(1, &(bufferItr->second.reference));
				CheckError(alGetError(), "Failed to delete non-stream buffer with zero reference count.");
				// Then delete it out of our container.
				mBufferMap.erase(bufferItr);
			}

			// Oh ohes! Someone requested a buffer refcount removal without an entry.
		}else{
			Ogre::LogManager::getSingleton().logMessage("vega: Internal Error: Reference count decrement on non-existent buffer.");
		}
	}

	void SoundFactory::_addBufferRef(const std::string& bufferName, BufferRef buffer)
	{
		// Attempt to find the buffer.
		BufferMap::iterator bufferItr = mBufferMap.find(bufferName);
		// If we have found the buffer.
		if(bufferItr != mBufferMap.end()) {
			// Then the reference in that buffer should be the same as that supplied!
			if(bufferItr->second.reference != buffer) 
				// Sweet jesus, something is horribly wrong.
				Ogre::LogManager::getSingleton().logMessage("vega: Internal Error: Reference mismatch in reference count incrementation.");
			

			// Cool, increment reference count.
			bufferItr->second.refCount++;

		}else
			// No buffer reference by that name. Add it with an initial count of 1.
			mBufferMap[bufferName] = BufferInfo(buffer, 1u);
	}
} // Namespace
