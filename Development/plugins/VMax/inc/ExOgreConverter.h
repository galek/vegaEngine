#ifndef _EXOGRECONVERTER_H
#define _EXOGRECONVERTER_H


#include "paramList.h"
#include "ExMaterialSet.h"

namespace VMax
{
	class ExOgreConverter
	{
	public:
		IGameScene* pIGame;

		//constructor
		ExOgreConverter(IGameScene* pIGameScene, ParamList params);

		//destructor
		~ExOgreConverter();

		// for skeleton unicity
		void addExportedRootBone(ExBone bone);
		bool isExportedRootBone(ExBone bone);
		void addSkinModifier(IGameSkin* skinmod);
		void setAllSkinToBindPos();
		void restoreAllSkin();
		void setHasError(bool state);
		bool hasError();

		bool writeEntityData(IGameNode* pGameNode, IGameObject* pGameObject, IGameMesh* pGameMesh, std::vector<ExMaterial*>& lmat);
		bool writeMaterialFile();
		ExMaterialSet* getMaterialSet();
		ParamList getParams();

	private:
		ParamList mParams;
		ExMaterialSet* mMaterialSet;
		std::vector<ExBone> mExportedRootBones;
		std::vector<Modifier*> mSkinList;
		std::vector<DWORD> mSkinLastStateList;
		bool mHasError;
	};
} // end of namespace

#endif
