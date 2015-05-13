#include "stdafx.h"
#include "wxChoicebookEx.h"
//#include <wx/listctrl.h> 
BEGIN_EVENT_TABLE(wxChoicebookEx, wxChoicebook)
EVT_CHOICEBOOK_PAGE_CHANGED(wxID_ANY, wxChoicebookEx::OnSelChange)
END_EVENT_TABLE()
wxChoicebookEx::wxChoicebookEx(void)
{
}

wxChoicebookEx::~wxChoicebookEx(void)
{
}
void  wxChoicebookEx::OnSelChange(wxChoicebookEvent &event){
	if (event.GetEventObject() == this){
		/*if (event.GetSelection() != m_selection )*/{
			int nOldSel = event.GetOldSelection();
			int nSel = event.GetSelection();
			wxASSERT(nOldSel != nSel); // impossible
			if (nOldSel != -1) {
				wxActivateEvent activeEvent(wxEVT_ACTIVATE, false, m_pages[nOldSel]->GetId());
				activeEvent.ResumePropagation(wxEVENT_PROPAGATE_MAX);
				activeEvent.SetEventObject(m_pages[nOldSel]);
				m_pages[nOldSel]->GetEventHandler()->ProcessEvent(activeEvent);
			}
			wxNotebookPage *pPage = m_pages[nSel];
			wxActivateEvent activeEvent(wxEVT_ACTIVATE, true, pPage->GetId());
			activeEvent.ResumePropagation(wxEVENT_PROPAGATE_MAX);
			activeEvent.SetEventObject(pPage);
			pPage->GetEventHandler()->ProcessEvent(activeEvent);
		}
	}
	// we want to give others a chance to process this message as well
	event.Skip();
}
wxChoicebookEx::wxChoicebookEx(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name) :wxChoicebook(parent, id, pos, size, style, name){
	//m_list->SetWindowStyleFlag (wxLC_LIST  );
}