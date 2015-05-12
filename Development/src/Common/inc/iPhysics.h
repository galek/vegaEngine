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
	struct IBody;
	struct BaseActorInterface;

	class iBasePhysics
	{
	public:
		virtual bool initialize()=0;
		/**
		Updates PhysSystem engine
		*/
		virtual bool update(float time)=0;
		virtual void destroy()=0;
		virtual void cleanScene()=0;
		virtual ~iBasePhysics(){};
		virtual IBody* addObject(BaseActorInterface *_actor) = 0;
	public:
		class iPhysicsBaseBodys *iPhBody;
	};
};