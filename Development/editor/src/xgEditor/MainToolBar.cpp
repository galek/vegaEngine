#include "stdafx.h"
#include "MainToolBar.h"
#include "ToolsIds.h"

#include "prsTool_t.h"

#include "EngineWrapper.h"
#include "frame_t.h"

namespace vega
{
	BEGIN_EVENT_TABLE(WxMainToolBar, wxToolBar)
		EVT_TOOL(IDM_TOOLBAR_TOOL_MOVE, WxMainToolBar::GizmoMovePressed)
		//EVT_TOOL_RCLICKED( ID_EDIT_TRANSLATE, WxMainToolBar::OnTransformButtonRightClick )
		//EVT_TOOL_RCLICKED( ID_EDIT_ROTATE, WxMainToolBar::OnTransformButtonRightClick )
		//EVT_TOOL_RCLICKED( ID_EDIT_SCALE, WxMainToolBar::OnTransformButtonRightClick )
		//EVT_TOOL_RCLICKED( ID_EDIT_SCALE_NONUNIFORM, WxMainToolBar::OnTransformButtonRightClick )
		END_EVENT_TABLE();

	WxMainToolBar::WxMainToolBar(wxWindow* InParent, wxWindowID InID)
		: wxToolBar(InParent, InID, wxDefaultPosition, wxDefaultSize, wxTB_HORIZONTAL | wxNO_BORDER | wxTB_3DBUTTONS)
	{
		// Menus

		//	PasteSpecialMenu.Append( IDM_PASTE_ORIGINAL_LOCATION, *LocalizeUnrealEd("OriginalLocation") );
		//	PasteSpecialMenu.Append( IDM_PASTE_WORLD_ORIGIN, *LocalizeUnrealEd("WorldOrigin") );

		// Bitmaps
		wxImage::AddHandler(new wxPNGHandler);

		NewB.Load(wxT("New"));
		OpenB.Load(wxT("Open"));
		SaveB.Load(wxT("Save"));
		SaveAllB.Load(wxT("SaveAll"));
		UndoB.Load(wxT("Undo"));
		RedoB.Load(wxT("Redo"));
		CutB.Load(wxT("Cut"));
		CopyB.Load(wxT("Copy"));

		PasteB.Load(wxT("Paste"));
		SearchB.Load(wxT("Search"));
		FullScreenB.Load(wxT("FullScreen"));
		ContentBrowserB.Load(wxT("MainToolBar_ContentBrowser"));
		GenericB.Load(wxT("ThumbnailView"));
		KismetB.Load(wxT("Kismet"));
		MouseLockB.Load(wxT("MouseLock"));
		ShowWidgetB.Load(wxT("ShowWidget"));

		TranslateB.Load(wxT("Translate"));
		RotateB.Load(wxT("Rotate"));
		ScaleB.Load(wxT("Scale"));
		ScaleNonUniformB.Load(wxT("ScaleNonUniform"));
		BrushPolysB.Load(wxT("BrushPolys"));
		PrefabLockB.Load(wxT("PrefabLock"));
		DistributionToggleB.Load(wxT("DistributionToggle"));
		PSysRealtimeLODToggleB.Load(wxT("PSysRealtimeLODToggle"));
		CamSlowB.Load(wxT("CamSlow"));

		CamNormalB.Load(wxT("CamNormal"));
		CamFastB.Load(wxT("CamFast"));
		ViewPushStartB.Load(wxT("ViewPushStart"));
		ViewPushStopB.Load(wxT("ViewPushStop"));
		ViewPushSyncB.Load(wxT("ViewPushSync"));
		PublishCookB.Load(wxT("PublishCook"));
		PublishCopyB.Load(wxT("PublishCopy"));
		MatineeListB.Load(wxT("MatineeList"));
		SentinelB.Load(wxT("Sentinel"));
		GameStatsVisualizerB.Load(wxT("GameStatsVisualizer"));

		BuildGeomB.Load(wxT("BuildGeom"));
		BuildLightingB.Load(wxT("BuildLighting"));
		BuildPathsB.Load(wxT("BuildPaths"));
		BuildCoverNodesB.Load(wxT("BuildCoverNodes"));
		BuildAllB.Load(wxT("BuildAll"));

		SocketsB.Load(wxT("ASV_SocketMgr"));
		// Play-In-Editor button
		PlayInEditorB.Load(wxT("Play"));
		// default PC play icon
		PlayOnB[B_PC].Load(wxT("PlayPC"));
		// Playstation play icon
		PlayOnB[B_PS3].Load(wxT("PlayPS"));
		// Xbox play icon
		PlayOnB[B_XBox360].Load(wxT("PlayXB"));

		AddTool(wxID_NEW, NewB, wxT("New Level"));
		AddTool(wxID_OPEN, OpenB, wxT("Open Level"));
		AddTool(wxID_SAVE, SaveB, wxT("Save Level"));
		AddTool(wxID_SAVE, SaveAllB, wxT("Save All"));
		AddTool(wxID_UNDO, UndoB, wxT("Undo"));
		AddTool(wxID_REDO, RedoB, wxT("Redo"));
		AddTool(wxID_CUT, CutB, wxT("Cut"));
		AddTool(wxID_COPY, CopyB, wxT("Copy"));

		AddTool(wxID_PASTE, PasteB, wxT("Paste"));
		AddTool(wxID_EXIT, SearchB, wxT("Search on scene"));
		AddTool(wxID_EXIT, FullScreenB, wxT("Fullscreen"));//TODO
		AddTool(wxID_EXIT, ContentBrowserB, wxT("Content Browser"));
		AddTool(wxID_EXIT, GenericB, wxT("Generic Browser"));
		AddTool(wxID_EXIT, KismetB, wxT("Kismet"));
		AddTool(wxID_EXIT, MouseLockB, wxT("Mouse lock"));//TODO
		AddTool(wxID_EXIT, ShowWidgetB, wxT("Show Widget"));//TODO

		AddCheckTool(IDM_TOOLBAR_TOOL_MOVE, wxT("Translate"), TranslateB);
		AddCheckTool(IDM_TOOLBAR_TOOL_ROTATE, wxT("Rotate"), RotateB);
		AddTool(IDM_TOOLBAR_TOOL_SCALE, ScaleB, wxT("Scale"));//TODO
		AddTool(wxID_EXIT, ScaleNonUniformB, wxT("Scale non uniform"));//TODO
		AddTool(wxID_EXIT, BrushPolysB, wxT("Brush Polys"));//TODO
		AddTool(wxID_EXIT, PrefabLockB, wxT("Prefab Lock"));//TODO
		AddTool(wxID_EXIT, DistributionToggleB, wxT("Distribution Toggle"));//TODO
		AddTool(wxID_EXIT, PSysRealtimeLODToggleB, wxT("PSys Realtime LOD Toggle"));//TODO
		AddTool(wxID_EXIT, CamSlowB, wxT("Cam Slow"));//TODO

		AddTool(wxID_EXIT, CamNormalB, wxT("Cam Normal"));//TODO
		AddTool(wxID_EXIT, CamFastB, wxT("Cam Fast"));//TODO
		AddTool(wxID_EXIT, ViewPushStartB, wxT("View Push Start"));//TODO
		AddTool(wxID_EXIT, ViewPushStopB, wxT("View Push Stop"));
		AddTool(wxID_EXIT, ViewPushSyncB, wxT("View Push Sync"));
		AddTool(wxID_EXIT, PublishCookB, wxT("Publish Cook"));
		AddTool(wxID_EXIT, PublishCopyB, wxT("Publish Copy"));
		AddTool(wxID_EXIT, MatineeListB, wxT("Matinee List"));
		AddTool(wxID_EXIT, SentinelB, wxT("Sentinel"));
		AddTool(wxID_EXIT, GameStatsVisualizerB, wxT("GameStats Visualizer"));


		AddTool(wxID_EXIT, BuildGeomB, wxT("Build Geom"));
		AddTool(wxID_EXIT, BuildLightingB, wxT("Build Lighting"));
		AddTool(wxID_EXIT, BuildPathsB, wxT("Build Paths"));
		AddTool(wxID_EXIT, BuildCoverNodesB, wxT("Build CoverNodes"));
		AddTool(wxID_EXIT, BuildAllB, wxT("Build All"));
		Realize();

		Realize();

		// The ViewPush button defulats to disabled because the object propagation destination is 'none'.
		// This must be called AFTER Realize()!
		EnablePushView(FALSE);
	}

