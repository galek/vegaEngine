///////////////////////////////////////////////////////////////////////////////
// Name:        manager.h
// Purpose:     wxaui: wx advanced user interface - docking window manager
// Author:      Benjamin I. Williams
// Modified by:
// Created:     2005-05-17
// RCS-ID:      
// Copyright:   (C) Copyright 2005, Kirix Corporation, All Rights Reserved.
// Licence:     wxWindows Library Licence, Version 3.1
///////////////////////////////////////////////////////////////////////////////
#pragma once

namespace veg
{
	enum wxFrameManagerDock
	{
		//##ModelId=44B8E81D002F
		wxAUI_DOCK_NONE = 0,
		//##ModelId=44B8E81D003E
		wxAUI_DOCK_TOP = 1,
		//##ModelId=44B8E81D004E
		wxAUI_DOCK_RIGHT = 2,
		//##ModelId=44B8E81D006D
		wxAUI_DOCK_BOTTOM = 3,
		//##ModelId=44B8E81D007D
		wxAUI_DOCK_LEFT = 4,
		//##ModelId=44B8E81D008C
		wxAUI_DOCK_CENTER = 5,
		//##ModelId=44B8E81D009C
		wxAUI_DOCK_CENTRE = wxAUI_DOCK_CENTER
	};

	//##ModelId=44B8E81D00BB
	enum wxFrameManagerOption
	{
		//##ModelId=44B8E81D00CC
		wxAUI_MGR_ALLOW_FLOATING = 1 << 0,
		//##ModelId=44B8E81D00DA
		wxAUI_MGR_ALLOW_ACTIVE_PANE = 1 << 1,
		//##ModelId=44B8E81D00EA
		wxAUI_MGR_TRANSPARENT_DRAG = 1 << 2,
		//##ModelId=44B8E81D00FA
		wxAUI_MGR_TRANSPARENT_HINT = 1 << 3,
		//##ModelId=44B8E81D0109
		wxAUI_MGR_TRANSPARENT_HINT_FADE = 1 << 4,

		//##ModelId=44B8E81D0119
		wxAUI_MGR_DEFAULT = wxAUI_MGR_ALLOW_FLOATING |
		wxAUI_MGR_TRANSPARENT_HINT |
		wxAUI_MGR_TRANSPARENT_HINT_FADE
	};

	//##ModelId=44B8E81D0177
	enum wxPaneDockArtSetting
	{
		//##ModelId=44B8E81D0187
		wxAUI_ART_SASH_SIZE = 0,
		//##ModelId=44B8E81D0196
		wxAUI_ART_CAPTION_SIZE = 1,
		//##ModelId=44B8E81D01A5
		wxAUI_ART_GRIPPER_SIZE = 2,
		//##ModelId=44B8E81D01B5
		wxAUI_ART_PANE_BORDER_SIZE = 3,
		//##ModelId=44B8E81D01C5
		wxAUI_ART_PANE_BUTTON_SIZE = 4,
		//##ModelId=44B8E81D01D4
		wxAUI_ART_BACKGROUND_COLOUR = 5,
		//##ModelId=44B8E81D01E4
		wxAUI_ART_SASH_COLOUR = 6,
		//##ModelId=44B8E81D01F4
		wxAUI_ART_ACTIVE_CAPTION_COLOUR = 7,
		//##ModelId=44B8E81D01F5
		wxAUI_ART_ACTIVE_CAPTION_GRADIENT_COLOUR = 8,
		//##ModelId=44B8E81D0203
		wxAUI_ART_INACTIVE_CAPTION_COLOUR = 9,
		//##ModelId=44B8E81D0213
		wxAUI_ART_INACTIVE_CAPTION_GRADIENT_COLOUR = 10,
		//##ModelId=44B8E81D0222
		wxAUI_ART_ACTIVE_CAPTION_TEXT_COLOUR = 11,
		//##ModelId=44B8E81D0232
		wxAUI_ART_INACTIVE_CAPTION_TEXT_COLOUR = 12,
		//##ModelId=44B8E81D0242
		wxAUI_ART_BORDER_COLOUR = 13,
		//##ModelId=44B8E81D0251
		wxAUI_ART_GRIPPER_COLOUR = 14,
		//##ModelId=44B8E81D0261
		wxAUI_ART_CAPTION_FONT = 15,
		//##ModelId=44B8E81D0271
		wxAUI_ART_GRADIENT_TYPE = 16
	};

	//##ModelId=44B8E81D0290
	enum wxPaneDockArtGradients
	{
		//##ModelId=44B8E81D02A0
		wxAUI_GRADIENT_NONE = 0,
		//##ModelId=44B8E81D02AF
		wxAUI_GRADIENT_VERTICAL = 1,
		//##ModelId=44B8E81D02BF
		wxAUI_GRADIENT_HORIZONTAL = 2
	};

	//##ModelId=44B8E81D02DE
	enum wxPaneButtonState
	{
		//##ModelId=44B8E81D02EF
		wxAUI_BUTTON_STATE_NORMAL = 0,
		//##ModelId=44B8E81D02FD
		wxAUI_BUTTON_STATE_HOVER = 1,
		//##ModelId=44B8E81D030D
		wxAUI_BUTTON_STATE_PRESSED = 2
	};

