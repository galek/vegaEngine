/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

#include "OgreVector3.h"
#include "OgreColourValue.h"


__inline Ogre::ColourValue VectorToColour(const Ogre::Vector3& v)
{
	return Ogre::ColourValue(v.x, v.y, v.z);
}

__inline Ogre::Vector3 ColourToVector(const Ogre::ColourValue& c)
{
	return Ogre::Vector3(c.r, c.g, c.b);
}