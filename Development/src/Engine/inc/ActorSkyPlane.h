/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

namespace vega
{
	class ENGINE_API ActorSkyPlane:public Actor
	{
	public:
		ActorSkyPlane(std::string  _material = "DefMat");
		ActorSkyPlane(std::string  _material, const Ogre::Vector4 &_vec);
		ActorSkyPlane(std::string  _material, float _a, float _b, float _c, float _d);
		virtual ~ActorSkyPlane();
	};
}