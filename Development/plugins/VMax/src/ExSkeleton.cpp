#include "ExPrerequisites.h"
#include "ExTools.h"
#include "ExSkeleton.h"
#include "VMaxLog.h"
#include "decomp.h"
#include "BipedApi.h"
#include "IMixer.h"
#include "iskin.h"
#include "IFrameTagManager.h"

namespace VMax
{
	ExSkeleton::ExSkeleton(IGameNode* node, IGameSkin* pGameSkin, Matrix3 offset, std::string name, ExOgreConverter* converter)
	{
		m_joints.clear();
		m_animations.clear();
		m_restorePose = "";
		m_name = name;
		m_pGameNode = node;
		m_pGameSkin = pGameSkin;
		m_converter = converter;
		m_params = converter->getParams();
		m_isBiped = false;
		offsetTM = offset;
	}

	ExSkeleton::~ExSkeleton()
	{
		clear();
	}

	// Clear skeleton data
	void ExSkeleton::clear()
	{
		m_joints.clear();
		m_weights.clear();
		m_jointIds.clear();
		m_animations.clear();
		m_restorePose = "";
	}

	const std::vector<float> ExSkeleton::getWeightList(int index)
	{
		return m_weights[index];
	}

	const std::vector<int> ExSkeleton::getJointList(int index)
	{
		return m_jointIds[index];
	}

