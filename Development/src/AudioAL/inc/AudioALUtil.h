#ifndef _OGREAL_UTIL_H_
#define _OGREAL_UTIL_H_

#include "AudioALPrereqs.h"

namespace vega {
	// Predeclare so we don't need to include AudioALSound.h
	class Sound;

	class MemberFunctionSlot
	{
	public:
		virtual ~MemberFunctionSlot(){};
		virtual void execute(Sound* source) = 0;
	};

	template<typename T>
	class MemberFunctionPointer : public MemberFunctionSlot
	{
	public:
		typedef void (T::*MemberFunction)(Sound* source);

		MemberFunctionPointer() : mUndefined(true){}

		MemberFunctionPointer(MemberFunction func, T* obj) :
			mFunction(func),
			mObject(obj),
			mUndefined(false)
		{}

		virtual ~MemberFunctionPointer(){}

		void execute(Sound* source)
		{
			if(!mUndefined)
				(mObject->*mFunction)(source);
		}

	protected:
		MemberFunction mFunction;
		T* mObject;
		bool mUndefined;
	};
} // Namespace
#endif
