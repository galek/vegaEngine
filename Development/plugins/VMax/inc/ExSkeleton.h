#ifndef _EXSKELETON_H
#define _EXSKELETON_H

#include "ExPrerequisites.h"
#include "ExOgreConverter.h"
#include "ExAnimation.h"

namespace VMax
{
	class ExBone
	{
	public:
		ExBone()
		{
			id = 0;
			pNode = 0;
			nodeID = 0;
			parentIndex = 0;
		}

		~ExBone()
		{
		}

		std::string name;
		int id;
		ULONG nodeID; //Unique Max INode ID ;
		INode* pNode;
		Matrix3 bindMatrix;
		int parentIndex;
		Point3 trans;
		Point3 scale;
		Quat rot;
	};

	class ExSkeleton
	{
	public:
		//constructor
		ExSkeleton(IGameNode* node, IGameSkin* pGameSkin, Matrix3 offset, std::string name, ExOgreConverter* converter);
		//destructor
		~ExSkeleton();
		//clear skeleton data
		void clear();
		//load skeleton data
		//		bool load(IGameNode* pGameNode, IGameObject* pGameObject, IGameSkin* pGameSkin);

		bool getVertexBoneWeights(int numVertices);

		// returns the index of the bone in the skeleton.  -1 if it doesn't exist.
		int getJointIndex(INode* pNode);

		//load skeletal animations
		bool loadAnims(IGameNode* pGameNode);
		//get joints
		std::vector<ExBone>& getJoints();
		//get animations
		std::vector<ExAnimation>& getAnimations();
		//restore skeleton pose
		void restorePose();
		//write to an OGRE binary skeleton
		bool writeOgreBinary();

		const std::vector<float> getWeightList(int index);
		const std::vector<int> getJointList(int index);
	private:
		//load a clip
		bool loadClip(std::string clipName, int start, int stop, int rate);
		//load a keyframe for a particular joint at current time
		skeletonKeyframe loadKeyframe(ExBone& j, int time);
		//write joints to an Ogre skeleton
		bool createOgreBones(Ogre::SkeletonPtr pSkeleton);
		// write skeleton animations to an Ogre skeleton
		bool createOgreSkeletonAnimations(Ogre::SkeletonPtr pSkeleton);
		//load a joint
		bool loadJoint(INode* pNode);

		Matrix3 offsetTM;
		IGameNode* m_pGameNode;
		IGameSkin* m_pGameSkin;
		std::vector<ExBone> m_joints;
		std::vector<ExAnimation> m_animations;
		std::vector<int> m_roots;
		std::vector< std::vector<float> > m_weights;
		std::vector< std::vector<int> > m_jointIds;
		std::string m_restorePose;
		std::string m_name;
		ParamList m_params;
		ExOgreConverter* m_converter;
		bool m_isBiped;
	};
}	//end namespace

#endif
