#include "stdafx.h"
#include "mathUtil.h"

namespace vega
{
	bool mathUtil_t::getViewRay(long x, long y, Ogre::Ray &ray, Ogre::Viewport *vp){
		Ogre::Camera *c = vp->getCamera();
		Ogre::RenderTarget *rt = vp->getTarget();
		float tgrx = x / (float)rt->getWidth();
		float tgry = y / (float)rt->getHeight();
		if (tgrx<vp->getLeft() || tgrx>vp->getLeft() + vp->getWidth() ||
			tgry<vp->getTop() || tgry>vp->getTop() + vp->getHeight())
			return false;
		float tscreenx = (tgrx - vp->getLeft()) / vp->getWidth();
		float tscreeny = (tgry - vp->getTop()) / vp->getHeight();
		ray = c->getCameraToViewportRay(tscreenx, tscreeny);
		return true;
	}
}