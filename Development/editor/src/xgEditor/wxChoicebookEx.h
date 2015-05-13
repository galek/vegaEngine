#pragma once
#include "wx/choicebk.h"

class wxChoicebookEx :
	public wxChoicebook
{
	DECLARE_EVENT_TABLE()
public:
	wxChoicebookEx(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0, const wxString &name = wxEmptyString);
	void  OnSelChange(wxChoicebookEvent &event);
	wxChoicebookEx(void);
	~wxChoicebookEx(void);
};
