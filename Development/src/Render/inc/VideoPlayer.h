/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

namespace vega
{
	class Render;
	class RENDER_API VideoPlayer
	{
	public:
		VideoPlayer(Render* _render);
		~VideoPlayer();
		bool playVideo (const std::string& resourceName, bool allowSkipping);
		void Update();
		void close();
		void stopVideo();
		bool isPlaying();
		void setResolution (int w, int h) { mWidth = w; mHeight = h; }
	private:
		Render* render;
		struct VideoState* mState;
		bool mAllowSkipping;
		Ogre::SceneManager* mSceneMgr;
		Ogre::Material* mVideoMaterial;
		Ogre::Rectangle2D* mRectangle;
		Ogre::Rectangle2D* mBackgroundRectangle;
		Ogre::SceneNode* mNode;
		Ogre::SceneNode* mBackgroundNode;
		int mWidth;
		int mHeight;
	};
}

#endif