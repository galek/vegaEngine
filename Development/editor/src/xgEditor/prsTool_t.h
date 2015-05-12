#pragma once
class wxGenericDirCtrl;

#include "tool_t.h"

namespace vega
{
	class prsTool_t :public wxWindow, public tool_t{
		bool active;
		Ogre::Entity *entity;
		Ogre::SceneNode *sceneNode;
		void updatePos(wxMouseEvent & event);
		void updateRot(wxMouseEvent & event);
		void updateScale(wxMouseEvent & event);
		void updateRotY(wxMouseEvent & event);

		int buttonId;
		Ogre::Ray oldRay;
		wxCoord oldMouseYPos;
		wxCoord oldMouseXPos;
		Ogre::Vector3 oldPoint;
		Ogre::Vector3 oldPostion;
	public:
		prsTool_t();
		~prsTool_t();
		virtual bool init();
		void onEvent(wxCommandEvent & event);
		virtual bool onActive();
		virtual bool onDeactive();
		bool onViewKeyEvent(wxKeyEvent & event);
		virtual bool onViewMouseEvent(wxMouseEvent &);
		virtual bool Show(bool show = true);
	public:
		//Testing only
		wxBoxSizer *box;
		wxButton* move;
	};
}