#include "AudioALException.h"
#include "AudioALListener.h"
#include "AudioALWavSound.h"

namespace vega {
	WavSound::WavSound(SoundManager*_mSoundManager,const std::string& name, const Ogre::DataStreamPtr& soundStream, bool loop, bool stream) :
		Sound(_mSoundManager,name, soundStream->getName(), stream)
	{
		try
		{
			 mSoundStream = soundStream;
			 mLoop = loop?AL_TRUE:AL_FALSE;

			// buffers
			char magic[5];
			magic[4] = '\0';
			unsigned char buffer32[4];
			unsigned char buffer16[2];

			// check magic
			CheckCondition(mSoundStream->read(magic, 4) == 4, 13, "Cannot read wav file " + mFileName);
			CheckCondition(std::string(magic) == "RIFF", 13, "Wrong wav file format. (no RIFF magic): " + mFileName);

			// The next 4 bytes are the file size, we can skip this since we get the size from the DataStream
			mSoundStream->skip(4);
			mSize = static_cast<ALsizei>(mSoundStream->size());

			// check file format
			CheckCondition(mSoundStream->read(magic, 4) == 4, 13, "Cannot read wav file " + mFileName);
			CheckCondition(std::string(magic) == "WAVE", 13, "Wrong wav file format. (no WAVE format): " + mFileName);

			// check 'fmt ' sub chunk (1)
			CheckCondition(mSoundStream->read(magic, 4) == 4, 13, "Cannot read wav file " + mFileName);
			CheckCondition(std::string(magic) == "fmt ", 13, "Wrong wav file format. (no 'fmt ' subchunk): " + mFileName);

			// read (1)'s size
			CheckCondition(mSoundStream->read(buffer32, 4) == 4, 13, "Cannot read wav file " + mFileName);
			unsigned long subChunk1Size = readByte32(buffer32);
			CheckCondition(subChunk1Size >= 16, 13, "Wrong wav file format. ('fmt ' chunk too small, truncated file?): " + mFileName);

			// check PCM audio format
			CheckCondition(mSoundStream->read(buffer16, 2) == 2, 13, "Cannot read wav file " + mFileName);
			unsigned short audioFormat = readByte16(buffer16);
			CheckCondition(audioFormat == 1, 13, "Wrong wav file format. This file is not a .wav file (audio format is not PCM): " + mFileName);

			// read number of channels
			CheckCondition(mSoundStream->read(buffer16, 2) == 2, 13, "Cannot read wav file " + mFileName);
			mChannels = readByte16(buffer16);

			// read frequency (sample rate)
			CheckCondition(mSoundStream->read(buffer32, 4) == 4, 13, "Cannot read wav file " + mFileName);
			mFreq = readByte32(buffer32);

			// skip 6 bytes (Byte rate (4), Block align (2))
			mSoundStream->skip(6);

			// read bits per sample
			CheckCondition(mSoundStream->read(buffer16, 2) == 2, 13, "Cannot read wav file " + mFileName);
			mBPS = readByte16(buffer16);

			// check 'data' sub chunk (2)
			CheckCondition(mSoundStream->read(magic, 4) == 4, 13, "Cannot read wav file " + mFileName);
			CheckCondition(std::string(magic) == "data" || std::string(magic) == "fact", 13,
				 "Wrong wav file format. (no data subchunk): " + mFileName);

			// fact is an option section we don't need to worry about
			if(std::string(magic) == "fact")
			{
				mSoundStream->skip(8);

				// Now we shoudl hit the data chunk
				CheckCondition(mSoundStream->read(magic, 4) == 4, 13, "Cannot read wav file " + mFileName);
				CheckCondition(std::string(magic) == "data", 13, "Wrong wav file format. (no data subchunk): " + mFileName);
			}

			// The next four bytes are the size remaing of the file
			CheckCondition(mSoundStream->read(buffer32, 4) == 4, 13, "Cannot read wav file " + mFileName);
			mDataSize = readByte32(buffer32);
			mDataStart = mSoundStream->tell();

			calculateFormatInfo();

			// mBufferSize is equal to 1/4 of a second of audio
			mLengthInSeconds = (float)mDataSize / ((float)mBufferSize * 4);

			generateBuffers();
			mBuffersLoaded = loadBuffers();
		}
		catch(Ogre::Exception e)
		  {
		    // If we have gone so far as to load the buffers, unload them.
		    if(mBuffers != NULL) {
		      for(int i = 0; i < mNumBuffers; i++)
			{
			  if (mBuffers[i] && alIsBuffer(mBuffers[i]) == AL_TRUE)
			    {
			      alDeleteBuffers(1, &mBuffers[i]);
			      CheckError(alGetError(), "Failed to delete Buffer");
			    }
			}
		    }

		    // Prevent the ~Sound() destructor from double-freeing.
		    mBuffers = NULL;
		    // Propagate.
		    throw (e);
		  }
	}

