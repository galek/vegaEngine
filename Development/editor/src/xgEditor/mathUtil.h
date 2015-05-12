#pragma once

namespace vega
{
	class mathUtil_t{
	public:
		static bool getViewRay(long x, long y, Ogre::Ray &ray, Ogre::Viewport *vp);
	};
}