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
	/**
	*/
	//Forward Declaration for Bullet
	class DynamicsWorld;
	struct BaseActorInterface;
	/**
	*/
	struct IBody{
		virtual ~IBody(){}
		virtual IBody* createPrimitiveSphereCollision(BaseActorInterface *_actor, DynamicsWorld *world) = 0;;
		virtual IBody* createConvex(BaseActorInterface *_actor, DynamicsWorld *world) = 0;;
		virtual IBody* createPrimitiveCapsuleCollision(BaseActorInterface *_actor, DynamicsWorld *world) = 0;;
		virtual IBody* createStaticGeometry(BaseActorInterface *_actor, DynamicsWorld *world) = 0;;
		virtual IBody* createPrimitiveBoxCollision(BaseActorInterface *_actor, DynamicsWorld *world) = 0;
		virtual IBody* createRagdoll(BaseActorInterface *_actor) = 0;
	};
}