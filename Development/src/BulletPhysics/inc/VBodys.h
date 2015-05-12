#pragma once

/**
*/
class btCollisionShape;
class btRigidBody;
class btTypedConstraint;
/**
*/
namespace vega
{
	/**
	*/
	class VBulletPhysics;
	class RigidBody;
	struct Ragdoll;
	/**
	*/
	struct VBody :public IBody{
		VBody(VBulletPhysics*ph = nullptr);
		virtual ~VBody();
		virtual VBody* createPrimitiveSphereCollision(BaseActorInterface *_actor, DynamicsWorld *world);
		virtual VBody* createConvex(BaseActorInterface *_actor, DynamicsWorld *world);
		virtual VBody* createPrimitiveCapsuleCollision(BaseActorInterface *_actor, DynamicsWorld *world);
		virtual VBody* createStaticGeometry(BaseActorInterface *_actor, DynamicsWorld *world);
		virtual VBody* createPrimitiveBoxCollision(BaseActorInterface *_actor, DynamicsWorld *world);
		virtual VBody* createRagdoll(BaseActorInterface *_actor);
	private:
		RigidBody*body;
		VBulletPhysics*ph;
	};
	/**
	*/
	struct Ragdoll :public VBody
	{
		struct structBoneInfo  // информация о костях по которой будем строить физические тела
		{
			Ogre::Vector3 basePhysPos;  // позиция физ. кости в момент составления Ragdoll (базовая)
			Ogre::Vector3 baseGraphPos; // позиция графической кости в момент составления Ragdoll (базовая)
			Ogre::Quaternion basePhysOrient; // ориентация физ кости в момент составления Ragdoll (базовая)
			Ogre::Quaternion baseGraphOrient; // ориентация графической кости в момент составления Ragdoll (базовая)
			Ogre::String type;  // тип кости (root, normal, multi, end (корневая, обычная, сложная, конечная))
			Ogre::String name;  // имя кости
			Ogre::Real lenght;  // длинна физ. тела прикрепленного к кости
			Ogre::Real mass; //масса
		};

		struct structContactInfo   // информация о соединении костей
		{
			Ogre::String nameA; // имена соединяемых костей
			Ogre::String nameB; //
			Ogre::Vector3 locOffsetPosA; // локальные отстройки места соединения
			Ogre::Vector3 locOffsetPosB; //
			Ogre::Quaternion locOffsetOrientA; // локальные отстройки ориентации соединения
			Ogre::Quaternion locOffsetOrientB; //
		};

		std::vector<structBoneInfo> boneInfo;
		std::vector<btCollisionShape*> ragShapes;
		std::vector<btRigidBody*> ragBodies;

		std::vector<structContactInfo> contactInfo;
		std::vector<btTypedConstraint*> ragJoints;
	public:
		Ragdoll(BaseActorInterface *_actor, VBulletPhysics*ph);
		virtual ~Ragdoll(){}
		void create(BaseActorInterface *_actor);
		virtual void update();
	private:
		Ragdoll(){}
		virtual VBody* createPrimitiveSphereCollision(BaseActorInterface *_actor, DynamicsWorld *world){ return NULL; }
		virtual VBody* createConvex(BaseActorInterface *_actor, DynamicsWorld *world){ return NULL; }
		virtual VBody* createPrimitiveCapsuleCollision(BaseActorInterface *_actor, DynamicsWorld *world){ return NULL; }
		virtual VBody* createStaticGeometry(BaseActorInterface *_actor, DynamicsWorld *world){ return NULL; }
		virtual VBody* createPrimitiveBoxCollision(BaseActorInterface *_actor, DynamicsWorld *world){ return NULL; }
		virtual Ragdoll* createRagdoll(BaseActorInterface *_actor){ return NULL; }
	private:
		VBulletPhysics*phys;
		BaseActorInterface *actor;
	};
}