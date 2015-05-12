#include "StdAfx.h"
#include "wxTreebookEx.h"
BEGIN_EVENT_TABLE(wxTreebookEx, wxTreebook)
EVT_TREEBOOK_PAGE_CHANGED(wxID_ANY, wxTreebookEx::OnSelChange)
EVT_CHILD_FOCUS(wxTreebookEx::OnChildFocus)
END_EVENT_TABLE()
wxTreebookEx::~wxTreebookEx(void){
}
wxTreebookEx::wxTreebookEx(wxWindow *parent,
			 wxWindowID id,
			 const wxPoint& pos,
			 const wxSize& size,
			 long style,
			 const wxString& name):wxTreebook(parent,id,pos,size,style,name){}
void  wxTreebookEx::OnChildFocus (wxChildFocusEvent &event){
	if ( m_selection != -1 ) {
		if(m_pages[m_selection]){
			wxActivateEvent activeEvent(wxEVT_ACTIVATE,true,m_pages[m_selection]->GetId());
			activeEvent.ResumePropagation(wxEVENT_PROPAGATE_MAX);
			activeEvent.SetEventObject(m_pages[m_selection]);
			m_pages[m_selection]->GetEventHandler()->ProcessEvent(activeEvent);
		}
	}
}
void wxTreebookEx::OnSelChange (wxTreebookEvent &event){
	if ( event.GetEventObject() == this ){
		/*if (event.GetSelection() != m_selection )*/{
			int nOldSel=event.GetOldSelection();
			int nSel=event.GetSelection();
			wxASSERT( nOldSel != nSel ); // impossible
			if ( nOldSel != -1 ) {
				if(m_pages[nOldSel]){
					wxActivateEvent activeEvent(wxEVT_ACTIVATE,false,m_pages[nOldSel]->GetId());
					activeEvent.ResumePropagation(wxEVENT_PROPAGATE_MAX);
					activeEvent.SetEventObject(m_pages[nOldSel]);
					m_pages[nOldSel]->GetEventHandler()->ProcessEvent(activeEvent);
				}
			}
			wxNotebookPage *pPage = m_pages[nSel];
			if(pPage){
				wxActivateEvent activeEvent(wxEVT_ACTIVATE,true,pPage->GetId());
				activeEvent.ResumePropagation(wxEVENT_PROPAGATE_MAX);
				activeEvent.SetEventObject(pPage);
				pPage->GetEventHandler()->ProcessEvent(activeEvent);
			}
		}
	}
	// we want to give others a chance to process this message as well
	event.Skip();
}