	/** Updates the 'Push View' toggle's bitmap and hint text based on the input state. */
	void WxMainToolBar::SetPushViewState(bool bIsPushingView)
	{

	}

	/** Enables/disables the 'Push View' button. */
	void WxMainToolBar::EnablePushView(bool bEnabled)
	{
		if (ViewPushStartStopButton)
		{
			//EnableTool( IDM_PUSHVIEW_StartStop, bEnabled ? true : false );
		}
	}

	/** Called when the trans/rot/scale widget toolbar buttons are right-clicked. */
	void WxMainToolBar::OnTransformButtonRightClick(wxCommandEvent& In)
	{
		//WxDlgTransform::ETransformMode NewMode = WxDlgTransform::TM_Translate;
		//switch( In.GetId() )
		//{
		//	case ID_EDIT_ROTATE:
		//		NewMode = WxDlgTransform::TM_Rotate;
		//		break;
		//	case ID_EDIT_SCALE:
		//	case ID_EDIT_SCALE_NONUNIFORM:
		//		NewMode = WxDlgTransform::TM_Scale;
		//		break;
		//}

		//const bool bIsShown = GApp->DlgTransform->IsShown();
		//const UBOOL bSameTransformMode = GApp->DlgTransform->GetTransformMode() == NewMode;
		//if ( bIsShown && bSameTransformMode )
		//{
		//	GApp->DlgTransform->Show( false );
		//}
		//else
		//{
		//	GApp->DlgTransform->SetTransformMode( NewMode );
		//	GApp->DlgTransform->Show( true );
		//}
	}



	/**
	 * WxMatineeMenuListToolBarButton
	 */

	//BEGIN_EVENT_TABLE( WxMatineeMenuListToolBarButton, WxBitmapButton )
	//	EVT_COMMAND( IDM_MainToolBar_MatineeList, wxEVT_COMMAND_BUTTON_CLICKED, WxMatineeMenuListToolBarButton::OnClick )
	//END_EVENT_TABLE()


	/** Called when the tool bar button is clicked */
	//void WxMatineeMenuListToolBarButton::OnClick( wxCommandEvent &In )
	//{
	//// Open Matinee!  If there's only one Matinee available to open this will just go ahead and do it, otherwise
	//// a menu object will be returned that we'll have to display
	//wxMenu* MenuToDisplay = GApp->Editorframe->OpenMatineeOrBuildMenu();
	//if( MenuToDisplay != NULL )
	//{
	//	// Display the menu directly below the button
	//	wxRect rc = GetRect();
	//	PopupMenu( MenuToDisplay, 0, rc.GetHeight() );
	//}
	//}

	void WxMainToolBar::GizmoMovePressed(wxCommandEvent& In)
	{
		//if (In.GetId == IDM_TOOLBAR_TOOL_MOVE)
		//if (In.IsChecked())
		{
			frame->GetPrsTool()->onEvent(In);
			frame->processViewEvent(In);
			frame->GetPrsTool()->onActive();
		}
	}
}