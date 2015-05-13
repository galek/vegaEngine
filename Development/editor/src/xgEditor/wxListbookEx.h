#pragma once
#include "wx/listbook.h"

class wxListbookEx :
	public wxListbook
{
	DECLARE_EVENT_TABLE()
public:
	wxListbookEx(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0, const wxString &name = wxEmptyString);
	void  OnSelChange(wxListbookEvent &event);
	wxListbookEx(void);
	~wxListbookEx(void);
};
