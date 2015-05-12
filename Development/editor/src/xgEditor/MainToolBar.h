#pragma once

class wxComboBox;
class wxWindow;
//#include <wx/treectrl.h>
//#include <wx/noteBook.h>
//#include <wx/filename.h> 
//#include <wx/msw/registry.h>
//#include <wx/generic/choicdgg.h>
//#include <wx/aui/aui.h>
#include <wx/wx.h>
//#include "wx/file.h"
//#include <wx/dynlib.h> 



namespace vega
{
	/**
 * WxMatineeMenuListToolBarButton
 */
	//class WxMatineeMenuListToolBarButton
	////Nick	: public WxBitmapButton
	//{
	//
	//public:
	//
	//private:
	//
	//	/** Called when the tool bar button is clicked */
	//	void OnClick( wxCommandEvent &In );
	//
	//	DECLARE_EVENT_TABLE()
	//};



	/**
	 * The toolbar that sits at the top of the main editor frame.
	 */
	class WxMainToolBar : public wxToolBar
	{
	public:
		WxMainToolBar(wxWindow* InParent, wxWindowID InID);

		enum EPlatformButtons { B_PC = 0, B_PS3, B_XBox360, B_MAX };

		/** Updates the 'Push View' toggle's bitmap and hint text based on the input state. */
		void SetPushViewState(bool bIsPushingView);

		/** Enables/disables the 'Push View' button. */
		void EnablePushView(bool bEnabled);

	private:
		wxBitmap NewB, OpenB, SaveB, SaveAllB, UndoB, RedoB, CutB, CopyB, PasteB, SearchB, FullScreenB, ContentBrowserB, GenericB, KismetB, TranslateB,
			ShowWidgetB, RotateB, ScaleB, ScaleNonUniformB, MouseLockB, BrushPolysB, PrefabLockB, CamSlowB, CamNormalB, CamFastB, ViewPushStartB, ViewPushStopB, ViewPushSyncB,
			DistributionToggleB, SocketsB, PSysRealtimeLODToggleB, PublishCookB, PublishCopyB, MatineeListB, SentinelB, GameStatsVisualizerB;
		wxToolBarToolBase* ViewPushStartStopButton;
		wxBitmap BuildGeomB, BuildLightingB, BuildPathsB, BuildCoverNodesB, BuildAllB, PlayOnB[B_MAX], PlayInEditorB;
		//Nick	WxMenuButton MRUButton, PasteSpecialButton;
		wxMenu PasteSpecialMenu;

		/** Drop down of all available Matinee sequences in the level */
		//	WxMatineeMenuListToolBarButton MatineeListButton;

		DECLARE_EVENT_TABLE();

		/** Called when the trans/rot/scale widget toolbar buttons are right-clicked. */
		void OnTransformButtonRightClick(wxCommandEvent& In);

		/** Called when the Matinee list tool bar button is clicked */
		void OnMatineeListMenu(wxCommandEvent& In);

	private:
		/** Called when the trans/rot/scale widget toolbar buttons are right-clicked. */
		void GizmoMovePressed(wxCommandEvent& In);

	public:
		wxComboBox* CoordSystemCombo;
	};
}