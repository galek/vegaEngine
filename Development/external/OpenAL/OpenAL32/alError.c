#include "openal_config.h"

#include <signal.h>

#ifdef HAVE_WINDOWS_H
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "alMain.h"
#include "AL/alc.h"
#include "alError.h"

ALboolean TrapALError = AL_FALSE;

ALvoid alSetError(ALCcontext *Context, ALenum errorCode)
{
    ALenum curerr = AL_NO_ERROR;
    if(TrapALError)
    {
#ifdef _WIN32
        /* DebugBreak will cause an exception if there is no debugger */
        if(IsDebuggerPresent())
            DebugBreak();
#elif defined(SIGTRAP)
        raise(SIGTRAP);
#endif
    }
    ATOMIC_COMPARE_EXCHANGE_STRONG(ALenum, &Context->LastError, &curerr, errorCode);
}

AL_API ALenum AL_APIENTRY alGetError(void)
{
    ALCcontext *Context;
    ALenum errorCode;

    Context = GetContextRef();
    if(!Context)
    {
        if(TrapALError)
        {
#ifdef _WIN32
            if(IsDebuggerPresent())
                DebugBreak();
#elif defined(SIGTRAP)
            raise(SIGTRAP);
#endif
        }
        return AL_INVALID_OPERATION;
    }

    errorCode = ATOMIC_EXCHANGE(ALenum, &Context->LastError, AL_NO_ERROR);

    ALCcontext_DecRef(Context);

    return errorCode;
}