	//##ModelId=44B8E81D032C
	enum wxPaneInsertLevel
	{
		//##ModelId=44B8E81D033C
		wxAUI_INSERT_PANE = 0,
		//##ModelId=44B8E81D033D
		wxAUI_INSERT_ROW = 1,
		//##ModelId=44B8E81D034B
		wxAUI_INSERT_DOCK = 2
	};



	// forwards and array declarations
	class wxDockUIPart;
	class wxPaneButton;
	class wxPaneInfo;
	class wxDockInfo;
	class wxDockArt;
	class wxFrameManagerEvent;

	WX_DECLARE_OBJARRAY(wxDockInfo, wxDockInfoArray);
	WX_DECLARE_OBJARRAY(wxDockUIPart, wxDockUIPartArray);
	WX_DECLARE_OBJARRAY(wxPaneButton, wxPaneButtonArray);
	WX_DECLARE_OBJARRAY(wxPaneInfo, wxPaneInfoArray);
	WX_DEFINE_ARRAY_PTR(wxPaneInfo*, wxPaneInfoPtrArray);
	WX_DEFINE_ARRAY_PTR(wxDockInfo*, wxDockInfoPtrArray);

	extern wxDockInfo wxNullDockInfo;
	extern wxPaneInfo wxNullPaneInfo;




	//##ModelId=44B8E81E00CB
	class WXDLLEXPORT wxPaneInfo
	{
	public:

		//##ModelId=44B8E81E00DA
		wxPaneInfo()
		{
			window = NULL;
			frame = NULL;
			state = 0;
			dock_direction = wxAUI_DOCK_LEFT;
			dock_layer = 0;
			dock_row = 0;
			dock_pos = 0;
			floating_pos = wxDefaultPosition;
			floating_size = wxDefaultSize;
			best_size = wxDefaultSize;
			min_size = wxDefaultSize;
			max_size = wxDefaultSize;
			dock_proportion = 0;

			DefaultPane();
		}

		//##ModelId=44B8E81E00EA
		wxPaneInfo(const wxPaneInfo& c)
		{
			name = c.name;
			caption = c.caption;
			window = c.window;
			frame = c.frame;
			state = c.state;
			dock_direction = c.dock_direction;
			dock_layer = c.dock_layer;
			dock_row = c.dock_row;
			dock_pos = c.dock_pos;
			best_size = c.best_size;
			min_size = c.min_size;
			max_size = c.max_size;
			floating_pos = c.floating_pos;
			floating_size = c.floating_size;
			dock_proportion = c.dock_proportion;
			buttons = c.buttons;
			rect = c.rect;
		}

		//##ModelId=44B8E81E00FA
		wxPaneInfo& operator=(const wxPaneInfo& c)
		{
			name = c.name;
			caption = c.caption;
			window = c.window;
			frame = c.frame;
			state = c.state;
			dock_direction = c.dock_direction;
			dock_layer = c.dock_layer;
			dock_row = c.dock_row;
			dock_pos = c.dock_pos;
			best_size = c.best_size;
			min_size = c.min_size;
			max_size = c.max_size;
			floating_pos = c.floating_pos;
			floating_size = c.floating_size;
			dock_proportion = c.dock_proportion;
			buttons = c.buttons;
			rect = c.rect;
			return *this;
		}

		//##ModelId=44B8E81E00FC
		bool IsOk() const { return (window != NULL) ? true : false; }
		//##ModelId=44B8E81E010A
		bool IsFixed() const { return !HasFlag(optionResizable); }
		//##ModelId=44B8E81E0119
		bool IsResizable() const { return HasFlag(optionResizable); }
		//##ModelId=44B8E81E011B
		bool IsShown() const { return !HasFlag(optionHidden); }
		//##ModelId=44B8E81E0128
		bool IsFloating() const { return HasFlag(optionFloating); }
		//##ModelId=44B8E81E0138
		bool IsDocked() const { return !HasFlag(optionFloating); }
		//##ModelId=44B8E81E013A
		bool IsToolbar() const { return HasFlag(optionToolbar); }
		//##ModelId=44B8E81E0149
		bool IsTopDockable() const { return HasFlag(optionTopDockable); }
		//##ModelId=44B8E81E0157
		bool IsBottomDockable() const { return HasFlag(optionBottomDockable); }
		//##ModelId=44B8E81E0159
		bool IsLeftDockable() const { return HasFlag(optionLeftDockable); }
		//##ModelId=44B8E81E0167
		bool IsRightDockable() const { return HasFlag(optionRightDockable); }
		//##ModelId=44B8E81E0177
		bool IsFloatable() const { return HasFlag(optionFloatable); }
		//##ModelId=44B8E81E0179
		bool IsMovable() const { return HasFlag(optionMovable); }
		//##ModelId=44B8E81E0186
		bool HasCaption() const { return HasFlag(optionCaption); }
		//##ModelId=44B8E81E0188
		bool HasGripper() const { return HasFlag(optionGripper); }
		//##ModelId=44B8E81E0196
		bool HasBorder() const { return HasFlag(optionPaneBorder); }
		//##ModelId=44B8E81E01A5
		bool HasCloseButton() const { return HasFlag(buttonClose); }
		//##ModelId=44B8E81E01A7
		bool HasMaximizeButton() const { return HasFlag(buttonMaximize); }
		//##ModelId=44B8E81E01B5
		bool HasMinimizeButton() const { return HasFlag(buttonMinimize); }
		//##ModelId=44B8E81E01C5
		bool HasPinButton() const { return HasFlag(buttonPin); }

