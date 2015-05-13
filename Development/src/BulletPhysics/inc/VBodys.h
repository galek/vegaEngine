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
		struct structBoneInfo  // ���������� � ������ �� ������� ����� ������� ���������� ����
		{
			Ogre::Vector3 basePhysPos;  // ������� ���. ����� � ������ ����������� Ragdoll (�������)
			Ogre::Vector3 baseGraphPos; // ������� ����������� ����� � ������ ����������� Ragdoll (�������)
			Ogre::Quaternion basePhysOrient; // ���������� ��� ����� � ������ ����������� Ragdoll (�������)
			Ogre::Quaternion baseGraphOrient; // ���������� ����������� ����� � ������ ����������� Ragdoll (�������)
			Ogre::String type;  // ��� ����� (root, normal, multi, end (��������, �������, �������, ��������))
			Ogre::String name;  // ��� �����
			Ogre::Real lenght;  // ������ ���. ���� �������������� � �����
			Ogre::Real mass; //�����
		};

		struct structContactInfo   // ���������� � ���������� ������
		{
			Ogre::String nameA; // ����� ����������� ������
			Ogre::String nameB; //
			Ogre::Vector3 locOffsetPosA; // ��������� ��������� ����� ����������
			Ogre::Vector3 locOffsetPosB; //
			Ogre::Quaternion locOffsetOrientA; // ��������� ��������� ���������� ����������
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