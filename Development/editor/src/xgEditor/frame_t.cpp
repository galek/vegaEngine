#include "stdafx.h"	
#include <wx/treectrl.h>
#include <wx/noteBook.h>
#include <wx/filename.h> 
#include <wx/msw/registry.h>
#include <wx/generic/choicdgg.h>
#include <wx/aui/aui.h>
#include <wx/wx.h>
#include "wx/file.h"
#include <wx/dynlib.h> 

#include "frame_t.h"
#include "docInfoList_t.h"
#include "wxNotebookEx.h"
#include "wxListbookEx.h"
#include "wxTreebookEx.h"
#include "wxChoicebookEx.h"
#include "toolBag_t.h"
#include "log_t.h"
#include "wxOgre_t.h"
#include "updateBag_t.h"
#include "eventHandler_t.h"
#include "prop_t.h"
#include "propBag_t.h"
#include "moveObjectPropBag_t.h"
#include "toolBag_t.h"
#include "cgSys_t.h"
#include "xgScene_t.h"
#include "cgSys_t.h"
#include "xml.h"
#include "direct.h"

#include "prsTool_t.h"

#include "MainToolBar.h"
#include "ToolsIds.h"

namespace vega
{
	static const long ID_UPDATE_TIMER = wxNewId();
	static const long ID_FRAME = wxNewId();
	/**
	*/
	BEGIN_EVENT_TABLE(frame_t, wxFrame)
		EVT_TIMER(ID_UPDATE_TIMER, frame_t::onTimer)
		EVT_MENU(wxID_NEW, frame_t::OnNew)
		EVT_MENU(wxID_ABOUT, frame_t::OnAbout)
		EVT_MENU(wxID_EXIT, frame_t::OnQuit)
		EVT_MENU(wxID_OPEN, frame_t::OnOpen)
		EVT_MENU(ID_RUNSCRIPT, frame_t::OnRunScript)
		EVT_MENU(wxID_SAVE, frame_t::OnSave)
		EVT_MENU(wxID_SAVEAS, frame_t::OnSaveAs)

		EVT_ACTIVATE(frame_t::OnActivate)
		END_EVENT_TABLE()
		frame_t *frame;
	/**
	*/
	void frame_t::OnAbout(wxCommandEvent& event){
		wxString msg;
		msg.Printf(wxT("welcome to "));
		wxMessageBox(msg, wxT("About VegaEd"),
			wxOK | wxICON_INFORMATION, this);
	}
	/**
	*/
	void frame_t::OnQuit(wxCommandEvent& event){
		// Destroy the frame
		if (timer.IsRunning())
			timer.Stop();
		Close();
	}
	/**
	*/
	void frame_t::OnActivate(wxActivateEvent& event){
		if (event.GetActive()){
			if (timer.IsRunning())
				timer.Stop();
			timer.Start(10);
		}
		else{
			if (timer.IsRunning())
				timer.Stop();
		}
	}
	/**
	*/
	void frame_t::onTimer(wxTimerEvent& event){
		if (updateBag)
			updateBag->update(event.GetInterval() / 1000.0f);
	}
	/**
	*/
	void frame_t::toggleTimerUpdate(){
		if (timer.IsRunning())
			timer.Stop();
		timer.Start(10);
	}
	/**
	*/
	void frame_t::OnNew(wxCommandEvent& event){
		resetTool();
		GetEditor()->GetEditorScene()->reset(true);
		documentName = "";
		return;
		if (docInfoList->size() > 1){
			wxString message = wxT("select a document type");
			wxString caption = wxT("select document type");
			wxArrayString array;
			docInfo_t **data = new docInfo_t*[docInfoList->size()];
			int count(0);
			size_t maxNameSize(0);
			size_t maxDescSize(0);
			for (docInfoList_t::iterator itr(docInfoList->begin()); itr != docInfoList->end(); ++itr){
				maxNameSize = std::max(maxNameSize, (*itr).name.length());
				maxDescSize = std::max(maxDescSize, (*itr).desc.length());
			}
			maxNameSize *= 2;
			maxDescSize *= 2;
			wxString buf;
			wxString fmt;
			for (docInfoList_t::iterator itr(docInfoList->begin()); itr != docInfoList->end(); ++itr){
				//wxSprintf(fmt,"%% -%ds%%s",maxNameSize);
				fmt.sprintf(fmt, "%% -%ds%%s", maxNameSize);
				buf.Printf(fmt, (*itr).name.c_str(), (*itr).desc.c_str());
				array.push_back(buf);
				data[count] = &(*itr);
				++count;
			}
			docInfo_t *ret = (docInfo_t *)wxGetSingleChoiceData(message, caption, array, (void **)data, this);
			if (ret){
				wxString path = ret->dllName.c_str();
				wxString entryName = ret->entryFunName.c_str();
				//Nick				createDocument(path, entryName);
			}
			delete[]data;
		}
		else if (docInfoList->size() == 1){
			wxString path = (*docInfoList)[0].dllName.c_str();
			wxString entryName = (*docInfoList)[0].entryFunName.c_str();
			//Nick			createDocument(path, entryName);
		}
		else{
			wxString msg;
			msg.Printf(wxT("no plugin defined in docInfoList.xml"));
			wxMessageBox(msg, wxT("error"));
		}
	}

