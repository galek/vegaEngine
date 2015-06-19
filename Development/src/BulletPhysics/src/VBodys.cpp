#include "..\..\Common\inc\IEngine.h"
#include "..\..\Core\inc\CorePrivate.h"
#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "Shapes\OgreBulletCollisionsBoxShape.h"
#include "Shapes\OgreBulletCollisionsCapsuleShape.h"
#include "Shapes\OgreBulletCollisionsCompoundShape.h"
#include "Shapes\OgreBulletCollisionsConeShape.h"
#include "Shapes\OgreBulletCollisionsCylinderShape.h"
#include "Shapes\OgreBulletCollisionsGImpactShape.h"
#include "Shapes\OgreBulletCollisionsMinkowskiSumShape.h"
#include "Shapes\OgreBulletCollisionsMultiSphereShape.h"
#include "Shapes\OgreBulletCollisionsSphereShape.h"
#include "Shapes\OgreBulletCollisionsStaticPlaneShape.h"
#include "Shapes\OgreBulletCollisionsTerrainShape.h"
#include "Shapes\OgreBulletCollisionsTriangleShape.h"
#include "Shapes\OgreBulletCollisionsTrimeshShape.h"
#include "shapes\OgreBulletCollisionsConvexHullShape.h"
#include "Utils\OgreBulletCollisionsMeshToShapeConverter.h"
#include "Utils\OgreBulletUtils.h"
#include "VBulletPhysics.h"
#include "VBodys.h"


namespace vega
{

	/**
	*/
	IBody* VBulletPhysics::createPrimitiveBoxCollision(BaseActorInterface *_actor)
	{
		VBody *body = new VBody(this);
		return body->createPrimitiveBoxCollision(_actor, mWorld);
	}

	/**
	*/
	IBody* VBulletPhysics::createConvex(BaseActorInterface *_actor)
	{
		VBody *body = new VBody(this);
		return body->createConvex(_actor, mWorld);
	}

	/**
	*/
	IBody* VBulletPhysics::createTriangle(BaseActorInterface *_actor)
	{
		TODO(__FUNCTION__);
		return nullptr;
	}

	/**
	*/
	IBody* VBulletPhysics::createPrimitiveSphereCollision(BaseActorInterface *_actor)	
	{
		VBody *body = new VBody(this);
		return body->createPrimitiveSphereCollision(_actor, mWorld);
	}

	/**
	*/
	IBody* VBulletPhysics::createStaticGeometry(BaseActorInterface *_actor)
	{
		VBody *body = new VBody(this);
		return body->createStaticGeometry(_actor, mWorld);
	}

	/**
	*/
	IBody* VBulletPhysics::createRagdoll(BaseActorInterface *_actor)
	{
		VBody *body = new VBody(this);
		body->createRagdoll(_actor);
		return body;
	}

	/**
	*/
	IBody* VBulletPhysics::createPrimitiveCapsuleCollision(BaseActorInterface *_actor)
	{
		VBody *body = new VBody(this);
		return body->createPrimitiveCapsuleCollision(_actor, mWorld);
	}

	/**
	*/
	VBody* VBody::createPrimitiveSphereCollision(BaseActorInterface *_actor, DynamicsWorld *world){
		SphereCollisionShape* chassisShape = new SphereCollisionShape(_actor->mRadius);
		body = new RigidBody(_actor->mName, world, _actor->mCollisionGroup, _actor->mCollisionMask);
		body->setShape(_actor->mNode, chassisShape, 1.0f, 1.0f, _actor->mMass, _actor->mPosition, _actor->mRotation);
		body->setDebugDisplayEnabled(true);
		return this;
	}

	/**
	*/
	VBody*VBody::createPrimitiveCapsuleCollision(BaseActorInterface *_actor, DynamicsWorld *world){
		CapsuleCollisionShape* chassisShape = new CapsuleCollisionShape(_actor->mRadius, _actor->mSize.y, _actor->mSize);
		body = new RigidBody(_actor->mName, world, _actor->mCollisionGroup, _actor->mCollisionMask);
		body->setShape(_actor->mNode, chassisShape, 1.0f, 1.0f, _actor->mMass, _actor->mPosition, _actor->mRotation);
		body->setDebugDisplayEnabled(true);
		return this;
	}