	// Get vertex bone assignements
	bool ExSkeleton::getVertexBoneWeights(int numVertices)
	{
		//NOTE don't use IGameNode here sometimes bones can be a mesh and GetBoneNode return null
		VMaxLog("Info : Get vertex bone weight\n");

		//init list indices
		m_weights.resize(numVertices);
		m_jointIds.resize(numVertices);

		std::vector<INode*> rootbones;

		//nothing to export
		if (m_pGameSkin->GetTotalBoneCount() <= 0)
		{
			VMaxLog("Warning : No assigned bones\n");
			return false;
		}

		for (int i = 0; i < m_pGameSkin->GetTotalBoneCount() && !m_isBiped; ++i)
		{
			INode* nbone = m_pGameSkin->GetBone(i, true);
			Control* nodeControl = nbone->GetTMController();
			if ((nodeControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) || (nodeControl->ClassID() == BIPBODY_CONTROL_CLASS_ID))
			{
				m_isBiped = true;
			}
			else
			{
				while (nbone->GetParentNode() != GetCOREInterface()->GetRootNode() && !m_isBiped)
				{
					nbone = nbone->GetParentNode();
					Control* pnodeControl = nbone->GetTMController();
					if ((pnodeControl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) || (pnodeControl->ClassID() == BIPBODY_CONTROL_CLASS_ID))
					{
						m_isBiped = true;
					}
				}
			}
		}

		if (m_isBiped)
		{
			for (int i = 0; i < m_pGameSkin->GetTotalBoneCount(); ++i)
			{
				// pass false to only get bones used by vertices
				INode* rootbone = m_pGameSkin->GetBone(i, false);
				if (rootbone)
				{
					while (m_pGameSkin->GetBoneIndex(rootbone->GetParentNode(), false) > -1)
					{
						rootbone = rootbone->GetParentNode();
					}

					bool bNewRootBone = true;
					for (int j = 0; j < rootbones.size(); ++j)
					{
						if (rootbones[j] == rootbone)
						{
							// this bone is already in the list
							bNewRootBone = false;
						}
					}

					if (bNewRootBone)
					{
#ifdef UNICODE
						VMaxLog("Info : Found a root bone : %ls\n", rootbone->GetName());
#else
						VMaxLog("Info : Found a root bone : %s\n", rootbone->GetName());
#endif
						rootbones.push_back(rootbone);
					}
				}
			}
		}
		else
		{
			for (int i = 0; i < m_pGameSkin->GetTotalBoneCount(); ++i)
			{
				// pass false to only get bones used by vertices
				INode* rootbone = m_pGameSkin->GetBone(i, false);
				if (rootbone)
				{
					while (rootbone->GetParentNode() != GetCOREInterface()->GetRootNode())
					{
						rootbone = rootbone->GetParentNode();
					}

					bool bNewRootBone = true;
					for (int j = 0; j < rootbones.size(); ++j)
					{
						if (rootbones[j] == rootbone)
						{
							// this bone is already in the list
							bNewRootBone = false;
						}
					}

					if (bNewRootBone)
					{
#ifdef UNICODE
						VMaxLog("Info : Found a root bone : %ls\n", rootbone->GetName());
#else
						VMaxLog("Info : Found a root bone : %s\n", rootbone->GetName());
#endif
						rootbones.push_back(rootbone);
					}
				}
			}
		}

		for (int i = 0; i < rootbones.size(); ++i)
		{
#ifdef UNICODE
			VMaxLog("Exporting root bone : %ls\n", rootbones[i]->GetName());
#else
			VMaxLog("Exporting root bone : %s\n", rootbones[i]->GetName());
#endif
			loadJoint(rootbones[i]);
		}

		int numSkinnedVertices = m_pGameSkin->GetNumOfSkinnedVerts();

		if (numSkinnedVertices > numVertices)
		{
			MessageBox(GetCOREInterface()->GetMAXHWnd(), _T("The Mesh informations is not up-to-date, perform a \"Reset X-form\" and retry."), _T("Warning"), MB_OK);
			return false;
		}

		VMaxLog("Num. Skinned Vertices: %d\n", numSkinnedVertices);
		std::vector<std::string> lwarnings;
		for (int i = 0; i < numSkinnedVertices; ++i)
		{
			int type = m_pGameSkin->GetVertexType(i);
			// Rigid vertices.
			if (type == IGameSkin::IGAME_RIGID)
			{
				INode* pBoneNode = m_pGameSkin->GetBone(i, 0);
				if (pBoneNode && m_pGameSkin->GetBoneIndex(pBoneNode, false) > -1)
				{
					int boneIndex = getJointIndex(pBoneNode);
					if (boneIndex >= 0)
					{
						m_weights[i].push_back(1.0f);
						m_jointIds[i].push_back(boneIndex);

						if (m_weights[i].size() > 4)
						{
#ifdef UNICODE
							std::wstring name_w = pBoneNode->GetName();
							std::string name_s;
							name_s.assign(name_w.begin(), name_w.end());
							lwarnings.push_back(name_s);
#else
							lwarnings.push_back(pBoneNode->GetName());
#endif
						}
					}
				}
			}
			// Blended vertices.
			else
			{
				int numWeights = m_pGameSkin->GetNumberOfBones(i);
				for (int j = 0; j < numWeights; ++j)
				{
					INode* pBoneNode = m_pGameSkin->GetBone(i, j);
					if (pBoneNode && m_pGameSkin->GetBoneIndex(pBoneNode, false) > -1)
					{
						int boneIndex = getJointIndex(pBoneNode);
						float weight = m_pGameSkin->GetWeight(i, j);
						if ((boneIndex >= 0) && (weight > 0.0f))
						{
							m_weights[i].push_back(weight);
							m_jointIds[i].push_back(boneIndex);

							if (m_weights[i].size() > 4)
							{
#ifdef UNICODE
								std::wstring name_w = pBoneNode->GetName();
								std::string name_s;
								name_s.assign(name_w.begin(), name_w.end());
								lwarnings.push_back(name_s);
#else
								lwarnings.push_back(pBoneNode->GetName());
#endif
							}
						}
					}
				}
			}
		}

		//sort and remove duplicated entries
		if (lwarnings.size() > 0)
		{
			std::sort(lwarnings.begin(), lwarnings.end());
			lwarnings.erase(std::unique(lwarnings.begin(), lwarnings.end()), lwarnings.end());
		}

		if (lwarnings.size() > 0)
		{
			std::string mess = "Warning : Vertex found with more than 4 weights on :\n";

			for (int i = 0; i < lwarnings.size(); ++i)
				mess.append("skeleton " + m_name + " with bone " + lwarnings[i] + "\n");

			mess.append("This is not compatible with hardware skinning method.\n");
			VMaxLog(mess.c_str());
#ifdef UNICODE
			std::wstring mess_w;
			mess_w.assign(mess.begin(), mess.end());
			MessageBox(GetCOREInterface()->GetMAXHWnd(), mess_w.data(), _T("Warning"), MB_OK);
#else
			MessageBox(GetCOREInterface()->GetMAXHWnd(), _T(mess.c_str()), _T("Warning"), MB_OK);
#endif
		}
		lwarnings.clear();

		return true;
	}

