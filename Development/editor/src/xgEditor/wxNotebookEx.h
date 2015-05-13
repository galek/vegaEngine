#pragma once

//##ModelId=44B8E81C03DB
class wxNotebookEx :
	public wxNotebook
{
	DECLARE_EVENT_TABLE()
public:
	//##ModelId=44B8E81C03DD
	wxNotebookEx(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0, const wxString &name = wxNotebookNameStr) :wxNotebook(parent, id, pos, size, style, name){
	}
	//##ModelId=44B8E81D0000
	void  OnSelChange(wxNotebookEvent &event);
	//##ModelId=44B8E81D0002
	wxNotebookEx(void);
	//##ModelId=44B8E81D0004
	~wxNotebookEx(void);
};
