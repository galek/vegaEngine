#include "stdafx.h"	

namespace vega
{
	unsigned int GetCountLights()
	{
		Ogre::LightList list = GetEngine()->mGSceneMgr->_getLightsAffectingFrustum();
		return (unsigned int)list.size();
	}
}