#include "stdafx.h"
#include <wx/listbook.h>
#include <wx/treebook.h>
#include "testTool_t.h"
#include "msgScatterHub_t.h"
#include "msgSwitchHub_t.h"
#include <wx/wxchar.h>

#include"frame_t.h"
namespace vega
{
#define test_button_id 0
	BEGIN_EVENT_TABLE(testTool_t, tool_t)
		EVT_BUTTON(test_button_id, onButtonEvent)
		EVT_TEXT(1, onTextEvent)
		END_EVENT_TABLE()
		testTool_t::testTool_t(){
		wxWindow *wid = new wxWindow(frame->getToolBook(), wxID_ANY);
		wxButton *wb = new wxButton(wid, test_button_id, L"testTool_t");
		wxTextCtrl *tc = new wxTextCtrl(wid, 1, wxT("hellow"), wxPoint(0, 100));
		frame->getToolBook()->AddPage(wid, L"test");
		frame->getMsgSwitchHub()->link(wid, this);
	}
	//##ModelId=44BA0692038A
	void testTool_t::onButtonEvent(wxCommandEvent &event){
		//Beep(2000,200);
	}
	//##ModelId=44BA0693002E
	void testTool_t::onTextEvent(wxCommandEvent &event){
		//Beep(2000,200);
	}
}