	/**
	*/
	void frame_t::OnOpen(wxCommandEvent& event)
	{
		if (documentName.length() != 0){
			if (wxMessageBox("do you want save old scene", "", wxYES_NO, this) == wxYES)
				OnSave(event);
		}
		wxFileDialog fd(this, "select scene file to open", wxFileName::GetCwd(), wxEmptyString, "*.scene.xml", wxFD_OPEN);
		if (fd.ShowModal() == wxID_OK){
			documentName = fd.GetPath();
			wxString status;
			status.Printf("opening file %s", documentName);
			SetStatusText(status);
			resetTool();
			GetEditor()->GetEditorScene()->load(documentName, true);
			status.Printf("open scene file %s success", documentName);
			SetStatusText(status);
		}
	}

	/**
	*/
	void frame_t::OnRunScript(wxCommandEvent& event)
	{
		if (documentName.length() != 0){
			if (wxMessageBox("do you want save old scene", "", wxYES_NO, this) == wxYES)
				OnSave(event);
		}
		wxFileDialog fd(this, "select script file to open", wxFileName::GetCwd(), wxEmptyString, "*.lua", wxFD_OPEN);
		if (fd.ShowModal() == wxID_OK)
		{
			wxFileName m_scriptName(fd.GetPath());
			documentName = m_scriptName.GetFullName();
			wxString status;
			status.Printf("running script file %s", documentName);
			SetStatusText(status);
			int resI = API::RunScript(documentName);
			wxString resS = "NOT LOADED";

			(resI) ? resS = "with error,see log" : resS = "success";
			status.Printf("running script file %s %s", documentName, resS);
			SetStatusText(status);
		}
	}
	/**
	*/
	void frame_t::OnSave(wxCommandEvent& event){
		wxString status;
		if (documentName.length() != 0){
			status.Printf("saving file %s", documentName);
			SetStatusText(status);
			GetEditor()->GetEditorScene()->save(documentName, true);
			status.Printf("save scene to file %s scuccess", documentName);
			SetStatusText(status);
		}
		else{
			wxFileDialog fd(this, "select scene file to save", wxFileName::GetCwd(), wxEmptyString, "*.scene.xml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
			if (fd.ShowModal() == wxID_OK){
				documentName = fd.GetPath();
				status.Printf("saving file %s", documentName);
				SetStatusText(status);
				GetEditor()->GetEditorScene()->save(documentName, true);
				status.Printf("save scene to file %s scuccess", documentName);
				SetStatusText(status);
			}
		}
	}
	/**
	*/
	void frame_t::OnSaveAs(wxCommandEvent& event){
		wxString status;
		wxFileDialog fd(this, "select scene file", wxFileName::GetCwd(), wxEmptyString, "*.scene.xml", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
		if (fd.ShowModal() == wxID_OK){
			documentName = fd.GetPath();
			status.Printf("saving file %s", documentName);
			SetStatusText(status);
			GetEditor()->GetEditorScene()->save(documentName, true);
			status.Printf("save scene to file %s scuccess", documentName);
			SetStatusText(status);
		}
	}
	/**
	*/
	frame_t::~frame_t()
	{
		int ret = MessageBoxA(NULL, "you scene has been changed,do you want to save your current scene", "alarm", MB_YESNO | MB_ICONQUESTION | MB_TASKMODAL);
		if (ret == IDYES)
			OnSave(wxCommandEvent());
		SAFE_DELETE(updateBag);
		SAFE_DELETE(propBag);
		SAFE_DELETE(moveObjectPropBag);
		viewWindow->PopEventHandler(true);
		SAFE_DELETE(docInfoList);
		fm->UnInit();
		SAFE_DELETE(fm);
		SAFE_DELETE(toolBag);
		SAFE_DELETE(viewMsgRecvToolBag);
		SAFE_DELETE(log);
	}
	/**
	*/
	class viewMsgHandle_t :public wxEvtHandler{
		frame_t *frame;
	public:
		/**
		*/
		~viewMsgHandle_t(){}
		/**
		*/
		viewMsgHandle_t(frame_t *frame) :frame(frame){}
		/**
		*/
		bool ProcessEvent(wxEvent &  event){
			return frame->processViewEvent(event);
		}
	};

	/**
	*/
	frame_t::frame_t(const wxString& title) :
		wxFrame(NULL, ID_FRAME, title, wxDefaultPosition, wxSize(1024, 768)),
		docInfoList(NULL), timer(this, ID_UPDATE_TIMER),
		currentViewMsgRecvTool(NULL)
	{
		propBag = new propBag_t;
		moveObjectPropBag = new moveObjectPropBag_t;
		fm = new wxAuiManager;
		updateBag = new updateBag_t;
		frame = this;
		toolBag = new toolBag_t(true);
		viewMsgRecvToolBag = new toolBag_t(false);
		fm->SetFrame(this);
		SetIcon(wxIcon(wxT("iconMain.ico")));

		// Create a menu bar
		wxMenu *fileMenu = new wxMenu;

		// The "About" item should be in the help menu
		wxMenu *helpMenu = new wxMenu;
		//wxMenu *testMenu=new wxMenu;
		helpMenu->Append(wxID_ABOUT, wxT("&About...\tF1"),
			wxT("Show about dialog"));
		fileMenu->Append(wxID_NEW, wxT("N&ew\tAlt-N"),
			wxT("Make a new document"));
		fileMenu->Append(wxID_OPEN, wxT("O&pen\tAlt-O"),
			wxT("open a document"));
		fileMenu->Append(ID_RUNSCRIPT, wxT("R&un script"),
			wxT("run script"));
		fileMenu->Append(wxID_SAVE, wxT("S&ave\tAlt-S"),
			wxT("save document"));
		fileMenu->Append(wxID_SAVEAS, wxT("Save A&s\tAlt-A"),
			wxT("save to another document"));
		fileMenu->Append(wxID_EXIT, wxT("E&xit\tAlt-X"),
			wxT("Quit this program"));
		// Now append the freshly created menu to the menu bar...
		menuBar = new wxMenuBar();
		menuBar->Append(fileMenu, wxT("&File"));
		menuBar->Append(helpMenu, wxT("&Help"));
		// ... and attach this menu bar to the frame
		SetMenuBar(menuBar);
		// Create a status bar just for fun
		CreateStatusBar(2);
		SetStatusText(wxT("Welcome to xgEditor!"));

		CreateEditorToolBar();

		propWindow = new wxChoicebookEx(this, wxID_ANY, wxDefaultPosition, wxSize(256, 50));
		fm->AddPane(propWindow, wxAuiPaneInfo().Name(wxT("property")).CloseButton(false).Caption(wxT("property")).Left());
		toolWindow = new wxTreebook(this, wxID_ANY, wxDefaultPosition, wxSize(200, 128), wxBK_TOP);
		fm->AddPane(toolWindow, wxAuiPaneInfo().Name(wxT("tools")).Caption(wxT("tool")).CloseButton(false).Right().BestSize(300, 100).MinSize(300, 100));

		viewWindow = new wxOgre_t(this);
		fm->AddPane(viewWindow, wxAuiPaneInfo().Name(wxT("view")).Caption(wxT("workspace")).Center().CloseButton(false).CaptionVisible(false));
		logWindow = new wxTextCtrl(this, wxID_ANY, L"", wxDefaultPosition, wxSize(128, 64), wxTE_RICH | wxTE_MULTILINE | wxTE_AUTO_URL);

		log = new log_t();
		log->setLogWindow(logWindow);
		fm->AddPane(logWindow, wxAuiPaneInfo().Name(wxT("log")).Caption(wxT("log")).Bottom());

		initTool();

		viewMsgHandle_t *viewMsgHandle = new viewMsgHandle_t(this);
		viewWindow->PushEventHandler(viewMsgHandle);
		toggleTimerUpdate();
		Center();

		fm->Update();

		UpdateStatusText();
	}
	/**
	*/
	bool frame_t::processViewEvent(wxEvent &  event){
		if (wxIS_KIND_OF((&event), wxMouseEvent)){
			wxMouseEvent &e = *wxStaticCastEvent(wxMouseEvent*, &event);
			viewMsgRecvToolBag->onViewMouseEvent(e);
			if (currentViewMsgRecvTool){
				currentViewMsgRecvTool->onViewMouseEvent(e);
			}
		}
		else if (wxIS_KIND_OF((&event), wxKeyEvent)){
			wxKeyEvent &e = *wxStaticCastEvent(wxKeyEvent*, &event);
			//		viewMsgRecvToolBag->onViewKeyEvent(e);
			if (currentViewMsgRecvTool){
				currentViewMsgRecvTool->onViewKeyEvent(e);
			}
		}
		return false;
	}
	/**
	*/
	void frame_t::pushViewMsgRecvTool(tool_t *me){
		viewMsgRecvToolBag->push_back(me);
	}
	/**
	*/
	void frame_t::popViewMsgRecvTool(tool_t *me){
		viewMsgRecvToolBag->remove(me);
	}
	/**
	*/
	void frame_t::setViewMsgRecvTool(tool_t *me){
		currentViewMsgRecvTool = me;
	}
	/**
	*/
	bool frame_t::ProcessEvent(wxEvent &  event){
		/*if(event.GetEventObject()==viewWindow){
		switchHub.onViewEvent(event);
		return false;
		}*/
		//scatterHub->onEvent(event);
		/*switchHub->onEvent(event);
		propBag->onEvent(event);*/
		return wxEvtHandler::ProcessEvent(event);
	}
	/**
	*/
	updateBag_t *frame_t::getUpdateBag()
	{
		return updateBag;
	}
	/**
	*/
	void frame_t::CreateEditorToolBar()
	{
		toolbar = new WxMainToolBar(this, -1);
		this->SetToolBar(toolbar);
	}
	/**
	*/
	void frame_t::UpdateStatusText(){
		if (GetEditor()->mEngineState == EngineWrapper::EngineState::ES_PAUSE)
			SetStatusText(wxT("Ready to play"));
		if (GetEditor()->mEngineState == EngineWrapper::EngineState::ES_PLAY)
			SetStatusText(wxT("Playing"));
		if (GetEditor()->mEngineState == EngineWrapper::EngineState::ES_LOADING)
			SetStatusText(wxT("Loading"));
		SetStatusText(wxT("Ready"));
	}
}