#include "stdafx.h"	
#include <wx/wx.h>
#include <wx\notebook.h>
#include "wxnotebookex.h"
BEGIN_EVENT_TABLE(wxNotebookEx, wxNotebook)
EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, wxNotebookEx::OnSelChange)
END_EVENT_TABLE()

wxNotebookEx::wxNotebookEx(void){
}


void wxNotebookEx::OnSelChange(wxNotebookEvent &event){
	//wxNotebook::OnSelChange(event);
	if (event.GetEventObject() == this){
		if (event.GetSelection() != m_selection){
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

wxNotebookEx::~wxNotebookEx(void){
}
