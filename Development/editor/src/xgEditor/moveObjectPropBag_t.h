#pragma once
#include "selectListener_t.h"

namespace vega
{
	class moveObjectProp_t;
	class moveObjectPropBag_t :public std::map<std::string, moveObjectProp_t*>, public selectListener_t{
	public:
		void addProp(moveObjectProp_t *prop);
		virtual void onSelNode(Ogre::SceneNode *me);
		virtual void onSelMovable(Ogre::MovableObject *me);
		~moveObjectPropBag_t();
	};
}