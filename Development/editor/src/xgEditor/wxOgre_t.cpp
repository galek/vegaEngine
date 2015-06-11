#include"stdafx.h"
#include "wxOgre_t.h"
#include "xml.h"
#ifdef __WXGTK__
#include <gdk/gdk.h>
#include <gtk/gtk.h> // just this should suffice as it should include gdk.h itself
#endif

#include "OgreLogManager.h"
using namespace Ogre;
//Engine.dll
#include "EngineWrapper.h"
#include "ActorDynamicSky.h"

#include "cgSys_t.h"

namespace vega
{
	/**
	*/
	// Required for the timer
	static const long ID_RENDERTIMER = wxNewId();
	/**
	*/
	// Required for WX
	IMPLEMENT_CLASS(wxOgre_t, wxControl)
		// Required for WX
		BEGIN_EVENT_TABLE(wxOgre_t, wxControl)
		EVT_SIZE(wxOgre_t::OnSize)
		EVT_ERASE_BACKGROUND(wxOgre_t::OnEraseBackground)
		EVT_TIMER(ID_RENDERTIMER, wxOgre_t::OnRenderTimer)
		END_EVENT_TABLE();
	/**
	*/
	void wxOgre_t::renderStaticThing(){
		TODO("Delme")
	}

	/**
	*/
	wxOgre_t::wxOgre_t(wxFrame* parent) :
		wxControl(parent, -1),
		mTimer(this, ID_RENDERTIMER)
	{
		// Create all Ogre objects
		createOgreRenderWindow();
		// Start the rendering timer
		toggleTimerRendering();
		//add this to Render Queue Listener
		addRenderQueueListener();
	}

	/**
	*/
	void wxOgre_t::createOgreRenderWindow()
	{
		// --------------------
		// Create a new parameters list according to compiled OS
		Ogre::NameValuePairList params;
		Ogre::String handle;
#ifdef __WXMSW__
		handle = Ogre::StringConverter::toString((size_t)((HWND)GetHandle()));
#elif defined(__WXGTK__)
		// TODO: Someone test this. you might to use "parentWindowHandle" if this
		// does not work.  Ogre 1.2 + Linux + GLX platform wants a string of the
		// format display:screen:window, which has variable types ulong:uint:ulong.
		GdkWindow * window = GetHandle()->window
			handle = Ogre::StringConverter::toString((ulong)GDK_WINDOW_XDISPLAY(window));
		handle += ":0:";
		handle += Ogre::StringConverter::toString((uint)GDK_WINDOW_XID(window));
#else
#error Not supported on this platform.
#endif
		params["externalWindowHandle"] = handle;

		// Get wx control window size
		int width;
		int height;
		GetSize(&width, &height);
		GetEditor()->Go(width, height, &params);
	}

	/**
	*/
	void wxOgre_t::toggleTimerRendering()
	{
		// Toggle Start/Stop
		if (mTimer.IsRunning())
			mTimer.Stop();
		mTimer.Start(10);
	}

	/**
	*/
	wxOgre_t::~wxOgre_t()
	{
		//// destroy Viewport and RenderWindow
		//if (mViewPort)
		//{
		//	mRenderWindow->removeViewport(mViewPort->getZOrder());
		//	mViewPort = 0;
		//}

		//Ogre::Root::getSingleton().detachRenderTarget(mRenderWindow);
		//delete mRenderWindow;
		//mRenderWindow = 0;

	}

	/**
	*/
	void wxOgre_t::OnSize(wxSizeEvent& event)
	{
		//Nick
		// Setting new size;
		int width;
		int height;
		GetSize(&width, &height);
		GetEditor()->mGWindow->resize(width, height);
		// Letting Ogre know the window has been resized;
		GetEditor()->mGWindow->windowMovedOrResized();
		// Set the aspect ratio for the new size;
		if (GetEditor()->mGCamera)
			GetEditor()->mGCamera->setAspectRatio(Ogre::Real(width) / Ogre::Real(height));

		update();
	}

	/**
	*/
	void wxOgre_t::OnEraseBackground(wxEraseEvent&)
	{
		update();
	}

	/**
	*/
	void wxOgre_t::OnRenderTimer(wxTimerEvent& event)
	{
		update();
	}

	/**
	*/
	void wxOgre_t::update()
	{
		auto root = GetEditor()->mGRoot;
		if (!root)
		{
			printf("Not exist mGRoot");
			return;
		}

		int width;
		int height;
		GetSize(&width, &height);
		if (this->width != width || this->height != height){
			this->width = width;
			this->height = height;
			wxSizeEvent wse;
			OnSize(wse);
		}
		root->renderOneFrame();
	}

	/**
	*/
	Ogre::Root *  wxOgre_t::getRoot()
	{
		return GetEditor()->mGRoot;
	}

	/**
	*/
	Ogre::Viewport* wxOgre_t::getViewPort()
	{
		return GetEditor()->mGViewport;
	}

	/**
	*/
	Ogre::SceneManager* wxOgre_t::getSceneManager()
	{
		return GetEditor()->mGSceneMgr;
	}

	/**
	*/
	Ogre::RenderWindow* wxOgre_t::getRenderWindow()
	{
		return GetEditor()->mGWindow;
	}

	/**
	*/
	void wxOgre_t::addRenderQueueListener(void){
		TODO("Delme");
	}

	/**
	*/
	Ogre::Camera* wxOgre_t::getCamera(){ return GetEditor()->mGCamera; }

	/**
	*/
	void wxOgre_t::setCamera(Ogre::Camera* camera){ GetEditor()->mGCamera = camera; }
}