	int ExSkeleton::getJointIndex(INode* pNode)
	{
		if (pNode)
		{
			for (int i = 0; i < m_joints.size(); i++)
			{
				if (m_joints[i].nodeID == pNode->GetHandle())
					return i;
			}
		}
		return -1;
	}

	// Load a joint
	bool ExSkeleton::loadJoint(INode* pNode)
	{
		// node index if already exist
		int boneIndex = getJointIndex(pNode);

		// get parent index
		int parentIdx = getJointIndex(pNode->GetParentNode());

		// test for supported bone type
		if (!IsPossibleBone(pNode))
		{
#ifdef UNICODE
			VMaxLog("Info : %ls is not a bone.\n", pNode->GetName());
#else
			VMaxLog("Info : %s is not a bone.\n", pNode->GetName());
#endif
			return false;
		}

		// initialise joint to avoid bad searchs
		ExBone newJoint;
		newJoint.pNode = pNode;
#ifdef UNICODE
		std::wstring name_w = pNode->GetName();
		std::string name_s;
		name_s.assign(name_w.begin(), name_w.end());
		newJoint.name = name_s;
#else
		newJoint.name = pNode->GetName();
#endif
		newJoint.nodeID = pNode->GetHandle();
		newJoint.id = -1;
		newJoint.parentIndex = parentIdx;

		if (boneIndex == -1)
		{
			bool duplicated = true;
			int dpid = 1;
			while (duplicated)
			{
				bool found = false;

				// Make sure we don't have a duplicate bone name
				for (size_t i = 0; i < m_joints.size(); ++i)
				{
					if (newJoint.name == m_joints[i].name)
					{
						std::string sid;
						std::stringstream strId;
						strId << dpid;
						sid = strId.str();

#ifdef UNICODE
						std::wstring name_w = pNode->GetName();
						std::string name_s;
						newJoint.name = std::string(name_s) + sid;
#else
						newJoint.name = std::string(pNode->GetName()) + sid;
#endif
						found = true;
						dpid++;
					}
				}
				duplicated = found;
			}

			// If this is a new joint, push one back to the end of the array.
			// Otherwise we still continue in case we had previously thought
			// this bone was a root bone (incorrectly).
			m_joints.push_back(newJoint);
			boneIndex = m_joints.size() - 1;
		}
		else
		{
			bool bShouldReExport = false;
			for (size_t i = 0; i < m_roots.size(); i++)
			{
				if (m_joints[m_roots[i]].pNode == pNode)
				{
					if (m_pGameSkin->GetBoneIndex(pNode->GetParentNode(), false) > -1)
					{
						int newParentIndex = getJointIndex(pNode->GetParentNode());

						if (-1 != newParentIndex)
						{
							bShouldReExport = true;
							m_roots.erase(m_roots.begin() + i);
							i--;
						}
					}
				}
			}
			if (!bShouldReExport)
			{
				// no sense in going further as we've already exported this joint and
				// it hasn't changed from a root to a non-root.
				return true;
			}
		}

		// Get mesh matrix at initial pose

		Modifier* skinMod = m_pGameSkin->GetMaxModifier();
		ISkin* pskin = 0;
		if (skinMod)
			pskin = (ISkin*)skinMod->GetInterface(I_SKIN);


		// set the new bone index
		m_joints[boneIndex].id = boneIndex;
		Matrix3 localTM;

		// get the root bone matrix relative to the mesh
		if (parentIdx == -1)
			localTM = GetLocalUniformMatrix(pNode, m_pGameNode->GetMaxNode(), offsetTM, m_params.yUpAxis, GetFirstFrame());
		else
			localTM = GetLocalUniformMatrix(pNode, offsetTM, m_params.yUpAxis, GetFirstFrame());

		AffineParts ap;
		decomp_affine(localTM, &ap);

		Point3 trans = ap.t * m_params.lum;
		Point3 scale = ap.k;
		Quat rot = ap.q;

		// Notice that in Max we flip the w-component of the quaternion;
		rot.w = -rot.w;

		if (boneIndex == m_joints.size() - 1)
			VMaxLog("Exporting joint %s. Trans(%f,%f,%f) Rot(%f,%f,%f,%f), Scale(%f,%f,%f).\n", pNode->GetName(), trans.x, trans.y, trans.z, rot.w, rot.x, rot.y, rot.z, scale.x, scale.y, scale.z);

		// Set joint coords
		m_joints[boneIndex].bindMatrix = localTM;
		m_joints[boneIndex].trans = trans;
		m_joints[boneIndex].scale = scale;
		m_joints[boneIndex].rot = rot;

		// If root is a root joint, save it's index in the roots list
		if (parentIdx < 0)
			m_roots.push_back(m_joints.size() - 1);

		// Load child joints
		for (size_t i = 0; i < pNode->NumberOfChildren(); i++)
		{
			INode* pChildNode = pNode->GetChildNode(i);
			if (pChildNode)
				loadJoint(pChildNode);
		}


		if (pskin != 0)
			skinMod->ReleaseInterface(I_SKIN, (void*)pskin);

		return true;
	}