	/**
	*/
	VBody* VBody::createConvex(BaseActorInterface *_actor, DynamicsWorld *world){
		StaticMeshToShapeConverter * shapeConverter = new StaticMeshToShapeConverter(_actor->mEntity);

		ConvexHullCollisionShape * convexCollisionShape = shapeConverter->createConvex();
		body = new RigidBody(_actor->mName, world, _actor->mCollisionGroup, _actor->mCollisionMask);
		body->setShape(_actor->mNode, convexCollisionShape, 1.0f, 1.0f, _actor->mMass, _actor->mPosition, _actor->mRotation);
		return this;
	}

	/**
	*/
	VBody*	VBody::createStaticGeometry(BaseActorInterface *_actor, DynamicsWorld *world) {
		StaticMeshToShapeConverter *smtsc = new StaticMeshToShapeConverter(_actor->mEntity);

		TriangleMeshCollisionShape *tri = smtsc->createTrimesh();

		RigidBody *caveBody = new RigidBody(_actor->mName, world);
		caveBody->setStaticShape(tri, 0.1f, 0.8f, _actor->mPosition, _actor->mRotation);//Note:—уть,в том,что если мы укажем поворот
		//и позицию работать не будет
		caveBody->setDebugDisplayEnabled(true);

		//mShapes.push_back(tri);
		//mBodies.push_back(caveBody);
		return this;
	}

	/**
	*/
	VBody*VBody::createPrimitiveBoxCollision(BaseActorInterface *_actor, DynamicsWorld *world)
	{
		BoxCollisionShape* chassisShape = new BoxCollisionShape(_actor->mSize);
		body = new RigidBody(_actor->mName, world, _actor->mCollisionGroup, _actor->mCollisionMask);
		body->setShape(_actor->mNode, chassisShape, 0.1f, 0.8f, _actor->mMass, _actor->mPosition, _actor->mRotation);
		body->setDebugDisplayEnabled(true);
		return this;
	}


	/**
	*/
	IBody* VBulletPhysics::addObject(BaseActorInterface *_actor)
	{
		IBody* value = nullptr;

		switch (_actor->mCollisionModel)
		{
		case BaseActorInterface::ACTORMESH_COLLISION_STATIC:
			value = createStaticGeometry(_actor);
			break;
		case BaseActorInterface::ACTORMESH_COLLISION_PRBOX:
			value = createPrimitiveBoxCollision(_actor);
			break;
		case BaseActorInterface::ACTORMESH_COLLISION_PRSPHERE:
			value = createPrimitiveSphereCollision(_actor);
			break;
		case BaseActorInterface::ACTORMESH_COLLISION_PRCAPSULE:
			value = createPrimitiveCapsuleCollision(_actor);
			break;
		case BaseActorInterface::ACTORMESH_COLLISION_CONVEX:
			value = createConvex(_actor);
			break;
		case BaseActorInterface::ACTORMESH_COLLISION_RAGGDOL:
			value = createRagdoll(_actor);
			break;
		case BaseActorInterface::ACTORMESH_COLLISION_TRIANGLE:
			value = createTriangle(_actor);
			break;
		default:
			ErrorTraceName(_actor->mName);
		}
		return value;
	}


