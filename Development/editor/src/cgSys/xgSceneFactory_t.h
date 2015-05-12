#pragma once

namespace vega
{
	class xgSceneFactory_t : public Ogre::SceneManagerFactory{
	protected:
		void initMetaData(void) const;
	public:
		static const Ogre::String FACTORY_TYPE_NAME;
		xgSceneFactory_t() {}
		~xgSceneFactory_t() {}
		/// Factory type name
		Ogre::SceneManager* createInstance(const Ogre::String& instanceName);
		void destroyInstance(Ogre::SceneManager* instance);
	};
}