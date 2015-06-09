#include "openal_config.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "alError.h"
#include "alMain.h"
#include "alFilter.h"
#include "alEffect.h"
#include "alAuxEffectSlot.h"
#include "alSource.h"
#include "alBuffer.h"
#include "AL/al.h"
#include "AL/alc.h"


const struct EffectList EffectList[] = {
    { "eaxreverb",  EAXREVERB,  "AL_EFFECT_EAXREVERB",      AL_EFFECT_EAXREVERB },
    { "reverb",     REVERB,     "AL_EFFECT_REVERB",         AL_EFFECT_REVERB },
#if 0
    { "autowah",    AUTOWAH,    "AL_EFFECT_AUTOWAH",        AL_EFFECT_AUTOWAH },
#endif
    { "chorus",     CHORUS,     "AL_EFFECT_CHORUS",         AL_EFFECT_CHORUS },
    { "compressor", COMPRESSOR, "AL_EFFECT_COMPRESSOR",     AL_EFFECT_COMPRESSOR },
    { "distortion", DISTORTION, "AL_EFFECT_DISTORTION",     AL_EFFECT_DISTORTION },
    { "echo",       ECHO,       "AL_EFFECT_ECHO",           AL_EFFECT_ECHO },
    { "equalizer",  EQUALIZER,  "AL_EFFECT_EQUALIZER",      AL_EFFECT_EQUALIZER },
    { "flanger",    FLANGER,    "AL_EFFECT_FLANGER",        AL_EFFECT_FLANGER },
    { "modulator",  MODULATOR,  "AL_EFFECT_RING_MODULATOR", AL_EFFECT_RING_MODULATOR },
    { "dedicated",  DEDICATED,  "AL_EFFECT_DEDICATED_LOW_FREQUENCY_EFFECT", AL_EFFECT_DEDICATED_LOW_FREQUENCY_EFFECT },
    { "dedicated",  DEDICATED,  "AL_EFFECT_DEDICATED_DIALOGUE", AL_EFFECT_DEDICATED_DIALOGUE },
    { NULL, 0, NULL, (ALenum)0 }
};


AL_API ALboolean AL_APIENTRY alIsExtensionPresent(const ALchar *extName)
{
    ALboolean ret = AL_FALSE;
    ALCcontext *context;
    const char *ptr;
    size_t len;

    context = GetContextRef();
    if(!context) return AL_FALSE;

    if(!(extName))
        SET_ERROR_AND_GOTO(context, AL_INVALID_VALUE, done);

    len = strlen(extName);
    ptr = context->ExtensionList;
    while(ptr && *ptr)
    {
        if(_strnicmp(ptr, extName, len) == 0 &&
           (ptr[len] == '\0' || isspace(ptr[len])))
        {
            ret = AL_TRUE;
            break;
        }
        if((ptr=strchr(ptr, ' ')) != NULL)
        {
            do {
                ++ptr;
            } while(isspace(*ptr));
        }
    }

done:
    ALCcontext_DecRef(context);
    return ret;
}


AL_API ALvoid* AL_APIENTRY alGetProcAddress(const ALchar *funcName)
{
    if(!funcName)
        return NULL;
    return alcGetProcAddress(NULL, funcName);
}

AL_API ALenum AL_APIENTRY alGetEnumValue(const ALchar *enumName)
{
    if(!enumName)
        return (ALenum)0;
    return alcGetEnumValue(NULL, enumName);
}