	/**
	*/
	VBody::VBody(VBulletPhysics*_ph)
		:body(nullptr),
		ph(_ph)
	{}
	/**
	*/
	VBody::~VBody()	{
		SAFE_DELETE(body);
	}
	/**
	*/
	void Ragdoll::create(BaseActorInterface *_actor)
	{
		Ogre::SceneManager*mSceneMgr = phys->GetSubS()->mGSceneMgr;
		DynamicsWorld *world = phys->GetWorld();
		Ogre::Entity*ragdollEntity = _actor->mEntity;//mSceneMgr->createEntity("Robot", "robot.mesh");
		Ogre::SceneNode*ragdollNode = _actor->mNode;// mSceneMgr->getRootSceneNode()->createChildSceneNode("ragdollNode");
		//Nick ragdollNode->attachObject(ragdollEntity);
		//Nick ragdollNode->setPosition(400, 500, 600);
		//  ragdollNode->yaw(Ogre::Degree(90));
		//  ragdollNode->pitch(Ogre::Degree(90));
		//   ragdollNode->roll(Ogre::Degree(45));
		ragdollEntity->getMesh()->_setBounds(Ogre::AxisAlignedBox(-9000, -9000, -9000, 9000, 9000, 90000));
		//создадим информацию о физическом скелете на осове анимационного
		Ogre::Skeleton *skeleton = ragdollEntity->getSkeleton();
		Ogre::Skeleton::BoneIterator it = skeleton->getBoneIterator();

		while (it.hasMoreElements())
		{
			Ogre::Bone *bone = it.getNext();

			if (bone->getParent() == 0) // корнева€ кость
			{
				bone->setManuallyControlled(true);
				structBoneInfo buffBoneInfo;
				buffBoneInfo.name = bone->getName();
				buffBoneInfo.type = "root";
				buffBoneInfo.baseGraphPos = bone->_getDerivedPosition();
				buffBoneInfo.basePhysPos = bone->_getDerivedPosition();
				buffBoneInfo.baseGraphOrient = bone->_getDerivedOrientation();
				buffBoneInfo.basePhysOrient = bone->_getDerivedOrientation();
				buffBoneInfo.mass = 10;//TODO:—делать массу кости
				buffBoneInfo.lenght = 1;

				boneInfo.push_back(buffBoneInfo);

				Ogre::LogManager::getSingleton().logMessage("Root bone: " + bone->getName());
			}
			else
			{
				if (bone->numChildren() == 0) //крайн€€ кость
				{
					Ogre::Vector3 posFrom = bone->getParent()->_getDerivedPosition();
					Ogre::Vector3 posTo = bone->_getDerivedPosition();
					Ogre::Vector3 direction = posTo - posFrom;

					direction.normalise();
					Ogre::Quaternion physOrient = Ogre::Vector3::UNIT_Y.getRotationTo(direction);

					structBoneInfo buffBoneInfo;
					buffBoneInfo.name = bone->getName();
					buffBoneInfo.type = "end";
					buffBoneInfo.baseGraphPos = bone->_getDerivedPosition();
					buffBoneInfo.basePhysPos = posTo;
					buffBoneInfo.baseGraphOrient = bone->_getDerivedOrientation();
					buffBoneInfo.basePhysOrient = physOrient;
					buffBoneInfo.mass = 10;
					buffBoneInfo.lenght = 0.1;

					boneInfo.push_back(buffBoneInfo);

					Ogre::LogManager::getSingleton().logMessage("bone end: " + bone->getName());
				}

				if (bone->numChildren() == 1) // обычна€ кость (одна дочерн€€)
				{
					bone->setManuallyControlled(true);
					Ogre::Vector3 posFrom = bone->_getDerivedPosition();
					Ogre::Vector3 posTo = bone->getChild(0)->_getDerivedPosition();
					Ogre::Vector3 physPos = (posFrom + posTo) / 2;

					Ogre::Vector3 direction = posTo - posFrom;
					Ogre::Real physLenght = direction.length();

					direction.normalise();
					Ogre::Quaternion physOrient = Ogre::Vector3::UNIT_Y.getRotationTo(direction);

					structBoneInfo buffBoneInfo;
					buffBoneInfo.name = bone->getName();
					buffBoneInfo.type = "normal";
					buffBoneInfo.baseGraphPos = bone->_getDerivedPosition();
					buffBoneInfo.basePhysPos = physPos;
					buffBoneInfo.baseGraphOrient = bone->_getDerivedOrientation();
					buffBoneInfo.basePhysOrient = physOrient;
					buffBoneInfo.mass = 10;
					buffBoneInfo.lenght = physLenght;

					boneInfo.push_back(buffBoneInfo);

					Ogre::LogManager::getSingleton().logMessage("Bone normal: " + bone->getName());
				}

				if (bone->numChildren() > 1) // сложна€ кость (более одной дочерней кости)
				{
					bone->setManuallyControlled(true);
					structBoneInfo buffBoneInfo;
					buffBoneInfo.name = bone->getName();
					buffBoneInfo.type = "multi";
					buffBoneInfo.baseGraphPos = bone->_getDerivedPosition();
					buffBoneInfo.basePhysPos = bone->_getDerivedPosition();
					buffBoneInfo.baseGraphOrient = bone->_getDerivedOrientation();
					buffBoneInfo.basePhysOrient = bone->_getDerivedOrientation();
					buffBoneInfo.mass = 10;
					buffBoneInfo.lenght = 0.1;

					boneInfo.push_back(buffBoneInfo);

					Ogre::LogManager::getSingleton().logMessage("Bone multi : " + bone->getName());
				}
			}
		}
		//========================================================================================================

		//создадим информацию о соединени€х

		Ogre::Skeleton::BoneIterator itt = skeleton->getBoneIterator();

		while (itt.hasMoreElements())
		{
			Ogre::Bone *boneB = itt.getNext();

			if (boneB->getParent())
			{
				Ogre::Node *boneA = boneB->getParent();

				structContactInfo buffContactInfo;
				buffContactInfo.nameA = boneA->getName();
				buffContactInfo.nameB = boneB->getName();

				unsigned int numberA = 0;
				unsigned int numberB = 0;
				for (unsigned int j = 0; j < boneInfo.size(); j++)
				{
					if (boneA->getName() == boneInfo[j].name)
						numberA = j;
					if (boneB->getName() == boneInfo[j].name)
						numberB = j;
				}

				buffContactInfo.locOffsetPosA = boneInfo[numberA].basePhysOrient.Inverse()* (boneInfo[numberB].baseGraphPos - boneInfo[numberA].basePhysPos);
				buffContactInfo.locOffsetPosB = -Ogre::Vector3::UNIT_Y * boneInfo[numberB].lenght / 2;

				Ogre::Quaternion contOrient(1.57, 0, 0, 1);
				buffContactInfo.locOffsetOrientA = boneInfo[numberA].basePhysOrient.Inverse() * ragdollNode->getOrientation().Inverse() * contOrient;
				buffContactInfo.locOffsetOrientB = boneInfo[numberB].basePhysOrient.Inverse() * ragdollNode->getOrientation().Inverse() * contOrient;

				contactInfo.push_back(buffContactInfo);
			}
		}
		//=========================================================================================================

		//создадим физ тело
		for (int i = 0; i < boneInfo.size(); i++)
		{
			btCollisionShape* buffShape = new btCapsuleShape(btScalar(3), boneInfo[i].lenght * 0.9);
			ragShapes.push_back(buffShape);

			btTransform offset; offset.setIdentity(); // “рансформаци€ нода модели
			offset.setOrigin(cvt(ragdollNode->getPosition()));
			offset.setRotation(cvt(ragdollNode->getOrientation()));

			btTransform transform;
			transform.setIdentity();
			transform.setOrigin(cvt(boneInfo[i].basePhysPos));
			transform.setRotation(cvt(boneInfo[i].basePhysOrient));

			btTransform startTransform;
			startTransform.setIdentity();
			startTransform = offset * transform;

			btVector3 localInertia(0, 0, 0);
			buffShape->calculateLocalInertia(boneInfo[i].mass, localInertia);

			btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

			btRigidBody::btRigidBodyConstructionInfo rbInfo(boneInfo[i].mass, myMotionState, buffShape, localInertia);

			btRigidBody* buffBody = new btRigidBody(rbInfo);
			ragBodies.push_back(buffBody);

			world->getBulletDynamicsWorld()->addRigidBody(buffBody, BaseActorInterface::ACTORMESH_COLLISION_RAGGDOL, _actor->mCollisionMask);

			buffBody->setGravity(btVector3(0, -100, 0));
			buffBody->setDamping(0.3, 0.85);
			buffBody->setDeactivationTime(0.45);
			buffBody->setSleepingThresholds(80.0, 80.0);
		}

		// ------------   создадим соединени€ -----------------------------
		for (int i = 0; i < contactInfo.size(); i++)
		{
			btTransform localA, localB;

			localA.setIdentity(); localB.setIdentity();
			localA.setOrigin(cvt(contactInfo[i].locOffsetPosA));
			localB.setOrigin(cvt(contactInfo[i].locOffsetPosB));
			localA.setRotation(cvt(contactInfo[i].locOffsetOrientA));
			localB.setRotation(cvt(contactInfo[i].locOffsetOrientB));

			unsigned int numberA = 0;
			unsigned int numberB = 0;
			for (unsigned int j = 0; j < boneInfo.size(); j++)
			{
				if (contactInfo[i].nameA == boneInfo[j].name)
				{
					numberA = j;
				}

				if (contactInfo[i].nameB == boneInfo[j].name)
				{
					numberB = j;
				}
			}

			btHingeConstraint* buffJoint = new btHingeConstraint(*ragBodies[numberA], *ragBodies[numberB], localA, localB);

			if (boneInfo[numberB].type == "end")
			{
				buffJoint->setLimit(btScalar(-0), btScalar(0));
			}
			else
			{
				buffJoint->setLimit(btScalar(-0.5), btScalar(0.5));
			}
			ragJoints.push_back(buffJoint);
			buffJoint->setDbgDrawSize(7);
			world->getBulletDynamicsWorld()->addConstraint(buffJoint, true);
		}
		Debug("[Ragdoll]«ј ќЌ„»Ћ»");
	}
	/**
	*/
	Ragdoll::Ragdoll(BaseActorInterface *_actor, VBulletPhysics*ph)
		:phys(ph),
		actor(_actor)
	{
		Debug("[Ragdoll]создаем тело");
		create(_actor);
		ph->addActorToList(this);
	}
	/**
	*/
	VBody* VBody::createRagdoll(BaseActorInterface *_actor){
		return new Ragdoll(_actor, ph);
	}