		//##ModelId=44B8E81E01C7
		wxPaneInfo& Window(wxWindow* w) { window = w; return *this; }
		//##ModelId=44B8E81E01D4
		wxPaneInfo& Name(const wxString& n) { name = n; return *this; }
		//##ModelId=44B8E81E01D6
		wxPaneInfo& Caption(const wxString& c) { caption = c; return *this; }
		//##ModelId=44B8E81E01E5
		wxPaneInfo& Left() { dock_direction = wxAUI_DOCK_LEFT; return *this; }
		//##ModelId=44B8E81E01F4
		wxPaneInfo& Right() { dock_direction = wxAUI_DOCK_RIGHT; return *this; }
		//##ModelId=44B8E81E01F5
		wxPaneInfo& Top() { dock_direction = wxAUI_DOCK_TOP; return *this; }
		//##ModelId=44B8E81E0203
		wxPaneInfo& Bottom() { dock_direction = wxAUI_DOCK_BOTTOM; return *this; }
		//##ModelId=44B8E81E0204
		wxPaneInfo& Center() { dock_direction = wxAUI_DOCK_CENTER; return *this; }
		//##ModelId=44B8E81E0213
		wxPaneInfo& Centre() { dock_direction = wxAUI_DOCK_CENTRE; return *this; }
		//##ModelId=44B8E81E0214
		wxPaneInfo& Direction(int direction) { dock_direction = direction; return *this; }
		//##ModelId=44B8E81E0222
		wxPaneInfo& Layer(int layer) { dock_layer = layer; return *this; }
		//##ModelId=44B8E81E0232
		wxPaneInfo& Row(int row) { dock_row = row; return *this; }
		//##ModelId=44B8E81E0234
		wxPaneInfo& Position(int pos) { dock_pos = pos; return *this; }
		//##ModelId=44B8E81E0242
		wxPaneInfo& BestSize(const wxSize& size) { best_size = size; return *this; }
		//##ModelId=44B8E81E0251
		wxPaneInfo& MinSize(const wxSize& size) { min_size = size; return *this; }
		//##ModelId=44B8E81E0253
		wxPaneInfo& MaxSize(const wxSize& size) { max_size = size; return *this; }
		//##ModelId=44B8E81E0261
		wxPaneInfo& BestSize(int x, int y) { best_size.Set(x, y); return *this; }
		//##ModelId=44B8E81E0271
		wxPaneInfo& MinSize(int x, int y) { min_size.Set(x, y); return *this; }
		//##ModelId=44B8E81E0274
		wxPaneInfo& MaxSize(int x, int y) { max_size.Set(x, y); return *this; }
		//##ModelId=44B8E81E0280
		wxPaneInfo& FloatingPosition(const wxPoint& pos) { floating_pos = pos; return *this; }
		//##ModelId=44B8E81E0290
		wxPaneInfo& FloatingPosition(int x, int y) { floating_pos.x = x; floating_pos.y = y; return *this; }
		//##ModelId=44B8E81E0293
		wxPaneInfo& FloatingSize(const wxSize& size) { floating_size = size; return *this; }
		//##ModelId=44B8E81E029F
		wxPaneInfo& FloatingSize(int x, int y) { floating_size.Set(x, y); return *this; }
		//##ModelId=44B8E81E02AF
		wxPaneInfo& Fixed() { return SetFlag(optionResizable, false); }
		//##ModelId=44B8E81E02B0
		wxPaneInfo& Resizable(bool resizable = true) { return SetFlag(optionResizable, resizable); }
		//##ModelId=44B8E81E02BF
		wxPaneInfo& Dock() { return SetFlag(optionFloating, false); }
		//##ModelId=44B8E81E02C0
		wxPaneInfo& Float() { return SetFlag(optionFloating, true); }
		//##ModelId=44B8E81E02CE
		wxPaneInfo& Hide() { return SetFlag(optionHidden, true); }
		//##ModelId=44B8E81E02CF
		wxPaneInfo& Show(bool show = true) { return SetFlag(optionHidden, !show); }
		//##ModelId=44B8E81E02DE
		wxPaneInfo& CaptionVisible(bool visible = true) { return SetFlag(optionCaption, visible); }
		//##ModelId=44B8E81E02EE
		wxPaneInfo& PaneBorder(bool visible = true) { return SetFlag(optionPaneBorder, visible); }
		//##ModelId=44B8E81E02F0
		wxPaneInfo& Gripper(bool visible = true) { return SetFlag(optionGripper, visible); }
		//##ModelId=44B8E81E02FD
		wxPaneInfo& CloseButton(bool visible = true) { return SetFlag(buttonClose, visible); }
		//##ModelId=44B8E81E02FF
		wxPaneInfo& MaximizeButton(bool visible = true) { return SetFlag(buttonMaximize, visible); }
		//##ModelId=44B8E81E030D
		wxPaneInfo& MinimizeButton(bool visible = true) { return SetFlag(buttonMinimize, visible); }
		//##ModelId=44B8E81E031C
		wxPaneInfo& PinButton(bool visible = true) { return SetFlag(buttonPin, visible); }
		//##ModelId=44B8E81E031E
		wxPaneInfo& DestroyOnClose(bool b = true) { return SetFlag(optionDestroyOnClose, b); }
		//##ModelId=44B8E81E032C
		wxPaneInfo& TopDockable(bool b = true) { return SetFlag(optionTopDockable, b); }
		//##ModelId=44B8E81E032E
		wxPaneInfo& BottomDockable(bool b = true) { return SetFlag(optionBottomDockable, b); }
		//##ModelId=44B8E81E033C
		wxPaneInfo& LeftDockable(bool b = true) { return SetFlag(optionLeftDockable, b); }
		//##ModelId=44B8E81E034B
		wxPaneInfo& RightDockable(bool b = true) { return SetFlag(optionRightDockable, b); }
		//##ModelId=44B8E81E034D
		wxPaneInfo& Floatable(bool b = true) { return SetFlag(optionFloatable, b); }
		//##ModelId=44B8E81E035B
		wxPaneInfo& Movable(bool b = true) { return SetFlag(optionMovable, b); }
		//##ModelId=44B8E81E035D
		wxPaneInfo& Dockable(bool b = true)
		{
			return TopDockable(b).BottomDockable(b).LeftDockable(b).RightDockable(b);
		}

