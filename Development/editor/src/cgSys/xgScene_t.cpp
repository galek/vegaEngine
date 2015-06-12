#include "stdafx.h"
#include "xgScene_t.h"
#include "xgSceneFactory_t.h"
#include "selectListener_t.h"
#include "type_t.h"
#include "sector_t.h"
#include "OgreMovableObject.h"
#include "xml.h"
#include <fstream>
#include "pfxType_t.h"
#include "dummyColors.h"
#include "OgreLight.h"
using namespace std;

namespace vega
{
	void xgScene_t::pushSelectListener(selectListener_t *me){
		selectListenerBag.push_back(me);
	}
	void xgScene_t::setSelSceneNode(SceneNode *me){
		for (selectListenerBag_t::const_iterator itr(selectListenerBag.begin()); itr != selectListenerBag.end(); ++itr){
			(*itr)->onSelNode(me);
		}
		if (selectSceneNode != me){
			if (selectSceneNode){
				selectSceneNode->showBoundingBox(false);
			}
			selectSceneNode = me;
			if (selectSceneNode){
				selectSceneNode->showBoundingBox(true);
			}
		}
	}
	void xgScene_t::setSelMovable(MovableObject *me){
		for (selectListenerBag_t::const_iterator itr(selectListenerBag.begin()); itr != selectListenerBag.end(); ++itr){
			(*itr)->onSelMovable(me);
		}
	}
	bool xgScene_t::getViewRay(long x, long y, Ray &ray){
		Viewport *vp = getCurrentViewport();
		Camera *c = vp->getCamera();
		RenderTarget *rt = vp->getTarget();
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
	MovableObject *xgScene_t::pickMoveable(long x, long y){
		Ray ray;
		if (!getViewRay(x, y, ray)){
			return NULL;
		}
		RaySceneQuery *rsq = createRayQuery(ray);
		rsq->setSortByDistance(true, 1);
		RaySceneQueryResult &rsqr = rsq->execute();
		if (rsqr.size() && rsqr.front().movable){
			return rsqr.front().movable;
		}
		return NULL;
	}
	SceneNode *xgScene_t::pickSceneNode(long x, long y){
		MovableObject *o = pickMoveable(x, y);
		if (o){
			return o->getParentSceneNode();
		}
		return NULL;
	}
	xgScene_t::xgScene_t(const String& name) :SceneManager(name), selectSceneNode(NULL){
		sectorRoot = new sectorRoot_t;
		renderOptimize = false;
		loadEntityCallback = NULL;
		loadEntityCallbackUserData = NULL;
	}
	void  xgScene_t::destroyAllMovableObjects(void)
	{
		GetEditor()->sceneManager->CleanScene();
		/*for (MovableObjectCollectionMap::iterator i = mMovableObjectCollectionMap.begin();
			i != mMovableObjectCollectionMap.end(); ++i){
			for (MovableObjectMap::iterator it(i->second->map.begin()); it != i->second->map.end(); ++it){
				const Ogre::Any *t = &(it->second->getUserAny());
				if (t){
					delete t;
					it->second->setUserAny(Ogre::Any(NULL));
				}
			}
		}
		SceneManager::destroyAllMovableObjects();*/
	}
	void  xgScene_t::destroyMovableObject(const String &name, const String &typeName){
		//MovableObjectCollection* objectMap = getMovableObjectCollection(typeName);
		//MovableObjectFactory* factory = Root::getSingleton().getMovableObjectFactory(typeName);
		//MovableObjectMap::iterator mi = objectMap->map.find(name);
		//if (mi != objectMap->map.end()){
		//	const Ogre::Any *t = &(mi->second->getUserAny());
		//	if (t){
		//		delete t;
		//		mi->second->setUserAny(Ogre::Any(NULL));
		//	}
		//	factory->destroyInstance(mi->second);
		//	objectMap->map.erase(mi);
		//}
	}
	xgScene_t::~xgScene_t(){
		delete sectorRoot;
	}
	const String& xgScene_t::getTypeName(void) const{
		return xgSceneFactory_t::FACTORY_TYPE_NAME;
	}
	void xgScene_t::delSelectListener(selectListener_t *me){
		remove(selectListenerBag.begin(), selectListenerBag.end(), me);
	}
	void xgScene_t::updateSectorSceneNode(){
		//for (SceneNodeList::iterator itr(mSceneNodes.begin()); itr != mSceneNodes.end(); ++itr){
		//	int numAttachedObjects = itr->second->numAttachedObjects();
		//	for (int i(0); i < numAttachedObjects; ++i){
		//		MovableObject *m = itr->second->getAttachedObject(i);
		//		type_t *tp = (type_t*)&(m->getUserAny());
		//		if (tp && tp->showInGame){
		//			sectorRoot->pushSceneNode(itr->second, tp->typeId);
		//			break;
		//		}
		//	}
		//}
	}
	void xgScene_t::enableRenderOptimize(bool enable){
		//renderOptimize = enable;
		//if (enable){
		//	sectorRoot->beginAddSector();
		//	for (SceneNodeList::iterator itr(mSceneNodes.begin()); itr != mSceneNodes.end(); ++itr){
		//		if (itr->second->numAttachedObjects() == 1){
		//			MovableObject *t = itr->second->getAttachedObject(0);
		//			type_t *tp = (type_t*)&(t->getUserAny());
		//			if (tp && tp->typeId == sector_type){
		//				sectorRoot->addSector(itr->second->_getWorldAABB());
		//			}
		//		}
		//	}
		//	sectorRoot->endAddSector();
		//	updateSectorSceneNode();
		//}
		//else{
		//	sectorRoot->clear();
		//}
	}
	void  xgScene_t::_findVisibleObjects(Camera *cam, VisibleObjectsBoundsInfo* visibleBounds, bool onlyShadowCasters){
		if (renderOptimize){
			return sectorRoot->findVisibleObjects(cam, getRenderQueue(), visibleBounds, true, mDisplayNodes, onlyShadowCasters);
		}
		else{
			return SceneManager::_findVisibleObjects(cam, visibleBounds, onlyShadowCasters);
		}
	}
	void saveVector3(const Vector3 &v, ofstream &o){
		o << "<x>" << v.x << "</x>" << "<y>" << v.y << "</y>" << "<z>" << v.z << "</z>";
	}
	void saveOrientation(const Quaternion &q, ofstream &o){
		o << "<x>" << q.x << "</x>" << "<y>" << q.y << "</y>" << "<z>" << q.z << "</z>" << "<w>" << q.w << "</w>";
	}
	void saveColourValue(const ColourValue &c, ofstream &o){
		o << "<r>" << c.r << "</r>" << "<g>" << c.g << "</g>" << "<b>" << c.b << "</b>" << "<a>" << c.a << "</a>";
	}
	//bool saveSceneNode(SceneNode*node,ofstream &o,bool editMode);
	bool saveEntity(MovableObject *me, ofstream &o, bool editMode){
		Entity *m = static_cast<Entity*>(me);
		if (m){
			o << "<type>" << "entity" << "</type>\n";
			o << "<name>" << m->getName() << "</name>\n";
			o << "<file>" << m->getMesh()->getName() << "</file>\n";
		}
		return true;
	}
	bool saveGameEntity(MovableObject *me, ofstream &o, bool editMode){
		//type_t *tp = (type_t *)&(me->getUserAny());
		//if (tp){
		//	o << "<type>" << "gameEntity" << "</type>\n";
		//	o<<"<name>"<<me->getName()<<"</name>\n";
		//	o<<"<template>"<<tp->userData<<"</template>\n";
		//}
		return true;
	}
	bool saveParticle(MovableObject *me, ofstream &o, bool editMode){
		//pfxType_t *tp = (pfxType_t*)&(me->getUserAny());
		//if (&tp){
		//	o << "<type>" << "pfx" << "</type>\n";
		//	o << "<name>" << me->getName() << "</name>\n";
		//	o << "<template>" << tp->templateName << "</template>\n";
		//}
		return true;
	}
	bool saveLight(MovableObject *me, ofstream &o, bool editMode){
		//Nick:“ÛÚ
		Light *m = static_cast<Light*>(me);
		if (m){
			o << "<type>" << "light" << "</type>\n";
			o << "<name>" << m->getName() << "</name>\n";
			o << "<diffuse>";
			saveColourValue(m->getDiffuseColour(), o);
			o << "</diffuse>\n";
			o << "<specular>";
			saveColourValue(m->getSpecularColour(), o);
			o << "</specular>\n";
			o << "<spotOuter>" << m->getSpotlightOuterAngle().valueRadians() << "</spotOuter>\n";
			o << "<spotInner>" << m->getSpotlightInnerAngle().valueRadians() << "</spotInner>\n";
			o << "<spotFalloff>" << m->getSpotlightFalloff() << "</spotFalloff>\n";
			o << "<range>" << m->getAttenuationRange() << "</range>\n";
			o << "<attenuationConst>" << m->getAttenuationConstant() << "</attenuationConst>\n";
			o << "<attenuationLinear>" << m->getAttenuationLinear() << "</attenuationLinear>\n";
			o << "<attenuationQuad>" << m->getAttenuationQuadric() << "</attenuationQuad>\n";
			o << "<powerScale>" << m->getPowerScale() << "</powerScale>\n";
		}
		return true;
	}
	bool isSceneNodeNeedSave(SceneNode*node){
		//if (!node)
		//	return false;
		//int numNode = node->numChildren();
		//int numObject = node->numAttachedObjects();
		//if (!(numNode || numObject)){
		//	return false;
		//}
		//bool hasNeedSaveEntity(false);
		//for (int i = 0; i < numObject; ++i){
		//	MovableObject *m = node->getAttachedObject(i);
		//	type_t *tp = (type_t*)&(m->getUserAny());
		//	if (tp && tp->saveable){
		//		switch (tp->typeId){
		//		case mesh_type:
		//		case fx_type:
		//		case light_type:
		//			hasNeedSaveEntity = true;
		//			break;
		//		default:
		//			hasNeedSaveEntity = false;
		//			break;
		//		}
		//		if (hasNeedSaveEntity)
		//			return true;
		//	}
		//}
		//for (int i = 0; i < numNode; ++i){
		//	Node *n = node->getChild(i);
		//	if (n && typeid(*n) == typeid(SceneNode)){
		//		if (isSceneNodeNeedSave(static_cast<SceneNode*>(n))){
		//			return true;
		//		}
		//	}
		//}
		return false;
	}
	bool saveSceneNode(SceneNode*node, ofstream &o, bool editMode){
		//int numNode = node->numChildren();
		//int numObject = node->numAttachedObjects();
		//o << "<sceneNode>\n"; {
		//	o << "<name>" << node->getName() << "</name>\n";
		//	o << "<p>";
		//	saveVector3(node->getPosition(), o);
		//	o << "</p>\n";
		//	o << "<r>";
		//	saveOrientation(node->getOrientation(), o);
		//	o << "</r>\n";
		//	o << "<s>";
		//	saveVector3(node->getScale(), o);
		//	o << "</s>\n";
		//	bool hasMoveObjects(false);
		//	for (int i = 0; i < numObject; ++i){
		//		MovableObject *m = node->getAttachedObject(i);

		//		type_t *tp = (type_t*)&(m->getUserAny());
		//		if (tp && tp->saveable){
		//			hasMoveObjects = true;
		//			break;
		//		}
		//	}
		//	if (hasMoveObjects){
		//		o << "<moveObjects>\n"; {
		//			for (int i = 0; i < numObject; ++i){
		//				MovableObject *m = node->getAttachedObject(i);
		//				if (editMode){
		//					type_t *tp = (type_t*)&(m->getUserAny());
		//					if (tp && tp->saveable){
		//						o << "<movableObject>\n";
		//						switch (tp->typeId){
		//						case mesh_type:{
		//							saveEntity(m, o, editMode);
		//							break; }
		//						case fx_type:{
		//							saveParticle(m, o, editMode);
		//							break; }
		//						case light_type:{
		//							saveLight(m, o, editMode);
		//							break; }
		//						}
		//						o << "</movableObject>\n";
		//					}
		//				}
		//			}
		//		}
		//		o << "</moveObjects>\n";
		//	}
		//	bool hasSceneNode(false);
		//	for (int i = 0; i < numNode; ++i){
		//		Node *n = node->getChild(i);
		//		if (n && typeid(*n) == typeid(SceneNode)){
		//			if (isSceneNodeNeedSave(static_cast<SceneNode*>(n))){
		//				hasSceneNode = true;
		//				break;
		//			}
		//		}
		//	}
		//	if (hasSceneNode){
		//		o << "<sceneNodes>\n";
		//		for (int i = 0; i < numNode; ++i){
		//			Node *n = node->getChild(i);
		//			if (n && typeid(*n) == typeid(SceneNode)){
		//				if (isSceneNodeNeedSave(static_cast<SceneNode*>(n)))
		//					saveSceneNode(static_cast<SceneNode*>(n), o, editMode);
		//			}
		//		}
		//		o << "</sceneNodes>\n";
		//	}
		//}
		//o << "</sceneNode>\n";
		return true;
	}
	void saveAABB(const AxisAlignedBox &me, ofstream &o){
		o << "<min>\n";
		saveVector3(me.getMinimum(), o);
		o << "</min>\n";
		o << "<max>\n";
		saveVector3(me.getMaximum(), o);
		o << "</max>\n";
	}
	//********************************************************
	static void loadVector3(Vector3 &v, xmlElement_t *o){
		o->get("x", v.x).get("y", v.y).get("z", v.z);
	}
	static void loadOrientation(Quaternion &q, xmlElement_t *o){
		o->get("x", q.x).get("y", q.y).get("z", q.z).get("w", q.w);
	}
	static void loadColourValue(ColourValue &c, xmlElement_t *o){
		o->get("r", c.r).get("g", c.g).get("b", c.b).get("a", c.a);
	}
	bool xgScene_t::loadPfx(SceneNode*node, xmlElement_t *o, bool editMode){
		string name;
		string file;
		o->get("name", name).get("template", file);
		MovableObject *me = createParticleSystem(name.c_str(), file.c_str());
		if (editMode)
			me->setUserAny(*new pfxType_t(fx_type_, file.c_str()));
		node->attachObject(me);
		return true;
	}

	const char *xgScene_t::getHelpMeshName(){
		static char name[1024];
		static int id;
		sprintf(name, "%s_%d", "dummy", ++id);
		while (hasManualObject(name)){
			sprintf(name, "%s_%d", "dummy", ++id);
		}
		return name;
	}
	bool xgScene_t::loadLight(SceneNode*node, xmlElement_t *o, bool editMode){
		string name;
		ColourValue diffuse;
		ColourValue specular;
		Real spotOuter;
		Real spotInner;
		Real spotFalloff;
		Real range;
		Real attenuationConst;
		Real attenuationLinear;
		Real attenuationQuad;
		Real powerScale;
		loadColourValue(diffuse, o->getChildByName("diffuse"));
		loadColourValue(specular, o->getChildByName("specular"));
		o->get("name", name)
			.get("spotOuter", spotOuter)
			.get("spotInner", spotInner)
			.get("spotFalloff", spotFalloff)
			.get("range", range)
			.get("attenuationConst", attenuationConst)
			.get("attenuationLinear", attenuationLinear)
			.get("attenuationQuad", attenuationQuad)
			.get("powerScale", powerScale);
		//Nick:“ÛÚ
		Light *l = createLight(name);
		if (l){
			l->setDiffuseColour(diffuse);
			l->setSpecularColour(specular);
			l->setSpotlightOuterAngle(Radian(spotOuter));
			l->setSpotlightInnerAngle(Radian(spotInner));
			l->setSpotlightFalloff(spotFalloff);
			l->setAttenuation(range, attenuationConst, attenuationLinear, attenuationQuad);
			l->setPowerScale(powerScale);
			if (editMode){
				l->setUserAny(*new type_t(light_type_));
				ManualObject *dummy = createBoxManualObject(getHelpMeshName(), LIGHT_COLOR, Vector3(-10, -10, -10), Vector3(10, 10, 10), "LightMesh");
				dummy->setUserAny(*new type_t(dummy_type_));
				node->attachObject(dummy);
			}
			node->attachObject(l);
		}

		return true;
	}

	bool xgScene_t::loadEntity(SceneNode*node, xmlElement_t *o, bool editMode){
		string name;
		string file;
		string templateName;
		o->get("name", name).get("file", file);

		Entity *entity = createEntity(name.c_str(), file.c_str());
		if (editMode)
			entity->setUserAny(*new type_t(mesh_type_));
		node->attachObject(entity);
		if (loadEntityCallback && loadEntityCallbackUserData){
			(*loadEntityCallback)(node, o, editMode, entity, loadEntityCallbackUserData);
		}
		return true;
	}
	bool xgScene_t::loadSceneNode(SceneNode*node, xmlElement_t *o, bool editMode){
		Vector3 pos;
		Vector3 scale;
		Quaternion q;
		xmlElement_t *posElement = o->getChildByName("p");
		loadVector3(pos, posElement);

		xmlElement_t *rotElement = o->getChildByName("r");
		loadOrientation(q, rotElement);

		xmlElement_t *scaleElement = o->getChildByName("s");
		loadVector3(scale, scaleElement);

		node->setPosition(pos);
		node->setOrientation(q);
		node->setScale(scale);

		xmlElement_t *moveObjects = o->getChildByName("moveObjects");
		xmlElement_t *sceneNodes = o->getChildByName("sceneNodes");

		if (moveObjects){
			string type;
			for (size_t i = 0; i < moveObjects->getChildrenCount(); ++i){
				xmlElement_t *moveObject = moveObjects->getChild(i);
				moveObject->get("type", type);
				if (type == "entity"){
					loadEntity(node, moveObject, editMode);
				}
				else if (type == "pfx"){
					loadPfx(node, moveObject, editMode);
				}
				else if (type == "light"){
					loadLight(node, moveObject, editMode);
				}/*
				else if(type=="gameEntity"){
				loadGameEntity(node,moveObject,editMode);
				}*/

			}
		}
		if (sceneNodes){
			string name;
			for (size_t i = 0; i < sceneNodes->getChildrenCount(); ++i){
				name.clear();
				xmlElement_t *sceneNode = sceneNodes->getChild(i);
				sceneNode->get("name", name);
				SceneNode *chileSceneNode = createSceneNode(name);
				node->addChild(chileSceneNode);
				loadSceneNode(chileSceneNode, sceneNode, editMode);
			}
		}
		return true;
	}
	static void loadAABB(AxisAlignedBox &me, xmlElement_t *o){
		Vector3 min, max;
		loadVector3(min, o->getChildByName("min"));
		loadVector3(max, o->getChildByName("max"));
		me.setMinimum(min);
		me.setMinimum(max);
	}
	//***************************************************
	bool xgScene_t::save(const char *name, bool editMode){
		return true;
	}
	bool xgScene_t::load(xmlElement_t *xml, bool editMode){
		selectSceneNode = NULL;
		xmlElement_t *o = xml->getChildByName("scene");
		if (!o)
			return false;
		reset(editMode);
		//____________________________________________________
		//resourceLocations
		xmlElement_t *resourceLocations = o->getChildByName("resourceLocations");
		if (resourceLocations){
			xmlElement_t *itr;
			string name;
			StringVector s = ResourceGroupManager::getSingleton().getResourceGroups();
			for (size_t i = 0; i < resourceLocations->getChildrenCount(); ++i){
				itr = resourceLocations->getChild(i);
				(*itr) >> name;
				StringVector::iterator itr(s.begin());
				for (; itr != s.end(); ++itr){
					if (*itr == name){
						break;
					}
				}
				if (itr == s.end())
					addResourceLocation(name.c_str());
			}
		}
		//resourceLocations
		//____________________________________________________
		xmlElement_t *sceneNode = o->getChildByName("sceneNode");
		if (sceneNode){
			loadSceneNode(getRootSceneNode(), sceneNode, editMode);
		}
		//load entityList
		//____________________________________________________
		xmlElement_t *entityList = o->getChildByName("entityList");
		if (editMode && entityList){
			xmlElement_t *entity;
			String name;
			for (size_t i = 0; i < entityList->getChildrenCount(); ++i){
				entity = entityList->getChild(i);
				string templateName;
				entity->get("name", name).get("template", templateName);
				SceneNode *node = createSceneNode(name);
				getRootSceneNode()->addChild(node);

				Vector3 pos;
				Vector3 scale;
				Quaternion q;
				xmlElement_t *posElement = entity->getChildByName("p");
				loadVector3(pos, posElement);

				xmlElement_t *rotElement = entity->getChildByName("r");
				loadOrientation(q, rotElement);

				xmlElement_t *scaleElement = entity->getChildByName("s");
				loadVector3(scale, scaleElement);

				node->setPosition(pos);
				node->setOrientation(q);
				node->setScale(scale);
				const char *meshFile = getEntityInfo(templateName.c_str());
				if (meshFile){
					Entity *entity = createEntity(name.c_str(), meshFile);
					type_t *type = new type_t(entity_type_);
					type->userData = templateName;
					entity->setUserAny(*type);
					node->attachObject(entity);
				}
				else{
					cout << "entity info " << templateName << " not find\n";
				}
			}
		}
		//load sector
		//____________________________________________________
		xmlElement_t *sectors = o->getChildByName("sectors");
		if (sectors){
			xmlElement_t *sector;
			String name;
			AxisAlignedBox ab;
			if (!editMode)
				sectorRoot->beginAddSector();
			for (size_t i = 0; i < sectors->getChildrenCount(); ++i){
				sector = sectors->getChild(i);
				ab.setNull();
				loadAABB(ab, sector);
				if (editMode){
					sector->get("name", name);
					Vector3 min, max;
					loadVector3(min, sector->getChildByName("min"));
					loadVector3(max, sector->getChildByName("max"));
					ManualObject *m = createBoxManualObject(name, SECTOR_COLOR, min, max, "sectorMesh");
					m->setUserAny(*new type_t(sector_type_));
					SceneNode *sceneNode = createSceneNode(name.c_str());
					sceneNode->attachObject(m);
					getRootSceneNode()->addChild(sceneNode);
					sceneNode->setPosition(min + (max - min)*.5f);
				}
				else{
					sectorRoot->addSector(ab);
				}
			}
			if (!editMode){
				sectorRoot->endAddSector();
				updateSectorSceneNode();
			}
		}
		//graph
		//____________________________________________________
		if (editMode){
			xmlElement_t *nodes = o->getChildByName("graph");
			if (nodes){
				xmlElement_t *node;
				String name;
				AxisAlignedBox ab;
				//if(!editMode)
				//	nodeRoot->beginAddSector();
				for (size_t i = 0; i < nodes->getChildrenCount(); ++i){
					node = nodes->getChild(i);
					ab.setNull();
					loadAABB(ab, node);
					if (editMode){
						node->get("name", name);
						Vector3 min, max;
						loadVector3(min, node->getChildByName("min"));
						loadVector3(max, node->getChildByName("max"));
						ManualObject *m = createBoxManualObject(name, AI_NAVI_COLOR, min, max, "aiNaviMesh");
						m->setUserAny(*new type_t(ai_navi_type_));
						SceneNode *sceneNode = createSceneNode(name.c_str());
						sceneNode->attachObject(m);
						getRootSceneNode()->addChild(sceneNode);
						sceneNode->setPosition(min + (max - min)*.5f);
					}
					else{
						//nodeRoot->addSector(ab);
					}
				}
				if (!editMode){
					//nodeRoot->endAddSector();
					//updateSectorSceneNode();
				}
			}
		}
		return true;
	}
	bool xgScene_t::load(const char *name, bool editMode){
		try{
			DataStreamPtr data = ResourceGroupManager::getSingleton().openResource(name, ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			if (data.isNull()){
				return false;
			}
			xmlStack_t x;
			char *buf = new char[data->size() + 1];
			buf[data->size()] = NULL;
			if (data->read(buf, data->size()) != data->size()){
				delete buf;
				return false;
			}
			if (!x.parseMem(buf, data->size())){
				delete buf;
				return false;
			}
			delete buf;
			return load((xmlElement_t *)&x, editMode);
		}
		catch (Exception& e) {
			std::cerr << "An exception has occured: " << e.getFullDescription();
		}
		catch (...){
		}
		return false;
	}
	void xgScene_t::createGridHelperMesh(){
		if (hasSceneNode("gridMesh"))
			return;
		ManualObject *grid = createManualObject("marke");
		grid->setUserAny(*new type_t(grid_helper_type_));
		grid->begin("gridMesh", RenderOperation::OT_LINE_LIST);
		float xwidth;
		float yheight;
		float gup;
		xwidth = 1000;
		yheight = 1000;
		gup = 20;
		xwidth /= 2;
		yheight /= 2;
		ColourValue gray(0.5f, 0.5f, 0.5f);
		ColourValue black(0.31f, 0.31f, 0.31f);
		for (float x = -xwidth; x < xwidth + 1; x += gup){
			grid->position(x, 0, -yheight);
			grid->normal(0, 1, 0);
			grid->textureCoord(0, 0);
			//grid->colour(0,1,0);
			grid->colour(gray);

			grid->position(x, 0, yheight);
			grid->normal(0, 1, 0);
			grid->textureCoord(0, 0);
			grid->colour(gray);

		}
		for (float z = -yheight; z < yheight + 1; z += gup){
			grid->position(-xwidth, 0, z);
			grid->normal(0, 1, 0);
			grid->textureCoord(0, 0);
			grid->colour(gray);

			grid->position(xwidth, 0, z);
			grid->normal(0, 1, 0);
			grid->textureCoord(0, 0);
			grid->colour(gray);
		}
		//----------------------------------------
		grid->position(0, 0, -yheight);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		//grid->colour(0,1,0);
		grid->colour(black);

		grid->position(0, 0, yheight);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(black);

		//----------------------------------------
		grid->position(-xwidth, 0, 0);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(black);

		grid->position(xwidth, 0, 0);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(black);
		//----------------------------------------
		//----------------------------------------z
		grid->position(0, 0, 0);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		//grid->colour(0,1,0);
		grid->colour(ColourValue(0, 0, 1, 1));

		grid->position(0, 0, 10);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(ColourValue(0, 0, 1, 1));

		//----------------------------------------x
		grid->position(0, 0, 0);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(ColourValue(1, 0, 0, 1));

		grid->position(10, 0, 0);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(ColourValue(1, 0, 0, 1));
		//----------------------------------------

		//----------------------------------------y
		grid->position(0, 0, 0);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(ColourValue(0, 1, 0, 1));

		grid->position(0, 10, 0);
		grid->normal(0, 1, 0);
		grid->textureCoord(0, 0);
		grid->colour(ColourValue(0, 1, 0, 1));
		//----------------------------------------
		grid->end();

		Ogre::SceneNode *mk = createSceneNode("gridMesh");
		getRootSceneNode()->addChild(mk);
		mk->attachObject(grid);
	}

	static void makeTrangle(ManualObject *me, const ColourValue &color, const Vector3 *p, int a, int b, int c){
		//ColourValue gray(0.0f,0.6f,0.0f,0.5f);
		Vector3 normal = (p[c] - p[b]).crossProduct(p[b] - p[a]);
		normal.normalise();
		me->position(p[a]);
		me->normal(normal);
		//manualObject->textureCoord(0,0);
		me->colour(color);

		me->position(p[b]);
		me->normal(normal);
		//manualObject->textureCoord(0,0);
		me->colour(color);

		me->position(p[c]);
		me->normal(normal);
		//manualObject->textureCoord(0,0);
		me->colour(color);

	}
	ManualObject *xgScene_t::createBoxManualObject(const String &name, const ColourValue &color, const Vector3 &min, const Vector3 &max, char *matName){
		ManualObject *o = createManualObject(name);
		updateBoxManualObject(o, color, min, max, matName);
		return o;
	}
	void xgScene_t::updateBoxManualObject(ManualObject *box, const ColourValue &color, const Vector3 &min, const Vector3 &max, char *matName){
		//Vector3 minp;
		//Vector3 maxp;
		Vector3 center;
		AxisAlignedBox aab;
		aab.setNull();
		aab.merge(min);
		aab.merge(max);
		center = aab.getCenter();
		Matrix4 mat(Matrix4::IDENTITY);
		mat.setTrans(-center);
		aab.transformAffine(mat);
		box->clear();
		box->begin(matName, RenderOperation::OT_TRIANGLE_LIST);
		const Vector3 *c = aab.getAllCorners();

		makeTrangle(box, color, c, 0, 3, 7);
		makeTrangle(box, color, c, 0, 7, 6);

		makeTrangle(box, color, c, 1, 5, 4);
		makeTrangle(box, color, c, 1, 4, 2);

		makeTrangle(box, color, c, 0, 6, 5);
		makeTrangle(box, color, c, 0, 5, 1);

		makeTrangle(box, color, c, 3, 2, 4);
		makeTrangle(box, color, c, 3, 4, 7);

		makeTrangle(box, color, c, 0, 1, 2);
		makeTrangle(box, color, c, 0, 2, 3);

		makeTrangle(box, color, c, 6, 7, 4);
		makeTrangle(box, color, c, 6, 4, 5);

		box->end();
	}
	bool xgScene_t::insertResource(const char *filePath){
		String file = filePath;//.mesh
		if (file.length() > 5 && file.substr(file.length() - 5, 5) == ".mesh"){
			return true;
		}
		return false;
	}
	void xgScene_t::reset(bool editMode){
		setSelSceneNode(NULL);
		clearScene();
		if (editMode)
			createGridHelperMesh();
	}
	string xgScene_t::getSubDir(const char *dir){
		char *rootPath = strdup(resRootPath.c_str());
		_strlwr(rootPath);
		char *lwrPath = strdup(dir);

		_strlwr(lwrPath);
		size_t t = strspn(lwrPath, rootPath);
		string retStr;
		if (t){
			retStr = lwrPath + t;
			string leftPath(lwrPath);
			leftPath = leftPath.substr(0, leftPath.length() - retStr.length());
			if (leftPath != rootPath){
				retStr = lwrPath;
			}
		}
		else
			retStr = lwrPath;

		free(rootPath);
		free(lwrPath);


		return retStr;
	}
	bool xgScene_t::addResourceLocation(const char *path)
	{
		StringVector s = ResourceGroupManager::getSingleton().getResourceGroups();
		StringVector::iterator itr(s.begin());
		for (; itr != s.end(); ++itr){
			if (*itr == path){
				break;
			}
		}
		if (itr == s.end())
		{
			ResourceGroupManager::getSingleton().addResourceLocation(path, "FileSystem", path);
			try{
				ResourceGroupManager::getSingleton().initialiseResourceGroup(path);
				return true;
			}
			catch (Exception& e) {
				MessageBoxA(0, ("An exception has occured: " + e.getFullDescription()).c_str(), "FATAL ERROR", 0);
			}
		}

		return false;
	}
	void xgScene_t::addEntityInfo(const char *templateName, const char *meshFile){
		entityInfoBag[templateName] = entityInfo_t(templateName, meshFile);
	}
	const char *xgScene_t::getEntityInfo(const char *templateName){
		entityInfoBag_t::iterator itr = entityInfoBag.find(templateName);
		if (itr != entityInfoBag.end()){
			return itr->second.meshFile.c_str();
		}
		return NULL;
	}
}