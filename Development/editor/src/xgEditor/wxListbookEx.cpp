/*
Copyright (c) 2005-2007, lidong wang

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* Neither the name of the lidongwang nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
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
void  wxListbookEx::OnSelChange (wxListbookEvent &event){
	if ( event.GetEventObject() == this ){
		/*if (event.GetSelection() != m_selection )*/{
			int nOldSel=event.GetOldSelection();
			int nSel=event.GetSelection();
			wxASSERT( nOldSel != nSel ); // impossible
			if ( nOldSel != -1 ) {
				wxActivateEvent activeEvent(wxEVT_ACTIVATE,false,m_pages[nOldSel]->GetId());
				activeEvent.ResumePropagation(wxEVENT_PROPAGATE_MAX);
				activeEvent.SetEventObject(m_pages[nOldSel]);
				m_pages[nOldSel]->GetEventHandler()->ProcessEvent(activeEvent);
			}
			wxNotebookPage *pPage = m_pages[nSel];
			wxActivateEvent activeEvent(wxEVT_ACTIVATE,true,pPage->GetId());
			activeEvent.ResumePropagation(wxEVENT_PROPAGATE_MAX);
			activeEvent.SetEventObject(pPage);
			pPage->GetEventHandler()->ProcessEvent(activeEvent);
		}
	}
	// we want to give others a chance to process this message as well
	event.Skip();
}
wxListbookEx::wxListbookEx (wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name):wxListbook(parent,id,pos,size,style,name){
		//m_list->SetWindowStyleFlag (wxLC_LIST  );
	}