/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#pragma once
#include "renderdllex.h"

namespace vega
{
	class RENDER_API WaterSystem
	{
	public:
		explicit WaterSystem(Render*_render);
		void Init(const std::string &_profile="Default.wsf");
		void Update(float evt);
	private:
		Render*render;
	};
}