/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.6.0
* Copyright (C) 2009-2015 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once

namespace vega
{
	class ENGINE_API EmitterParticle:public Actor
	{
	public:
		EmitterParticle(std::string _name,const char *_temp);
		virtual ~EmitterParticle();
		//   Attaching to Node(SceneNode)
		void attach(Actor *_mParent);
	private:
		Ogre::ParticleSystem* ps;
	};
}