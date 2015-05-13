#pragma once

#include<wx/treebook.h>

class wxTreebookEx :
	public wxTreebook
{
	DECLARE_EVENT_TABLE()
public:
	wxTreebookEx(wxWindow *parent,
		wxWindowID id,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxBK_DEFAULT,
		const wxString& name = wxEmptyString);
public:
	void  OnChildFocus(wxChildFocusEvent &event);
	void  OnSelChange(wxTreebookEvent &event);
	~wxTreebookEx(void);
};
