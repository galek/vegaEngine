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
	class Hydrax;
	class EngineGlobals;

	class ENGINE_API ActorWater :public Actor
	{
	public:
		ActorWater();
		virtual ~ActorWater();
		void AddWave(Ogre::Vector2 dir = Ogre::Vector2(0, 0), float A = 0.0f, float T = 0.0f, float p = 0.0f);
		void LoadCfg(const char _name);
		void Update(float evt);
		__inline Hydrax*GetHydrax(){ return water; }
	private:
		void _Init(const std::string &_profile);
	private:
		Hydrax*water;
		EngineGlobals*mG;
	};
}