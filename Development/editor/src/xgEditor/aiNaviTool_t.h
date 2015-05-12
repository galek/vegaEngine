#pragma once
class wxGenericDirCtrl;


namespace vega
{
	class aiNaviTool_t :public wxWindow, public tool_t{
		DECLARE_EVENT_TABLE()
		bool active;
		Ogre::ManualObject *manualObject;
		Ogre::SceneNode *sceneNode;
		Ogre::Vector3 oldPoint;
		Ogre::Vector3 startPoint;
		Ogre::Vector3 endPoint;
		int state;
		int id;
		wxCoord oldMouseYPos;
		Ogre::Vector3 update();
		void makeTrangle(const Ogre::Vector3 *p, int a, int b, int c);
	public:
		aiNaviTool_t();
		~aiNaviTool_t();
		virtual bool init();
		void onEvent(wxCommandEvent & event);
		virtual bool onActive();
		virtual bool onDeactive();
		virtual bool onViewMouseEvent(wxMouseEvent & event);
		virtual bool Show(bool show = true);
	};
}