		//##ModelId=44B8E81E036B
		wxPaneInfo& DefaultPane()
		{
			state |= optionTopDockable | optionBottomDockable |
				optionLeftDockable | optionRightDockable |
				optionFloatable | optionMovable | optionResizable |
				optionCaption | optionPaneBorder | buttonClose;
			return *this;
		}

		//##ModelId=44B8E81E036C
		wxPaneInfo& CentrePane() { return CenterPane(); }
		//##ModelId=44B8E81E037A
		wxPaneInfo& CenterPane()
		{
			state = 0;
			return Center().PaneBorder().Resizable();
		}

		//##ModelId=44B8E81E037B
		wxPaneInfo& ToolbarPane()
		{
			DefaultPane();
			state |= (optionToolbar | optionGripper);
			state &= ~(optionResizable | optionCaption);
			if (dock_layer == 0)
				dock_layer = 10;
			return *this;
		}

		//##ModelId=44B8E81E038A
		wxPaneInfo& SetFlag(unsigned int flag, bool option_state)
		{
			if (option_state)
				state |= flag;
			else
				state &= ~flag;
			return *this;
		}

		//##ModelId=44B8E81E038D
		bool HasFlag(unsigned int flag) const
		{
			return (state & flag) ? true : false;
		}

	public:

		//##ModelId=44B8E81F008C
		enum wxPaneState
		{
			//##ModelId=44B8E81F009D
			optionFloating = 1 << 0,
			//##ModelId=44B8E81F009E
			optionHidden = 1 << 1,
			//##ModelId=44B8E81F00AB
			optionLeftDockable = 1 << 2,
			//##ModelId=44B8E81F00AC
			optionRightDockable = 1 << 3,
			//##ModelId=44B8E81F00BB
			optionTopDockable = 1 << 4,
			//##ModelId=44B8E81F00BC
			optionBottomDockable = 1 << 5,
			//##ModelId=44B8E81F00CB
			optionFloatable = 1 << 6,
			//##ModelId=44B8E81F00DA
			optionMovable = 1 << 7,
			//##ModelId=44B8E81F00DB
			optionResizable = 1 << 8,
			//##ModelId=44B8E81F00DC
			optionPaneBorder = 1 << 9,
			//##ModelId=44B8E81F00EA
			optionCaption = 1 << 10,
			//##ModelId=44B8E81F00EB
			optionGripper = 1 << 11,
			//##ModelId=44B8E81F00FA
			optionDestroyOnClose = 1 << 12,
			//##ModelId=44B8E81F00FB
			optionToolbar = 1 << 13,
			//##ModelId=44B8E81F0109
			optionActive = 1 << 14,

			//##ModelId=44B8E81F010A
			buttonClose = 1 << 24,
			//##ModelId=44B8E81F0119
			buttonMaximize = 1 << 25,
			//##ModelId=44B8E81F011A
			buttonMinimize = 1 << 26,
			//##ModelId=44B8E81F0128
			buttonPin = 1 << 27,
			//##ModelId=44B8E81F0129
			buttonCustom1 = 1 << 28,
			//##ModelId=44B8E81F0138
			buttonCustom2 = 1 << 29,
			//##ModelId=44B8E81F0139
			buttonCustom3 = 1 << 30,
			//##ModelId=44B8E81F0148
			actionPane = 1 << 31  // used internally
		};

	public:
		//##ModelId=44B8E81E039A
		wxString name;        // name of the pane
		//##ModelId=44B8E81E03AA
		wxString caption;     // caption displayed on the window

		//##ModelId=44B8E81E03BA
		wxWindow* window;     // window that is in this pane
		//##ModelId=44B8E81E03BF
		wxWindow* frame;      // floating frame window that holds the pane
		//##ModelId=44B8E81E03C8
		unsigned int state;   // a combination of wxPaneState values

		//##ModelId=44B8E81E03D8
		int dock_direction;   // dock direction (top, bottom, left, right, center)
		//##ModelId=44B8E81E03D9
		int dock_layer;       // layer number (0 = innermost layer)
		//##ModelId=44B8E81F0000
		int dock_row;         // row number on the docking bar (0 = first row)
		//##ModelId=44B8E81F0001
		int dock_pos;         // position inside the row (0 = first position)

