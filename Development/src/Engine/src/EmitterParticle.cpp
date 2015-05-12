/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
#include "EmitterParticle.h"


using namespace Ogre;

namespace vega
{
	/**
	*/
	EmitterParticle::EmitterParticle(const char *name,const char *_temp)
		:ps(nullptr)
	{
		ParticleSystem::setDefaultNonVisibleUpdateTimeout(5);  // set nonvisible timeout
		// create some nice fireworks and place it at the origin
		ps = GetEngine()->mGSceneMgr->createParticleSystem(name, _temp);
		attach(nullptr);
	}
	/**
	*/
	void EmitterParticle::attach(Actor *_mParent)
	{
		if(!_mParent)
			GetEngine()->mGSceneMgr->getRootSceneNode()->attachObject(ps);
		else
			_mParent->getNode()->attachObject(ps);
	}
	/**
	*/
	EmitterParticle::~EmitterParticle()
	{

	}
}