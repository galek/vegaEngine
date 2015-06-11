#pragma once

class wxListbook;
class wxTreebook;
class wxChoicebook;
class wxAuiManager;


namespace vega
{
	// Declare our main frame class
	class docInfoList_t;
	class toolBag_t;
	class wxOgre_t;
	class updateBag_t;
	class propBag_t;
	class moveObjectPropBag_t;
	class tool_t;
	class xmlStack_t;
	class xmlElement_t;
	class log_t;
	class prsTool_t;

	class WxMainToolBar;

	class frame_t : public wxFrame
	{
	public:
		const std::string &getResRootPath(){ return resRootPath; }
		bool processViewEvent(wxEvent &);
		void pushViewMsgRecvTool(tool_t *me);
		void popViewMsgRecvTool(tool_t *me);
		void setViewMsgRecvTool(tool_t *me);
		propBag_t *getPropBag(){ return propBag; }
		void toggleTimerUpdate();
		updateBag_t *getUpdateBag();
		wxOgre_t *getView(){ return viewWindow; }
		virtual bool ProcessEvent(wxEvent &  event);
		wxMenuBar *getMenuBar(){ return menuBar; }
		wxTextCtrl *getLogText(){ return logWindow; }
		wxTreebook *getToolBook(){ return toolWindow; }
		wxChoicebook *getPropBook(){ return propWindow; }
		wxChoicebook *getProjectBook(){ return projectWindow; }
		wxAuiManager *getFrameManager(){ return fm; }
		frame_t(const wxString& title);
		void OnQuit(wxCommandEvent& event);
		void OnAbout(wxCommandEvent& event);
		void OnNew(wxCommandEvent& event);
		void OnOpen(wxCommandEvent& event);
		void OnRunScript(wxCommandEvent& event);
		void OnSave(wxCommandEvent& event);
		void OnSaveAs(wxCommandEvent& event);
		void OnActivate(wxActivateEvent& event);
		void UpdateStatusText();
		~frame_t();
	private:
		DECLARE_EVENT_TABLE()
		void initTool();
		void onTimer(wxTimerEvent& event);
		void resetTool();
		void CreateEditorToolBar();

		/*private*/public:
			wxString documentName;
			std::string resRootPath;
			moveObjectPropBag_t *moveObjectPropBag;
			propBag_t *propBag;
			wxTimer	timer;
			updateBag_t *updateBag;
			toolBag_t *toolBag;
			toolBag_t *viewMsgRecvToolBag;
			wxMenuBar *menuBar;
			wxTreebook *toolWindow;
			wxChoicebook *propWindow;
			wxOgre_t *viewWindow;
			wxChoicebook *projectWindow;
			wxTextCtrl *logWindow;
			wxAuiManager *fm;
			docInfoList_t *docInfoList;
			tool_t *currentViewMsgRecvTool;
			WxMainToolBar*toolbar;
			log_t*log;
			prsTool_t*prs;
	};
	extern frame_t *frame;
}