	//-----------------------------------------------------------------------------
	void Ragdoll::update(){
		//--------------- обновим графический скелет ragdoll по физике
		Ogre::Skeleton *skeleton = actor->mEntity->getSkeleton();
		Ogre::Skeleton::BoneIterator it = skeleton->getBoneIterator();

		while (it.hasMoreElements()) //переберем каждую граф кость
		{
			Ogre::Bone *bone = it.getNext();

			int number = 0;
			for (int j = 0; j < boneInfo.size(); j++) //найдем физ тело, соответствующее граф кости
			{
				if (bone->getName() == boneInfo[j].name && boneInfo[j].type != "end")
				{
					number = j;
					break;
				}
			}

			btTransform newPhysTrans;
			newPhysTrans.setIdentity();
			newPhysTrans = ragBodies[number]->getWorldTransform();

			Ogre::Quaternion newPhysOrient = cvt(newPhysTrans.getRotation());
			Ogre::Vector3 newPhysPos = cvt(newPhysTrans.getOrigin());

			Ogre::Vector3 newPhysDeriverdPos = actor->mNode->getOrientation().Inverse() * (newPhysPos - actor->mNode->getPosition());
			Ogre::Quaternion newPhysDerivedOrient = actor->mNode->getOrientation().Inverse() * newPhysOrient;

			Ogre::Vector3 newPhysDerivedDirection = newPhysDerivedOrient  * Ogre::Vector3::UNIT_Y;
			newPhysDerivedDirection.normalise();

			Ogre::Vector3 newGraphDerivedPos;
			Ogre::Quaternion newGraphDerivedOrient;
			newGraphDerivedPos = newPhysDeriverdPos - newPhysDerivedDirection * boneInfo[number].lenght / 2;
			newGraphDerivedOrient = newPhysDerivedOrient * boneInfo[number].baseGraphOrient * boneInfo[number].basePhysOrient.Inverse();

			if (bone->getParent() == 0) //корнева€ кость
			{
				bone->setPosition(newGraphDerivedPos);
				bone->setOrientation(newGraphDerivedOrient);
			}
			else
			{
				bone->_setDerivedPosition(newGraphDerivedPos);
				bone->_setDerivedOrientation(newGraphDerivedOrient);
			}
		}
	}
	//-----------------------------------------------------------------------------
}