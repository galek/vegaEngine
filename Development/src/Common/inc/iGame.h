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
	class iGame
	{
	public:
		virtual void			Initialize()=0;
		virtual void			CreateScene()=0;
		virtual void			Update(float _time)=0;
	};
};