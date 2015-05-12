#pragma once
#include <wx/notebook.h>
#include "wx/dir.h"
class wxGenericDirCtrl;
class wxListCtrl;
class wxTreeCtrl;

namespace vega
{
	class resInsertTool_t :public wxNotebook, public tool_t, public wxDirTraverser{
		int currentResType;
		DECLARE_EVENT_TABLE()
		wxGenericDirCtrl *dir;
		wxListCtrl *particleList;
		wxTreeCtrl *entityList;
		wxString meshFile;
		wxString moveObjectName;
		std::string gameEntityName;
		bool active;
		Ogre::MovableObject *entity;
		Ogre::SceneNode *sceneNode;
		int id;
		int dirTraverseMode;
		enum dirTraverseModeEnum{
			listEntityXmlEnum,
			listEntityMeshEnum
		};
		bool entityXmlListLoaded;
		bool entityMeshPathLoaded;
		bool getIntersectGroundPoint(int x, int y, Ogre::Vector3 &pos);
		void update(int resType);
		void updateParticleList();
		void loadEntityXmlList();
		bool loadEntityXmlFile(const wxTreeItemId &rootId, const wxString &file);
		void loadEntityMeshPath();
	public:
		virtual wxDirTraverseResult  OnFile(const wxString &filename);
		virtual wxDirTraverseResult  OnDir(const wxString &dirname);
		bool insertRes(const char *file, int x, int y);
		void refresh();
		resInsertTool_t();
		~resInsertTool_t();
		virtual bool init();
		void onEvent(wxTreeEvent & event);
		void onParticleListEvent(wxListEvent  & event);
		void onEntityListEvent(wxTreeEvent & event);
		virtual bool onActive();
		virtual bool onDeactive();
		virtual bool Show(bool show = true);
		virtual bool onViewMouseEvent(wxMouseEvent & event);
		void onLightCreateCommanEvent(wxCommandEvent & event);
		bool onViewEvent(wxEvent & event);
	};
}