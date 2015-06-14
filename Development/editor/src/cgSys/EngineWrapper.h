#pragma once

#include "../../../src/Engine/EngineAPI.inc"

namespace vega
{
	class xgScene_t;

	struct EngineWrapper :public EngineGlobals
	{
		EngineWrapper();
		virtual ~EngineWrapper(void);
		virtual void StartupOgre(void);
		virtual bool Configure(void);
		virtual void PreInitSystems(void);
		virtual void StartupSystems(void);
		virtual void Go(int width, int height, Ogre::NameValuePairList*_ptr);
		virtual void ChooseSceneManager(void);
		virtual void CreateFrameListener(void);
		virtual bool Setup(void);
		virtual void PrecacheResources(void);
	private:
		virtual void SetupResources(void);
	public:
		xgScene_t* GetEditorScene(void);
		void ShowEditor(void);
	private:
		struct CreationParams
		{
			int width, height;
			Ogre::NameValuePairList* ptr;
		};
		CreationParams params;
		xgScene_t*edscene;
	};

	EngineWrapper*GetEditor();
}