		//##ModelId=44B8E81F0010
		wxSize best_size;     // size that the layout engine will prefer
		//##ModelId=44B8E81F0015
		wxSize min_size;      // minimum size the pane window can tolerate
		//##ModelId=44B8E81F0020
		wxSize max_size;      // maximum size the pane window can tolerate

		//##ModelId=44B8E81F002F
		wxPoint floating_pos; // position while floating
		//##ModelId=44B8E81F0034
		wxSize floating_size; // size while floating
		//##ModelId=44B8E81F003E
		int dock_proportion;  // proportion while docked

		//##ModelId=44B8E81F004F
		wxPaneButtonArray buttons; // buttons on the pane

		//##ModelId=44B8E81F005E
		wxRect rect;              // current rectangle (populated by wxAUI)
	};





	//##ModelId=44B8E820000F
	class WXDLLEXPORT wxFrameManager : public wxEvtHandler
	{
		friend class wxFloatingPane;

	public:

		//##ModelId=44B8E8200011
		wxFrameManager(wxFrame* frame = NULL,
			unsigned int flags = wxAUI_MGR_DEFAULT);
		//##ModelId=44B8E8200109
		virtual ~wxFrameManager();
		//##ModelId=44B8E82001E4
		void UnInit();

		//##ModelId=44B8E82002BF
		void SetFlags(unsigned int flags);
		//##ModelId=44B8E8200399
		unsigned int GetFlags() const;

		//##ModelId=44B8E821008C
		void SetFrame(wxFrame* frame);
		//##ModelId=44B8E8210167
		wxFrame* GetFrame() const;

		//##ModelId=44B8E8210242
		void SetArtProvider(wxDockArt* art_provider);
		//##ModelId=44B8E821031C
		wxDockArt* GetArtProvider() const;

		//##ModelId=44B8E822000F
		wxPaneInfo& GetPane(wxWindow* window);
		//##ModelId=44B8E82200EA
		wxPaneInfo& GetPane(const wxString& name);
		//##ModelId=44B8E82201D4
		wxPaneInfoArray& GetAllPanes();

		//##ModelId=44B8E82202AF
		bool AddPane(wxWindow* window,
			const wxPaneInfo& pane_info);

		//##ModelId=44B8E822037A
		bool AddPane(wxWindow* window,
			int direction = wxLEFT,
			const wxString& caption = wxEmptyString);

		//##ModelId=44B8E823005D
		bool InsertPane(wxWindow* window,
			const wxPaneInfo& pane_info,
			int insert_level = wxAUI_INSERT_PANE);

		//##ModelId=44B8E8230128
		bool DetachPane(wxWindow* window);

		//##ModelId=44B8E82301E4
		wxString SavePerspective();

		//##ModelId=44B8E823029F
		bool LoadPerspective(const wxString& layout,
			bool update = true);

		//##ModelId=44B8E823035B
		void Update();

	private:

		//##ModelId=44B8E82303A9
		void DrawHintRect(wxWindow* pane_window,
			const wxPoint& pt,
			const wxPoint& offset);

		//##ModelId=44B8E82303C8
		void DoFrameLayout();

		//##ModelId=44B8E824001F
		void LayoutAddPane(wxSizer* cont,
			wxDockInfo& dock,
			wxPaneInfo& pane,
			wxDockUIPartArray& uiparts,
			bool spacer_only);

		//##ModelId=44B8E82400BB
		void LayoutAddDock(wxSizer* cont,
			wxDockInfo& dock,
			wxDockUIPartArray& uiparts,
			bool spacer_only);

		//##ModelId=44B8E8240138
		wxSizer* LayoutAll(wxPaneInfoArray& panes,
			wxDockInfoArray& docks,
			wxDockUIPartArray& uiparts,
			bool spacer_only = false);

		//##ModelId=44B8E82401B5
		bool DoDrop(wxDockInfoArray& docks,
			wxPaneInfoArray& panes,
			wxPaneInfo& target,
			const wxPoint& pt,
			const wxPoint& offset = wxPoint(0, 0));

		//##ModelId=44B8E82401F4
		wxPaneInfo& LookupPane(wxWindow* window);
		//##ModelId=44B8E82401F6
		wxPaneInfo& LookupPane(const wxString& name);
		//##ModelId=44B8E82401F8
		wxDockUIPart* HitTest(int x, int y);
		//##ModelId=44B8E82402EE
		wxDockUIPart* GetPanePart(wxWindow* wnd);
		//##ModelId=44B8E824031C
		int GetDockPixelOffset(wxPaneInfo& test);
		//##ModelId=44B8E824036B
		void OnFloatingPaneMoveStart(wxWindow* wnd);
		//##ModelId=44B8E824037A
		void OnFloatingPaneMoving(wxWindow* wnd);
		//##ModelId=44B8E8240399
		void OnFloatingPaneMoved(wxWindow* wnd);
		//##ModelId=44B8E82403A9
		void OnFloatingPaneActivated(wxWindow* wnd);
		//##ModelId=44B8E82403B9
		void OnFloatingPaneClosed(wxWindow* wnd);
		//##ModelId=44B8E82403C8
		void OnFloatingPaneResized(wxWindow* wnd, const wxSize& size);
		//##ModelId=44B8E8250000
		void Render(wxDC* dc);
		//##ModelId=44B8E825000F
		void Repaint(wxDC* dc = NULL);
		//##ModelId=44B8E825001F
		void ProcessMgrEvent(wxFrameManagerEvent& event);
		//##ModelId=44B8E82500FA
		void UpdateButtonOnScreen(wxDockUIPart* button_ui_part,
			const wxMouseEvent& event);
		//##ModelId=44B8E8250109
		void GetPanePositionsAndSizes(wxDockInfo& dock,
			wxArrayInt& positions,
			wxArrayInt& sizes);
		//##ModelId=44B8E82501A5
		void ShowHint(const wxRect& rect);
		//##ModelId=44B8E82501C5
		void HideHint();
		//##ModelId=44B8E82501E4
		void RemoveHint();

