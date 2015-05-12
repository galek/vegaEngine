#include "stdafx.h"
#include "tool_t.h"
#include "moveObjectProp_t.h"
#include "moveObjectPropBag_t.h"
#include "frame_t.h"
#include "wx/choicebk.h"
#include "type_t.h"


#include "EngineWrapper.h"

namespace vega
{
	moveObjectPropBag_t::~moveObjectPropBag_t(){
		for (iterator itr(begin()); itr != end(); ++itr){
			delete itr->second;
		}
		clear();
	}
	void moveObjectPropBag_t::addProp(moveObjectProp_t *prop){
		prop->init();
		this->operator[](prop->getTypeName()) = prop;
	}
	void moveObjectPropBag_t::onSelNode(Ogre::SceneNode *me){
		if (me){
			int numAttachedObjects = me->numAttachedObjects();
			for (int i(0); i < numAttachedObjects; ++i){
				MovableObject *m = me->getAttachedObject(i);
				type_t* tp = (type_t*)&(m->getUserAny());
				if (tp->editable && !tp->dummy){
					std::string type = m->getMovableType();
					iterator itr = find(type);
					if (itr != end()){
						frame->getPropBook()->SetSelection(itr->second->getIndex());
						itr->second->update(m, false);
						break;
					}
				}
			}
		}
		else{
			std::string type;
			iterator itr = find(type);
			if (itr != end()){
				frame->getPropBook()->SetSelection(itr->second->getIndex());
				itr->second->update(NULL, false);
			}
		}
	}
	void moveObjectPropBag_t::onSelMovable(Ogre::MovableObject *me){
	}
}