	WavSound::~WavSound()
	{}

	bool WavSound::loadBuffers()
	{
		for(int i = 0; i < mNumBuffers; i++)
		{
			CheckCondition(AL_NONE != mBuffers[i], 13, "Could not generate buffer");
			Buffer buffer = bufferData(mSoundStream, mStream?mBufferSize:mDataSize);
			alBufferData(mBuffers[i], mFormat, &buffer[0], static_cast<ALsizei>(buffer.size()), mFreq);
			CheckError(alGetError(), "Could not load buffer data");
		}

		return true;
	}

	bool WavSound::unloadBuffers()
	{
		if(mStream)
		{
			mSoundStream->seek(mDataStart);
			return false;
		}
		else
		{
			return true;
		}
	}

	void WavSound::setSecondOffset(float seconds)
	{
		if(seconds < 0) return;

		if(!mStream)
		{
			Sound::setSecondOffset(seconds);
		}
		else
		{
			bool wasPlaying = isPlaying();

			pause();

			// mBufferSize is 1/4 of a second
			size_t dataOffset = static_cast<size_t>(seconds * mBufferSize * 4);
			mSoundStream->seek(dataOffset + mDataStart);

			if(wasPlaying) play();
		}
	}

	float WavSound::getSecondOffset()
	{
		if(!mStream)
		{
			return Sound::getSecondOffset();
		}
		else
		{
			/*
			** We know that we are playing a buffer and that we have another buffer loaded.
			** We also know that each buffer is 1/4 of a second when full.
			** We can get the current offset in the OggStream which will be after both bufers
			** and subtract from that 1/4 of a second for the waiting buffer and 1/4 of a second
			** minus the offset into the current buffer to get the current overall offset.
			*/

			// mBufferSize is 1/4 of a second
			float wavStreamOffset = (float)(mSoundStream->tell() - mDataStart) / (float)(mBufferSize * 4);
			float bufferOffset = Sound::getSecondOffset();

			float totalOffset = wavStreamOffset + (0.25f - bufferOffset);
			return totalOffset;
		}
	}

	bool WavSound::updateSound()
	{
		// Call the parent method to update the position
		Sound::updateSound();

		bool eof = false;

		if(mStream && (mSource != AL_NONE) && isPlaying())
		{
			// Update the stream
			int processed;

			alGetSourcei(mSource, AL_BUFFERS_PROCESSED, &processed);
			CheckError(alGetError(), "Failed to get source");
		 
			while(processed--)
			{
				ALuint buffer;
		        
				alSourceUnqueueBuffers(mSource, 1, &buffer);
				CheckError(alGetError(), "Failed to unqueue buffers");
		 
				Buffer data = bufferData(mSoundStream, mBufferSize);
				alBufferData(buffer, mFormat, &data[0], static_cast<ALsizei>(data.size()), mFreq);

				eof = mSoundStream->eof();
		 
				alSourceQueueBuffers(mSource, 1, &buffer);
				CheckError(alGetError(), "Failed to queue buffers");

				if(eof)
				{
					if(mLoop)
					{
						eof = false;
						mSoundStream->seek(mDataStart);
						if(mLoopedCallback)
							mLoopedCallback->execute(static_cast<Sound*>(this));
					}
					else
					{
						stop();
						if(mFinishedCallback)
							mFinishedCallback->execute(static_cast<Sound*>(this));
					}
				}
			}
		}
	 
		return !eof;
	}

	Buffer WavSound::bufferData(Ogre::DataStreamPtr dataStream, int size)
	{
		size_t bytes;
		std::vector<char> data;
		char *array = new char[mBufferSize];

		while(data.size() != size)
		{
			// Read up to a buffer's worth of decoded sound data
			bytes = mSoundStream->read(array, mBufferSize);

			if (bytes <= 0)
				break;

			if (data.size() + bytes > size)
				bytes = size - data.size();

			// Append to end of buffer
			data.insert(data.end(), array, array + bytes);
		}

		delete []array;
		array = NULL;

		return data;
	}
} // Namespace