	// Load animations
	bool ExSkeleton::loadAnims(IGameNode* pGameNode)
	{
		VMaxLog("Loading joint animations...\n");

		// clear animations list
		m_animations.clear();

		//load clips from mixer
		IMixer* mixer = 0;
		IBipMaster* bipMaster = GetBipedMasterInterface(m_pGameSkin);
		if (bipMaster)
			mixer = bipMaster->GetMixer();

		bool useDefault = true;
		//we found a mixer try to load clips
		if (mixer)
		{
			DWORD actMode = bipMaster->GetActiveModes();
			if (!(actMode & BMODE_MIXER))
				bipMaster->BeginModes(BMODE_MIXER, 0);

			int clipId = 0;
			int numGroups = mixer->NumTrackgroups();
			for (size_t j = 0; j < numGroups; j++)
			{
				IMXtrackgroup* group = mixer->GetTrackgroup(j);
#ifdef UNICODE
				VMaxLog("Info : mixer track found %ls\n", group->GetName());
#else
				VMaxLog("Info : mixer track found %s\n", group->GetName());
#endif

				int numTracks = group->NumTracks();
				for (size_t k = 0; k < numTracks; k++)
				{
					IMXtrack* track = group->GetTrack(k);
					BOOL tMode = track->GetSolo();
					track->SetSolo(TRUE);

					int numClips = track->NumClips(BOT_ROW);
					for (size_t l = 0; l < numClips; l++)
					{
						IMXclip* clip = track->GetClip(l, BOT_ROW);
						if (clip)
						{
							int start;
							int stop;
#ifdef PRE_MAX_2010
							std::string clipName = formatClipName(std::string(clip->GetFilename()), clipId);
#else
							MaxSDK::AssetManagement::AssetUser &clipFile = const_cast<MaxSDK::AssetManagement::AssetUser&>(clip->GetFile());
#ifdef UNICODE
							std::wstring clipFileName_w = clipFile.GetFileName();
							std::string clipFileName_s;
							clipFileName_s.assign(clipFileName_w.begin(), clipFileName_w.end());
							std::string clipName = formatClipName(std::string(clipFileName_s), clipId);
#else
							std::string clipName = formatClipName(std::string(clipFile.GetFileName()), clipId);
#endif
#endif

							clip->GetGlobalBounds(&start, &stop);
							VMaxLog("Info : mixer clip found %s from %i to %i\n", clipName.c_str(), start, stop);

							if (loadClip(clipName, start, stop, GetTicksPerFrame()))
								useDefault = false;
							clipId++;
						}
					}
					track->SetSolo(tMode);
				}
			}

			if (!(actMode & BMODE_MIXER))
				bipMaster->EndModes(BMODE_MIXER, 0);
		}

		//release max interface
		ReleaseBipedMasterInterface(m_pGameSkin, bipMaster);

		//load main clip on actual timeline;
		if (useDefault)
		{
			Interval animRange = GetCOREInterface()->GetAnimRange();
			IFrameTagManager* frameTagMgr = static_cast<IFrameTagManager*>(GetCOREInterface(FRAMETAGMANAGER_INTERFACE));
			int cnt = frameTagMgr->GetTagCount();

			if (!cnt)
			{
				loadClip("default_skl", animRange.Start(), animRange.End(), GetTicksPerFrame());
			}
			else
			{
				for (int i = 0; i < cnt; i++)
				{
					DWORD t = frameTagMgr->GetTagID(i);
					DWORD tlock = frameTagMgr->GetLockIDByID(t);

					//ignore locked tags used for animation end
					if (tlock != 0)
						continue;

					TimeValue tv = frameTagMgr->GetTimeByID(t, FALSE);
					TimeValue te = animRange.End();

					DWORD tnext = 0;
					if ((i + 1) < cnt)
					{
						tnext = frameTagMgr->GetTagID(i + 1);
						te = frameTagMgr->GetTimeByID(tnext, FALSE);
					}

					Interval ianim(tv, te);
#ifdef UNICODE
					std::wstring name_w = frameTagMgr->GetNameByID(t);
					std::string name_s;
					name_s.assign(name_w.begin(), name_w.end());
					loadClip(std::string(name_s), ianim.Start(), ianim.End(), GetTicksPerFrame());
#else
					loadClip(std::string(frameTagMgr->GetNameByID(t)), ianim.Start(), ianim.End(), GetTicksPerFrame());
#endif
				}
			}
		}

		return true;
	}

