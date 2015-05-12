#include "ExPrerequisites.h"
#include "ExOgreConverter.h"
#include "VMaxLog.h"
#include "ExMesh.h"
#include "decomp.h"

namespace VMax
{
	// constructor
	ExOgreConverter::ExOgreConverter(IGameScene* pIGameScene, ParamList params)
	{
		mParams = params;
		pIGame = pIGameScene;
		mMaterialSet = new ExMaterialSet(this);
	}

	// destructor
	ExOgreConverter::~ExOgreConverter()
	{
		if (mMaterialSet)
		{
			delete mMaterialSet;
			mMaterialSet = 0;
		}
	}

	ExMaterialSet* ExOgreConverter::getMaterialSet()
	{
		return mMaterialSet;
	}

	ParamList ExOgreConverter::getParams()
	{
		return mParams;
	}

	bool ExOgreConverter::writeEntityData(IGameNode* pGameNode, IGameObject* pGameObject, IGameMesh* pGameMesh, std::vector<ExMaterial*>& lmat)
	{
		bool ret = false;

		std::string meshName = mParams.resPrefix;
#ifdef UNICODE
		std::wstring name_w = pGameNode->GetName();
		std::string name_s;
		name_s.assign(name_w.begin(), name_w.end());
		meshName.append(name_s);
#else
		meshName.append(pGameNode->GetName());
#endif
		meshName = optimizeResourceName(meshName);
		ExMesh* mesh = new ExMesh(this, pGameNode, pGameMesh, meshName);

		lmat = mesh->getMaterials();

		if (!isFirstInstance(pGameNode))
		{
			VMaxLog("Info: Ignore instanciated mesh\n");
			ret = true;
		}
		else
		{
			if (mParams.exportMesh)
			{
#ifdef UNICODE
				MSTR nodeName = pGameNode->GetName();
				VMaxLog("Writing %ls mesh binary...\n", nodeName.data());
#else
				VMaxLog("Writing %s mesh binary...\n", pGameNode->GetName());
#endif

				if (!(ret = mesh->writeOgreBinary()))
				{
					VMaxLog("Warning : Mesh skipped, see previous log to know why.\n");
				}
			}
		}

		// Write skeleton binary
		if (mParams.exportSkeleton && mesh->getSkeleton() && isFirstInstance(pGameNode))
		{
			// Load skeleton animations
			mesh->getSkeleton()->loadAnims(pGameNode);

			VMaxLog("Writing skeleton binary...\n");
			if (!mesh->getSkeleton()->writeOgreBinary())
			{
				VMaxLog("Error writing skeleton binary file\n");
			}
		}

		delete mesh;
		return ret;
	}

	bool ExOgreConverter::writeMaterialFile()
	{
		bool ret = true;
		if (mParams.exportMaterial)
		{
			VMaxLog("Writing materials data...\n");
			if (!mMaterialSet->writeOgreScript(mParams))
			{
				VMaxLog("Error writing materials file\n");
				ret = false;
			}
		}

		return ret;
	}

	void ExOgreConverter::addExportedRootBone(ExBone bone)
	{
		mExportedRootBones.push_back(bone);
	}

	bool ExOgreConverter::isExportedRootBone(ExBone bone)
	{
		for (size_t i = 0; i < mExportedRootBones.size(); i++)
		{
			if (bone.nodeID == mExportedRootBones[i].nodeID)
				return true;
		}

		return false;
	}

	void ExOgreConverter::addSkinModifier(IGameSkin* skinmod)
	{
		mSkinList.push_back(skinmod->GetMaxModifier());
		IBipMaster* bipMaster = GetBipedMasterInterface(skinmod);
		DWORD prevBipMode = bipMaster ? bipMaster->GetActiveModes() : 0;
		mSkinLastStateList.push_back(prevBipMode);
		ReleaseBipedMasterInterface(skinmod, bipMaster);
	}

	void ExOgreConverter::setAllSkinToBindPos()
	{
		for (unsigned int i = 0; i < mSkinList.size(); i++)
		{
			Modifier* skinmod = mSkinList[i];
			IBipMaster* bipMaster = GetBipedMasterInterface(skinmod);
			DWORD prevBipMode = bipMaster ? bipMaster->GetActiveModes() : 0;

			if (bipMaster)
			{
				bipMaster->EndModes(prevBipMode, 0);
				bipMaster->BeginModes(BMODE_FIGURE, 0);
				ReleaseBipedMasterInterface(skinmod, bipMaster);
			}
		}
	}

	void ExOgreConverter::restoreAllSkin()
	{
		for (unsigned int i = 0; i < mSkinList.size(); i++)
		{
			Modifier* skinmod = mSkinList[i];
			IBipMaster* bipMaster = GetBipedMasterInterface(skinmod);
			DWORD prevBipMode = bipMaster ? bipMaster->GetActiveModes() : 0;

			if (bipMaster)
			{
				bipMaster->EndModes(prevBipMode, 0);
				bipMaster->BeginModes(mSkinLastStateList[i], 0);
				ReleaseBipedMasterInterface(skinmod, bipMaster);
			}
		}
	}
} //end of namespace
