#include "stdafx.h"
#include ".\wxlistbookex.h"
#include <wx/listctrl.h> 
BEGIN_EVENT_TABLE(wxListbookEx, wxListbook)
EVT_LISTBOOK_PAGE_CHANGED(wxID_ANY, wxListbookEx::OnSelChange)
END_EVENT_TABLE()
wxListbookEx::wxListbookEx(void)
{
}

wxListbookEx::~wxListbookEx(void)
{
}
void  wxListbookEx::OnSelChange(wxListbookEvent &event){
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
wxListbookEx::wxListbookEx(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name) :wxListbook(parent, id, pos, size, style, name){
	//m_list->SetWindowStyleFlag (wxLC_LIST  );
}