	private:

		// events
		//##ModelId=44B8E82501E5
		void OnPaint(wxPaintEvent& event);
		//##ModelId=44B8E82501F5
		void OnEraseBackground(wxEraseEvent& event);
		//##ModelId=44B8E8250204
		void OnSize(wxSizeEvent& event);
		//##ModelId=44B8E8250214
		void OnSetCursor(wxSetCursorEvent& event);
		//##ModelId=44B8E8250223
		void OnLeftDown(wxMouseEvent& event);
		//##ModelId=44B8E8250233
		void OnLeftUp(wxMouseEvent& event);
		//##ModelId=44B8E8250242
		void OnMotion(wxMouseEvent& event);
		//##ModelId=44B8E8250251
		void OnLeaveWindow(wxMouseEvent& event);
		//##ModelId=44B8E8250253
		void OnPaneButton(wxFrameManagerEvent& event);
		//##ModelId=44B8E8250255
		void OnChildFocus(wxChildFocusEvent& event);
		//##ModelId=44B8E8250261
		void OnHintFadeTimer(wxTimerEvent& event);

	private:

		enum
		{
			actionNone = 0,
			actionResize,
			actionClickButton,
			actionClickCaption,
			actionDragToolbarPane,
			actionDragFloatingPane
		};

	private:

		//##ModelId=44B8E8250281
		wxFrame* m_frame;            // the frame being managed
		//##ModelId=44B8E8250291
		wxDockArt* m_art;            // dock art object which does all drawing
		//##ModelId=44B8E8250295
		unsigned int m_flags;        // manager flags wxAUI_MGR_*

		//##ModelId=44B8E82502A0
		wxPaneInfoArray m_panes;     // array of panes structures
		//##ModelId=44B8E82502A5
		wxDockInfoArray m_docks;     // array of docks structures
		//##ModelId=44B8E82502B0
		wxDockUIPartArray m_uiparts; // array of UI parts (captions, buttons, etc)

		//##ModelId=44B8E82502B4
		int m_action;                // current mouse action
		//##ModelId=44B8E82502C0
		wxPoint m_action_start;      // position where the action click started
		//##ModelId=44B8E82502C5
		wxPoint m_action_offset;     // offset from upper left of the item clicked
		//##ModelId=44B8E82502CF
		wxDockUIPart* m_action_part; // ptr to the part the action happened to
		//##ModelId=44B8E82502DF
		wxWindow* m_action_window;   // action frame or window (NULL if none)
		//##ModelId=44B8E82502E4
		wxRect m_action_hintrect;    // hint rectangle for the action
		//##ModelId=44B8E82502EF
		wxDockUIPart* m_hover_button;// button uipart being hovered over
		//##ModelId=44B8E82502F4
		wxRect m_last_hint;          // last hint rectangle
		//##ModelId=44B8E82502FE
		wxPoint m_last_mouse_move;   // last mouse move position (see OnMotion)

		//##ModelId=44B8E825030E
		wxWindow* m_hint_wnd;        // transparent hint window (for now, only msw)
		//##ModelId=44B8E8250313
		wxTimer m_hint_fadetimer;    // transparent fade timer (for now, only msw)
		//##ModelId=44B8E825031E
		int m_hint_fadeamt;          // transparent fade amount (for now, only msw)

		DECLARE_EVENT_TABLE()
	};



	// event declarations/classes

	//##ModelId=44B8E825035B
	class WXDLLEXPORT wxFrameManagerEvent : public wxEvent
	{
	public:
		//##ModelId=44B8E825035D
		wxFrameManagerEvent(wxEventType type) : wxEvent(0, type)
		{
			pane = NULL;
			button = 0;
		}

		//##ModelId=44B8E825035F
		wxFrameManagerEvent(const wxFrameManagerEvent& c) : wxEvent(c)
		{
			pane = c.pane;
			button = c.button;
		}

		//##ModelId=44B8E825036B
		wxEvent *Clone() const { return new wxFrameManagerEvent(*this); }

		//##ModelId=44B8E825036D
		void SetPane(wxPaneInfo* p) { pane = p; }
		//##ModelId=44B8E825036F
		void SetButton(int b) { button = b; }
		//##ModelId=44B8E8250371
		wxPaneInfo* GetPane() { return pane; }
		//##ModelId=44B8E825037A
		int GetButton() { return button; }

	public:
		//##ModelId=44B8E825037C
		wxPaneInfo* pane;
		//##ModelId=44B8E8250380
		int button;
	};




	// dock art provider code - a dock provider provides all drawing
	// functionality to the wxAui dock manager.  This allows the dock
	// manager to have plugable look-and-feels

	//##ModelId=44B8E82503C8
	class wxDockArt
	{
	public:

		//##ModelId=44B8E82503D8
		wxDockArt() { }
		//##ModelId=44B8E82503D9
		virtual ~wxDockArt() { }

