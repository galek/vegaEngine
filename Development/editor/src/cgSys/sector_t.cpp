/*
Copyright (c) 2005-2007, lidong wang

All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
* Neither the name of the lidongwang nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "stdafx.h"
#include "sector_t.h"


namespace vega
{
	void sectorRoot_t::clear(){
		for (iterator itr(begin()); itr != end(); ++itr)
			delete *itr;
		sector_t::clear();
		rootSector.clear();
		rootSector.setNull();
	}
	void sectorRoot_t::beginAddSector(){
		clear();
	}
	void sectorRoot_t::addSector(const AxisAlignedBox &aab){
		sector_t *t = new sector_t;
		t->setExtents(aab.getMinimum(), aab.getMaximum());
		push_back(t);
		rootSector.merge(aab);
	}
	inline int aabCompare(const AxisAlignedBox *a, const AxisAlignedBox *b){
		float va = a->volume();
		float vb = b->volume();
		float iv = a->intersection(*b).volume();
		if (va >= vb){
			if (iv > vb*0.9)
				return 1;
			else
				return 0;
		}
		else{
			if (iv > va*0.9)
				return -1;
			else
				return 0;
		}
	}
	inline int sectorFuncCompare(sector_t * a, sector_t * b){
		return aabCompare(a, b);
	}

	/*
		sector_t *a=(sector_t*)elem1;
		sector_t *b=(sector_t*)elme2;*/

	/*
	if( a->getMinimum()-0.1f<b->getMinimum() && a->getMaximum()+0.1f>b->getMaximum())
	return 1;
	else if(b->getMinimum()-0.1f<a->getMinimum() && b->getMaximum()+0.1f>a->getMaximum())
	return -1;
	else return 0;*/
	static int qSortSectorFuncCompare(const void *a, const void *b){
		return -sectorFuncCompare((sector_t*)a, (sector_t*)b);
	}
	void sectorRoot_t::endAddSector(){
		if (size()){
			if (size() > 1)
				qsort((void*)&(*begin()), size(), (sizeof(sector_t*)), &qSortSectorFuncCompare);
			for (iterator itr(begin()); itr != end(); ++itr){
				rootSector.pushSector(*itr);
			}
		}
	}
	bool sector_t::pushSector(sector_t *me){
		if (sectorFuncCompare(this, me) > 0){
			iterator itr(begin());
			for (; itr != end(); ++itr){
				if ((*itr)->pushSector(me)){
					break;
				}
			}
			if (itr == end()){
				push_back(me);
			}
			return true;
		}
		else{
			if (intersects(*me)){
				neighborSectorBag.push_back(me);
				me->neighborSectorBag.push_back(this);
			}
			return false;
		}
	}
	void sectorRoot_t::findVisibleObjects(Camera *cam, RenderQueue * queue, VisibleObjectsBoundsInfo* visibleBounds, bool includeChildren, bool displayNodes, bool onlyShadowCasters){
		rootSector.findVisibleObjects(cam, queue, visibleBounds, includeChildren, displayNodes, onlyShadowCasters, false);
	}
	bool sectorRoot_t::pushSceneNode(SceneNode *o, int typeId){
		rootSector.merge(o->_getWorldAABB());;
		return rootSector.pushSceneNode(o, typeId);
	}
	void sector_t::findVisibleObjects(Camera *cam, RenderQueue * queue, VisibleObjectsBoundsInfo* visibleBounds, bool includeChildren, bool displayNodes, bool onlyShadowCasters, bool neighbor){
		if (neighbor){
			if (!cam->isVisible(*this))
				return;
		}
		else{
			if (!intersects(cam->getDerivedPosition()))
				return;
		}
		for (SceneNodeBagMap_t::iterator itr(sceneNodeBagMap.begin()); itr != sceneNodeBagMap.end(); ++itr){
			for (sceneNodeBag_t::iterator it(itr->second->begin()); it != itr->second->end(); ++it){
				(*it)->_findVisibleObjects(cam, queue, visibleBounds, includeChildren, displayNodes, onlyShadowCasters);
			}
		}
		if (!neighbor){
			for (iterator itr(begin()); itr != end(); ++itr)
				(*itr)->findVisibleObjects(cam, queue, visibleBounds, includeChildren, displayNodes, onlyShadowCasters, false);
			for (iterator itr(neighborSectorBag.begin()); itr != neighborSectorBag.end(); ++itr)
				(*itr)->findVisibleObjects(cam, queue, visibleBounds, includeChildren, displayNodes, onlyShadowCasters, true);
		}
	}
	bool sector_t::pushSceneNode(SceneNode *o, int typeId){
		if (aabCompare(this, &o->_getWorldAABB()) > 0){
			iterator itr(begin());
			for (; itr != end(); ++itr)
				if ((*itr)->pushSceneNode(o, typeId)){
				break;
				}
			if (itr == end()){
				SceneNodeBagMap_t::iterator itr = sceneNodeBagMap.find(typeId);
				if (itr == sceneNodeBagMap.end()){
					sceneNodeBagMap[typeId] = new sceneNodeBag_t;
					itr = sceneNodeBagMap.find(typeId);
				}
				itr->second->push_back(o);
			}
			return true;
		}
		return false;
	}
	sectorRoot_t::~sectorRoot_t(){
		clear();
	}
	void sectorRoot_t::populateLightList(const Vector3 &position, Real radius, LightList &destList, int lightId){

	}
	void sector_t::populateLightList(const Vector3 &position, Real radius, LightList &destList, int lightId){

	}
	sector_t::~sector_t(){
		clear();
	}
	void sector_t::clear(){
		vector<sector_t*>::clear();
		neighborSectorBag.clear();
		for (SceneNodeBagMap_t::iterator itr(sceneNodeBagMap.begin()); itr != sceneNodeBagMap.end(); ++itr){
			delete itr->second;
		}
		sceneNodeBagMap.clear();
	}
}