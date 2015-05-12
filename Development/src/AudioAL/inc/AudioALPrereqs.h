#pragma once

#include "Ogre.h"

namespace vega{
	#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
	#	include "OpenAl\include\AL\al.h"
	#	include "OpenAl\include\AL\alc.h"
	// X-RAM Function pointer definitions
	typedef ALboolean (__cdecl *EAXSetBufferMode)(ALsizei n, ALuint *buffers, ALint value);
	typedef ALenum    (__cdecl *EAXGetBufferMode)(ALuint buffer, ALint *value);
#elif OGRE_COMPILER == OGRE_COMPILER_GNUC
#	include "OpenAl\include\AL\al.h"
#	include "OpenAl\include\AL\alc.h"
		// fake EAX/X-RAM
		typedef ALenum (*EAXGetBufferMode)(ALuint buffer, ALint *pReserved);
		typedef ALboolean (*EAXSetBufferMode)(ALsizei n, ALuint *buffers, ALint value);
	#else // Other Compilers
	#	include "al.h"    
	#	include "alc.h"
	#	include "xram.h"
	#endif

	#define OGREAL_AUTO_MUTEX_NAME ogrealMutex
	#	define OGREAL_AUTO_MUTEX
	#	define OGREAL_LOCK_AUTO_MUTEX
	
	enum AudioFormat
	{
		DEFAULT,
		MONO_CHANNEL,
		STEREO_CHANNEL,
		QUAD_CHANNEL,
		MULTI_CHANNEL_51,
		MULTI_CHANNEL_61,
		MULTI_CHANNEL_71
	};

	typedef ALenum BufferFormat;
	typedef ALenum State;
	typedef ALenum Error;
	typedef ALuint SourceRef;
	typedef ALuint BufferRef;
	typedef ALuint EAXMode;
	typedef ALchar* ALString;
	typedef std::vector<char> Buffer;
	typedef std::queue<SourceRef> SourcePool;

	/// Struct used with STL containers
	struct DeleteObject
	{
		template<typename T>
		void operator()(const T* ptr)const
		{
			delete ptr;
		}
	};

	/// Struct used with STL containers
	struct DeleteFirst
	{
		template<typename T1, typename T2>
		void operator()(std::pair<T1*, T2> pair)const
		{
			delete pair.first;
		}
	};

	/// Struct used with STL containers
	struct DeleteSecond
	{
		template<typename T1, typename T2>
		void operator()(std::pair<T1, T2*> pair)const
		{
			delete pair.second;
		}
	};

	/// Returns the next 16 bytes from a buffer
	static unsigned short readByte16(const unsigned char buffer[2])
	{
		#if(OGRE_ENDIAN == OGRE_ENDIAN_BIG)
			return (buffer[0] << 8) + buffer[1];
		#else
			return (buffer[1] << 8) + buffer[0];
		#endif
	}

	/// Returns the next 32 bytes from a buffer
	static unsigned long readByte32(const unsigned char buffer[4])
	{
		#if(OGRE_ENDIAN == OGRE_ENDIAN_BIG)
			return (buffer[0] << 24) + (buffer[1] << 16) + (buffer[2] << 8) + buffer[3];
		#else
			return (buffer[3] << 24) + (buffer[2] << 16) + (buffer[1] << 8) + buffer[0];
		#endif
	}
};