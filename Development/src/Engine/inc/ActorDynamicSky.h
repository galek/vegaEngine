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
	class SkyX;
	class BasicController;

	class ENGINE_API ActorDynamicSky :public Actor
	{
	public:
		ActorDynamicSky();
		~ActorDynamicSky();
		void Update(float evt);
		void AddCloudLayer();
		bool isValid(const char* _funct) const;
		EFORCEINLINE SkyX*GetSkyX() { return mSkyX; }
	private:
		SkyX* mSkyX;
		BasicController* mBC;
	};
}