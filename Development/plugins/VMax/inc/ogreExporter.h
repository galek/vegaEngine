#ifndef OGRE_EXPORTER_H
#define OGRE_EXPORTER_H

#include "ExScene.h"
#include "ExOgreConverter.h"
#include "ExPrerequisites.h"

namespace VMax
{

	class OgreSceneExporter : public SceneExport
	{
	public:
		// public methods
		OgreSceneExporter();
		virtual ~OgreSceneExporter();

		int ExtCount(void);
		const TCHAR* Ext(int n);
		const TCHAR* LongDesc(void);
		const TCHAR* ShortDesc(void);
		const TCHAR* AuthorName(void);
		const TCHAR* CopyrightMessage(void);
		const TCHAR* OtherMessage1(void);
		const TCHAR* OtherMessage2(void);
		unsigned int Version(void);
		void ShowAbout(HWND hWnd);
		BOOL SupportsOptions(int ext, DWORD options);
		int	DoExport(const TCHAR* name, ExpInterface* pExpInterface, Interface* pInterface, BOOL suppressPrompts = FALSE, DWORD options = 0);
		void loadExportConf(std::string path, ParamList &param);
	};

	class OgreExporter
	{
	public:
		// public methods
		OgreExporter();
		~OgreExporter();

		bool exportScene();
		ParamList m_params;

	private:
		ExScene* sceneData;
		ExOgreConverter* ogreConverter;
		TimeValue m_curTime;
		IGameScene* pIGame;
		std::vector<INode*> lFoundBones;
		int nodeCount;

		void initIGameConf(std::string path);
		void saveExportConf(std::string path);
		bool exportNode(IGameNode* pGameNode, TiXmlElement* parent);
		void LoadSkinBones(IGameNode* pGameNode);
		bool IsSkinnedBone(IGameNode* pGameNode);
		bool IsNodeToExport(IGameNode* pGameNode);
	};
}	//end namespace

#endif