	// Load an animation clip
	bool ExSkeleton::loadClip(std::string clipName, int start, int stop, int rate)
	{
		// if skeleton has no joints we can't load the clip
		if (m_joints.size() < 0)
			return false;

		// display clip name
		VMaxLog("clip \"%s\"\n", clipName.c_str());

		// calculate times from clip sample rate
		if (rate <= 0)
		{
			VMaxLog("invalid sample rate for the clip (must be >0), we skip it\n");
			return false;
		}

		//add time steps
		std::vector<int> times;
		times.clear();
		for (int t = start; t < stop; t += rate)
			times.push_back(t);

		//force the last key
		times.push_back(stop);
		times.erase(std::unique(times.begin(), times.end()), times.end());

		// get animation length
		int length = 0;
		if (times.size() >= 0)
			length = times[times.size() - 1] - times[0];
		if (length < 0)
		{
			VMaxLog("invalid time range for the clip, we skip it\n");
			return false;
		}

		// create the animation
		ExAnimation a;
		a.m_name = clipName.c_str();
		a.m_tracks.clear();
		a.m_length = (static_cast<float>(length) / static_cast<float>(rate)) / GetFrameRate();
		m_animations.push_back(a);
		int animIdx = m_animations.size() - 1;

		// create a track for current clip for all joints
		std::vector<ExTrack> animTracks;
		for (size_t i = 0; i < m_joints.size(); i++)
		{
			ExTrack t;
			t.m_type = TT_SKELETON;
			t.m_bone = m_joints[i].name;
			t.m_skeletonKeyframes.clear();
			animTracks.push_back(t);
		}

		// evaluate animation curves at selected times
		for (size_t i = 0; i < times.size(); i++)
		{
			//int closestFrame = (int)(.5f + times[i]* GetFrameRate());

			//load a keyframe for every joint at current time
			for (size_t j = 0; j < m_joints.size(); j++)
			{
				skeletonKeyframe key = loadKeyframe(m_joints[j], times[i]);

				VMaxLog("add key frame: %f\n", key.time);
				//set key time
				key.time = (static_cast<float>((times[i] - times[0])) / static_cast<float>(rate)) / GetFrameRate();

				//add keyframe to joint track
				animTracks[j].addSkeletonKeyframe(key);
			}
#if 0//Nick			
			if (params.skelBB)
			{
				// Update bounding boxes of loaded submeshes
				for (auto j=0; j<params.loadedSubmeshes.size(); j++)
				{
					IGameNode *pGameNode = params.loadedSubmeshes[j]->m_pGameNode;
					if(pGameNode)
					{
						IGameObject* pGameObject = pGameNode->GetIGameObject();
						if(pGameObject)
						{
							Box3 bbox;
							pGameObject->GetBoundingBox(bbox);
							params.loadedSubmeshes[j]->m_boundingBox += bbox;
						}
						pGameNode->ReleaseIGameObject();
					}
				}
			}
#endif
		}
		// add created tracks to current clip
		for (size_t i = 0; i < animTracks.size(); i++)
		{
			animTracks[i].optimize();
			m_animations[animIdx].addTrack(animTracks[i]);
		}

		// display info
		VMaxLog("length: %f\n", m_animations[animIdx].m_length);
		if (animTracks.size() > 0)
			VMaxLog("num keyframes: %d\n", animTracks[0].m_skeletonKeyframes.size());

		// clip successfully loaded
		return true;
	}

