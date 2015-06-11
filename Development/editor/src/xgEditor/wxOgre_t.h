#pragma once

namespace vega
{
	class wxOgre_t :
		public wxControl
	{
		int width;
		int height;
		DECLARE_CLASS(wxOgre_t)
	public:
		void renderStaticThing();
		/** A new wxOgre_t must receive a parent frame to which to attach
		itself to */
		wxOgre_t(wxFrame* parent);
		~wxOgre_t();

		/** Renders a single Ogre frame */
		void update();

		/** Returns the currently used camera */
		Ogre::Camera* getCamera();
		/** Sets a new camera for rendering */
		void setCamera(Ogre::Camera* camera);
		void addRenderQueueListener(void);
	protected:
		DECLARE_EVENT_TABLE()
	private:

		/** Creates an ogre rendering window and all other default objects
		such as the Ogre Root, default camera, default scene manager etc */
		void createOgreRenderWindow();
		/** Toggles the rendering timer on/off */
		void toggleTimerRendering();

		/** Callback function to a window resize event */
		void OnSize(wxSizeEvent& event);
		/** Callback function to an EraseBackground event */
		void OnEraseBackground(wxEraseEvent&);
		/** Callback function to a timer "tick" event */
		void OnRenderTimer(wxTimerEvent& event);

		/* WX members */
		/** Rendering timer */
		wxTimer	mTimer;
	public:
		Ogre::Root *  getRoot();
		Ogre::Viewport* getViewPort();
		Ogre::SceneManager* getSceneManager();
		Ogre::RenderWindow* getRenderWindow();
		static wxOgre_t& getSingleton();
		static wxOgre_t* getSingletonPtr();
	};
}