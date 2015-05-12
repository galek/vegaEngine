#pragma once

namespace vega
{
	class xgScene_t;

	struct EngineWrapper :EngineGlobals
	{
		EngineWrapper();
		virtual ~EngineWrapper();
		virtual void StartupOgre();
		virtual void PreInitSystems();
		virtual void StartupSystems();
		virtual void Go(int width, int height, Ogre::NameValuePairList*_ptr);
		virtual void ChooseSceneManager();
		virtual void CreateFrameListener();
	public:
		xgScene_t* GetEditorScene();
		void ShowEditor();
	private:
		xgScene_t*edscene;
	};

	EngineWrapper*GetEditor();
}