	// Load a keyframe for a given joint at current time
	skeletonKeyframe ExSkeleton::loadKeyframe(ExBone& j, int time)
	{
		INode* bone = j.pNode;
		Matrix3 boneTM;

		// get the root bone matrix relative to the mesh
		if (j.parentIndex == -1)
			boneTM = GetLocalUniformMatrix(bone, m_pGameNode->GetMaxNode(), offsetTM, m_params.yUpAxis, time);
		else
			boneTM = GetLocalUniformMatrix(bone, offsetTM, m_params.yUpAxis, time);

		Matrix3 relMat = GetRelativeMatrix(boneTM, j.bindMatrix);

		AffineParts tap;
		decomp_affine(boneTM, &tap);
		Point3 trans = (tap.t * m_params.lum) - j.trans;

		AffineParts ap;
		decomp_affine(relMat, &ap);
		Point3 scale = ap.k;
		Quat rot = ap.q;
		// Notice that in Max we flip the w-component of the quaternion;
		rot.w = -rot.w;

		//create keyframe
		skeletonKeyframe key;
		key.time = 0;
		key.trans = trans;
		key.rot = rot;
		key.scale = scale;

		return key;
	}

	// Restore skeleton pose
	void ExSkeleton::restorePose()
	{
		// TODO: required in Max?
	}

	// Get joint list
	std::vector<ExBone>& ExSkeleton::getJoints()
	{
		return m_joints;
	}

	// Get animations
	std::vector<ExAnimation>& ExSkeleton::getAnimations()
	{
		return m_animations;
	}

