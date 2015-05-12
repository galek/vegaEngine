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

		NewB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\New.bmp"), wxBITMAP_TYPE_BMP);
		OpenB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\Open.bmp"), wxBITMAP_TYPE_BMP);
		SaveB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\Save.bmp"), wxBITMAP_TYPE_BMP);
		SaveAllB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\SaveAll.bmp"), wxBITMAP_TYPE_BMP);
		UndoB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\Undo.bmp"), wxBITMAP_TYPE_BMP);
		RedoB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\Redo.bmp"), wxBITMAP_TYPE_BMP);
		CutB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\Cut.bmp"), wxBITMAP_TYPE_BMP);
		CopyB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\Copy.bmp"), wxBITMAP_TYPE_BMP);

		PasteB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\Paste.bmp"), wxBITMAP_TYPE_BMP);
		SearchB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\Search.bmp"), wxBITMAP_TYPE_BMP);
		FullScreenB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\FullScreen.bmp"), wxBITMAP_TYPE_BMP);
		ContentBrowserB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\MainToolBar_ContentBrowser.bmp"), wxBITMAP_TYPE_BMP);
		GenericB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\ThumbnailView.bmp"), wxBITMAP_TYPE_BMP);
		KismetB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\Kismet.bmp"), wxBITMAP_TYPE_BMP);
		MouseLockB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\MouseLock.bmp"), wxBITMAP_TYPE_BMP);
		ShowWidgetB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\ShowWidget.bmp"), wxBITMAP_TYPE_BMP);

		TranslateB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\Translate.bmp"), wxBITMAP_TYPE_BMP);
		RotateB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\Rotate.bmp"), wxBITMAP_TYPE_BMP);
		ScaleB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\Scale.bmp"), wxBITMAP_TYPE_BMP);
		ScaleNonUniformB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\ScaleNonUniform.bmp"), wxBITMAP_TYPE_BMP);
		BrushPolysB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\BrushPolys.bmp"), wxBITMAP_TYPE_BMP);
		PrefabLockB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\PrefabLock.bmp"), wxBITMAP_TYPE_BMP);
		DistributionToggleB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\DistributionToggle.bmp"), wxBITMAP_TYPE_BMP);
		PSysRealtimeLODToggleB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\PSysRealtimeLODToggle.bmp"), wxBITMAP_TYPE_BMP);
		CamSlowB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\CamSlow.bmp"), wxBITMAP_TYPE_BMP);

		CamNormalB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\CamNormal.bmp"), wxBITMAP_TYPE_BMP);
		CamFastB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\CamFast.bmp"), wxBITMAP_TYPE_BMP);
		ViewPushStartB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\ViewPushStart.bmp"), wxBITMAP_TYPE_BMP);
		ViewPushStopB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\ViewPushStop.bmp"), wxBITMAP_TYPE_BMP);
		ViewPushSyncB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\ViewPushSync.bmp"), wxBITMAP_TYPE_BMP);
		PublishCookB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\PublishCook.bmp"), wxBITMAP_TYPE_BMP);
		PublishCopyB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\PublishCopy.bmp"), wxBITMAP_TYPE_BMP);
		MatineeListB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\MatineeList.bmp"), wxBITMAP_TYPE_BMP);
		SentinelB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\Sentinel.bmp"), wxBITMAP_TYPE_BMP);
		GameStatsVisualizerB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\GameStatsVisualizer.bmp"), wxBITMAP_TYPE_BMP);

		BuildGeomB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\BuildGeom.bmp"), wxBITMAP_TYPE_BMP);
		BuildLightingB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\BuildLighting.bmp"), wxBITMAP_TYPE_BMP);
		BuildPathsB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\BuildPaths.bmp"), wxBITMAP_TYPE_BMP);
		BuildCoverNodesB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\BuildCoverNodes.bmp"), wxBITMAP_TYPE_BMP);
		BuildAllB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\BuildAll.bmp"), wxBITMAP_TYPE_BMP);

		SocketsB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\ASV_SocketMgr.bmp"), wxBITMAP_TYPE_BMP);
		// Play-In-Editor button
		PlayInEditorB.LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\Play.bmp"), wxBITMAP_TYPE_BMP);
		// default PC play icon
		PlayOnB[B_PC].LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\PlayPC.bmp"), wxBITMAP_TYPE_BMP);
		// Playstation play icon
		PlayOnB[B_PS3].LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\PlayPS.bmp"), wxBITMAP_TYPE_BMP);
		// Xbox play icon
		PlayOnB[B_XBox360].LoadFile(wxT("..\\Editor\\EditorResources\\wxRes\\PlayXB.bmp"), wxBITMAP_TYPE_BMP);

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
			frame->prs->onEvent(In);
			frame->processViewEvent(In);
			frame->prs->onActive();
		}
	}
}