		//##ModelId=44B8E82503DB
		virtual int GetMetric(int id) = 0;
		//##ModelId=44B8E82503DE
		virtual void SetMetric(int id, int new_val) = 0;
		//##ModelId=44B8E8260001
		virtual void SetFont(int id, const wxFont& font) = 0;
		//##ModelId=44B8E8260005
		virtual wxFont GetFont(int id) = 0;
		//##ModelId=44B8E8260008
		virtual wxColour GetColour(int id) = 0;
		//##ModelId=44B8E8260011
		virtual void SetColour(int id, const wxColor& colour) = 0;
		//##ModelId=44B8E8260015
		wxColor GetColor(int id) { return GetColour(id); }
		//##ModelId=44B8E8260017
		void SetColor(int id, const wxColor& color) { SetColour(id, color); }

		//##ModelId=44B8E8260021
		virtual void DrawSash(wxDC& dc,
			int orientation,
			const wxRect& rect) = 0;

		//##ModelId=44B8E8260026
		virtual void DrawBackground(wxDC& dc,
			int orientation,
			const wxRect& rect) = 0;

		//##ModelId=44B8E826002F
		virtual void DrawCaption(wxDC& dc,
			const wxString& text,
			const wxRect& rect,
			wxPaneInfo& pane) = 0;

		//##ModelId=44B8E8260035
		virtual void DrawGripper(wxDC& dc,
			const wxRect& rect,
			wxPaneInfo& pane) = 0;

		//##ModelId=44B8E8260041
		virtual void DrawBorder(wxDC& dc,
			const wxRect& rect,
			wxPaneInfo& pane) = 0;

		//##ModelId=44B8E8260046
		virtual void DrawPaneButton(wxDC& dc,
			int button,
			int button_state,
			const wxRect& rect,
			wxPaneInfo& pane) = 0;
	};


	// this is the default art provider for wxFrameManager.  Dock art
	// can be customized by creating a class derived from this one,
	// or replacing this class entirely

	//##ModelId=44B8E82600FA
	class wxDefaultDockArt : public wxDockArt
	{
	public:

		//##ModelId=44B8E826010A
		wxDefaultDockArt();

		//##ModelId=44B8E8260290
		int GetMetric(int id);
		//##ModelId=44B8E827002E
		void SetMetric(int id, int new_val);
		//##ModelId=44B8E82701A5
		wxColour GetColour(int id);
		//##ModelId=44B8E827031C
		void SetColour(int id, const wxColor& colour);
		//##ModelId=44B8E82800AB
		void SetFont(int id, const wxFont& font);
		//##ModelId=44B8E8280232
		wxFont GetFont(int id);

		//##ModelId=44B8E8280399
		void DrawSash(wxDC& dc,
			int orientation,
			const wxRect& rect);

		//##ModelId=44B8E8290119
		void DrawBackground(wxDC& dc,
			int orientation,
			const wxRect& rect);

		//##ModelId=44B8E8290280
		void DrawCaption(wxDC& dc,
			const wxString& text,
			const wxRect& rect,
			wxPaneInfo& pane);

		//##ModelId=44B8E82903D8
		void DrawGripper(wxDC& dc,
			const wxRect& rect,
			wxPaneInfo& pane);

		//##ModelId=44B8E82A0148
		void DrawBorder(wxDC& dc,
			const wxRect& _rect,
			wxPaneInfo& pane);

		//##ModelId=44B8E82A02AF
		void DrawPaneButton(wxDC& dc,
			int button,
			int button_state,
			const wxRect& _rect,
			wxPaneInfo& pane);

	protected:

		//##ModelId=44B8E82B000F
		void DrawCaptionBackground(wxDC& dc, const wxRect& rect, bool active);

	protected:

		//##ModelId=44B8E82B0178
		wxPen m_border_pen;
		//##ModelId=44B8E82B0187
		wxBrush m_sash_brush;
		//##ModelId=44B8E82B018C
		wxBrush m_background_brush;
		//##ModelId=44B8E82B0197
		wxBrush m_gripper_brush;
		//##ModelId=44B8E82B019C
		wxFont m_caption_font;
		//##ModelId=44B8E82B01A6
		wxBitmap m_inactive_close_bitmap;
		//##ModelId=44B8E82B01AB
		wxBitmap m_inactive_pin_bitmap;
		//##ModelId=44B8E82B01B6
		wxBitmap m_active_close_bitmap;
		//##ModelId=44B8E82B01BB
		wxBitmap m_active_pin_bitmap;
		//##ModelId=44B8E82B01C6
		wxPen m_gripper_pen1;
		//##ModelId=44B8E82B01CB
		wxPen m_gripper_pen2;
		//##ModelId=44B8E82B01D5
		wxPen m_gripper_pen3;
		//##ModelId=44B8E82B01DA
		wxColour m_active_caption_colour;
		//##ModelId=44B8E82B01E5
		wxColour m_active_caption_gradient_colour;
		//##ModelId=44B8E82B01F5
		wxColour m_active_caption_text_colour;
		//##ModelId=44B8E82B01FA
		wxColour m_inactive_caption_colour;
		//##ModelId=44B8E82B0204
		wxColour m_inactive_caption_gradient_colour;
		//##ModelId=44B8E82B0209
		wxColour m_inactive_caption_text_colour;
		//##ModelId=44B8E82B0215
		int m_border_size;
		//##ModelId=44B8E82B0222
		int m_caption_size;
		//##ModelId=44B8E82B0232
		int m_sash_size;
		//##ModelId=44B8E82B0233
		int m_button_size;
		//##ModelId=44B8E82B0242
		int m_gripper_size;
		//##ModelId=44B8E82B0251
		int m_gradient_type;
	};


