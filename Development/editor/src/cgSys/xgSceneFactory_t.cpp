#include "stdafx.h"
#include "xgSceneFactory_t.h"
#include "xgScene_t.h"


namespace vega
{
	const String xgSceneFactory_t::FACTORY_TYPE_NAME = "xgSceneFactory";
	void xgSceneFactory_t::initMetaData(void) const{
		mMetaData.typeName = FACTORY_TYPE_NAME;
		mMetaData.description = "xg scene manager";
		mMetaData.sceneTypeMask = xgSceneType;
		mMetaData.worldGeometrySupported = false;
	}
	Ogre::SceneManager* xgSceneFactory_t::createInstance(const String& instanceName){
		return new xgScene_t(instanceName);
	}
	void xgSceneFactory_t::destroyInstance(Ogre::SceneManager* instance){
		delete instance;
	}
}