	// Write to an OGRE binary skeleton
	bool ExSkeleton::writeOgreBinary()
	{
		// Construct skeleton
		Ogre::SkeletonPtr pSkeleton = Ogre::SkeletonManager::getSingleton().create(m_name.c_str(), Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

		// Create skeleton bones
		if (!createOgreBones(pSkeleton))
		{
			VMaxLog("Error on bone creation\n");
		}

		// skeleton empty
		if (pSkeleton->getNumBones() == 0)
			return false;

		pSkeleton->setBindingPose();

		// Create skeleton animation
		if (m_params.exportSkelAnims)
		{
			if (!createOgreSkeletonAnimations(pSkeleton))
			{
				VMaxLog("Error writing ogre skeleton animations\n");
			}
			else
			{
				// Optimise animations
				pSkeleton->optimiseAllAnimations();
			}
		}

		// Export skeleton binary
		Ogre::SkeletonSerializer serializer;

		std::string filePath = makeOutputPath(m_params.outputDir, m_params.meshOutputDir, m_name, "skeleton");
		serializer.exportSkeleton(pSkeleton.getPointer(), filePath.c_str());
		pSkeleton.setNull();

		// Skeleton successfully exported
		return true;
	}

	// Write joints to an Ogre skeleton
	bool ExSkeleton::createOgreBones(Ogre::SkeletonPtr pSkeleton)
	{
		// Doug Perkowski
		// 5/25/2010
		// To prevent a crash on content with more than  (256) bones, we need to check
		// to make sure there aren't more than OGRE_MAX_NUM_BONES bones.
		if (m_joints.size() > OGRE_MAX_NUM_BONES)
		{
			MessageBox(NULL, _T("Failure: Skeleton has more than OGRE_MAX_NUM_BONES.  No bones will be exported."), _T("EasyOgre Export Error"), MB_OK | MB_ICONWARNING);
			VMaxLog("Failure: Skeleton has more than OGRE_MAX_NUM_BONES.  No bones will be exported.\n");
			return false;
		}

		// Create the bones
		for (size_t i = 0; i < m_joints.size(); i++)
		{
			ExBone j = m_joints[i];
			if (!m_converter->isExportedRootBone(j))
			{
				m_converter->addExportedRootBone(j);

				// Create a new bone
				Ogre::Bone* pBone = pSkeleton->createBone(m_joints[i].name.c_str(), m_joints[i].id);

				// Set bone position (relative to it's parent)
				pBone->setPosition(j.trans.x, j.trans.y, j.trans.z);

				// Set bone orientation (relative to it's parent)
				Ogre::Quaternion orient(j.rot.w, j.rot.x, j.rot.y, j.rot.z);
				pBone->setOrientation(orient);

				// Set bone scale (relative to it's parent
				pBone->setScale(j.scale.x, j.scale.y, j.scale.z);
			}
		}

		// Create the hierarchy
		for (size_t i = 0; i < m_joints.size(); i++)
		{
			int parentIdx = m_joints[i].parentIndex;
			if (parentIdx >= 0)
			{
				// Get current joint from skeleton
				Ogre::Bone* pParent = 0;
				Ogre::Bone* pBone = 0;
				try
				{
					pParent = pSkeleton->getBone(m_joints[parentIdx].id);
					pBone = pSkeleton->getBone(m_joints[i].id);
				}
				catch (Ogre::Exception &)
				{
					0;
				}

				// Place current bone in the parent's child list
				if (pParent && pBone)
					pParent->addChild(pBone);
			}
		}
		return true;
	}

	// Write skeleton animations to an Ogre skeleton
	bool ExSkeleton::createOgreSkeletonAnimations(Ogre::SkeletonPtr pSkeleton)
	{
		// Read loaded skeleton animations
		// parse the list reversed for good anims order
		for (size_t i = 0; i < m_animations.size(); i++)
		{
			// Create a new animation
			Ogre::Animation* pAnimation = pSkeleton->createAnimation(m_animations[i].m_name.c_str(), m_animations[i].m_length);

			// Create tracks for current animation
			for (size_t j = 0; j < m_animations[i].m_tracks.size(); j++)
			{
				ExTrack* t = &m_animations[i].m_tracks[j];

				// Create a new track
				Ogre::Bone* oBone = pSkeleton->getBone(t->m_bone.c_str());
				if (!oBone)
					continue;

				Ogre::NodeAnimationTrack* pTrack = pAnimation->createNodeTrack(j, oBone);

				// Create keyframes for current track
				for (size_t k = 0; k < t->m_skeletonKeyframes.size(); k++)
				{
					skeletonKeyframe* keyframe = &t->m_skeletonKeyframes[k];

					// Create a new keyframe
					Ogre::TransformKeyFrame* pKeyframe = pTrack->createNodeKeyFrame(keyframe->time);

					// Set translation
					pKeyframe->setTranslate(Ogre::Vector3(keyframe->trans.x, keyframe->trans.y, keyframe->trans.z));

					// Set rotation
					pKeyframe->setRotation(Ogre::Quaternion(keyframe->rot.w, keyframe->rot.x, keyframe->rot.y, keyframe->rot.z));

					// Set scale
					pKeyframe->setScale(Ogre::Vector3(keyframe->scale.x, keyframe->scale.y, keyframe->scale.z));
				}
			}
		}
		return true;
	}
}	//end namespace