	//##ModelId=44B8E82B029F
	class wxDockInfo
	{
	public:
		//##ModelId=44B8E82B02A0
		wxDockInfo()
		{
			dock_direction = 0;
			dock_layer = 0;
			dock_row = 0;
			size = 0;
			min_size = 0;
			resizable = true;
			fixed = false;
			toolbar = false;
		}

		//##ModelId=44B8E82B02A1
		wxDockInfo(const wxDockInfo& c)
		{
			dock_direction = c.dock_direction;
			dock_layer = c.dock_layer;
			dock_row = c.dock_row;
			size = c.size;
			min_size = c.min_size;
			resizable = c.resizable;
			fixed = c.fixed;
			toolbar = c.toolbar;
			panes = c.panes;
			rect = c.rect;
		}

		//##ModelId=44B8E82B02B0
		wxDockInfo& operator=(const wxDockInfo& c)
		{
			dock_direction = c.dock_direction;
			dock_layer = c.dock_layer;
			dock_row = c.dock_row;
			size = c.size;
			min_size = c.min_size;
			resizable = c.resizable;
			fixed = c.fixed;
			toolbar = c.toolbar;
			panes = c.panes;
			rect = c.rect;
			return *this;
		}

		//##ModelId=44B8E82B02B2
		bool IsOk() const { return (dock_direction != 0) ? true : false; }
		//##ModelId=44B8E82B02B4
		bool IsHorizontal() const {
			return (dock_direction == wxAUI_DOCK_TOP ||
				dock_direction == wxAUI_DOCK_BOTTOM) ? true : false;
		}
		//##ModelId=44B8E82B02B6
		bool IsVertical() const {
			return (dock_direction == wxAUI_DOCK_LEFT ||
				dock_direction == wxAUI_DOCK_RIGHT ||
				dock_direction == wxAUI_DOCK_CENTER) ? true : false;
		}
	public:
		//##ModelId=44B8E82B02C0
		wxPaneInfoPtrArray panes; // array of panes
		//##ModelId=44B8E82B02C5
		wxRect rect;              // current rectangle
		//##ModelId=44B8E82B02C9
		int dock_direction;       // dock direction (top, bottom, left, right, center)
		//##ModelId=44B8E82B02CE
		int dock_layer;           // layer number (0 = innermost layer)
		//##ModelId=44B8E82B02CF
		int dock_row;             // row number on the docking bar (0 = first row)
		//##ModelId=44B8E82B02D0
		int size;                 // size of the dock
		//##ModelId=44B8E82B02D1
		int min_size;             // minimum size of a dock (0 if there is no min)
		//##ModelId=44B8E82B02DE
		bool resizable;           // flag indicating whether the dock is resizable
		//##ModelId=44B8E82B02DF
		bool toolbar;             // flag indicating dock contains only toolbars
		//##ModelId=44B8E82B02E0
		bool fixed;               // flag indicating that the dock operates on
		// absolute coordinates as opposed to proportional
	};


	//##ModelId=44B8E82B030D
	class wxDockUIPart
	{
	public:
		enum
		{
			typeCaption,
			typeGripper,
			typeDock,
			typeDockSizer,
			typePane,
			typePaneSizer,
			typeBackground,
			typePaneBorder,
			typePaneButton,
		};

		//##ModelId=44B8E82B030E
		int type;                // ui part type (see enum above)
		//##ModelId=44B8E82B030F
		int orientation;         // orientation (either wxHORIZONTAL or wxVERTICAL)
		//##ModelId=44B8E82B031D
		wxDockInfo* dock;        // which dock the item is associated with
		//##ModelId=44B8E82B0322
		wxPaneInfo* pane;        // which pane the item is associated with
		//##ModelId=44B8E82B032D
		wxPaneButton* button;    // which pane button the item is associated with
		//##ModelId=44B8E82B0332
		wxSizer* cont_sizer;     // the part's containing sizer
		//##ModelId=44B8E82B033D
		wxSizerItem* sizer_item; // the sizer item of the part
		//##ModelId=44B8E82B0342
		wxRect rect;             // client coord rectangle of the part itself
	};


	//##ModelId=44B8E82B034B
	class wxPaneButton
	{
	public:
		//##ModelId=44B8E82B034C
		int button_id;        // id of the button (e.g. buttonClose)
	};




	// wx event machinery


	// right now the only event that works is wxEVT_AUI_PANEBUTTON. A full
	// spectrum of events will be implemented in the next incremental version

	BEGIN_DECLARE_EVENT_TYPES()
		DECLARE_EVENT_TYPE(wxEVT_AUI_PANEBUTTON, 0)
	END_DECLARE_EVENT_TYPES()

		//##ModelId=44B8E82B035B
		typedef void (wxEvtHandler::*wxFrameManagerEventFunction)(wxFrameManagerEvent&);

#define wxFrameManagerEventHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxFrameManagerEventFunction, &func)

#define EVT_AUI_PANEBUTTON(func) \
   wx__DECLARE_EVT0(wxEVT_AUI_PANEBUTTON, wxFrameManagerEventHandler(func))

}