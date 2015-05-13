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
	class iGame
	{
	public:
		virtual void PreInitialize() = 0;
		virtual void CreateScene()=0;
		virtual void Update(float _time)=0;
	};
};