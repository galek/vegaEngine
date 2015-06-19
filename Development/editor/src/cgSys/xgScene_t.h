#pragma once


namespace vega
{
	const int xgSceneType = Ogre::ST_INTERIOR + 4;
	class selectListener_t;
	class sectorRoot_t;
	class xmlElement_t;
	typedef void(*loadEntityCallback_t)(Ogre::SceneNode*node, xmlElement_t *o, bool editMode, Ogre::Entity *ent, void *userData);

	class xgScene_t :public Ogre::SceneManager
	{
		struct entityInfo_t{
			std::string templateName;
			std::string meshFile;
			entityInfo_t(const char *templateName, const char *meshFile)
				:templateName(templateName), meshFile(meshFile){}
			entityInfo_t(){}
		};
		typedef std::map<std::string, entityInfo_t> entityInfoBag_t;
		entityInfoBag_t entityInfoBag;
		typedef std::vector<selectListener_t*> selectListenerBag_t;
		selectListenerBag_t selectListenerBag;
		sectorRoot_t *sectorRoot;
		loadEntityCallback_t loadEntityCallback;
		void *loadEntityCallbackUserData;
		bool renderOptimize;
		bool loadLight(Ogre::SceneNode*node, xmlElement_t *o, bool editMode);
		bool loadSceneNode(Ogre::SceneNode*node, xmlElement_t *o, bool editMode);
		bool loadEntity(Ogre::SceneNode*node, xmlElement_t *o, bool editMode);
		bool loadPfx(Ogre::SceneNode*node, xmlElement_t *o, bool editMode);
		void updateSectorSceneNode();
	public:
		void addEntityInfo(const char *templateName, const char *meshFile);
		const char *getEntityInfo(const char *templateName);
		std::string getSubDir(const char *dir);
		bool addResourceLocation(const char *path);
		void setResRootPath(std::string _resRootPath){ this->resRootPath = _resRootPath; MessageBoxA(0, _resRootPath.c_str(), resRootPath.c_str(), 0); }
		void enableRenderOptimize(bool enable);
		void pushSelectListener(selectListener_t *me);
		void delSelectListener(selectListener_t *me);

		void SetSelectObject(Actor *me);
		
		void setSelMovable(Ogre::MovableObject *me);
		bool getViewRay(long x, long y, Ogre::Ray &ray);
		Ogre::SceneNode *pickSceneNode(long x, long y);
		Ogre::MovableObject *pickMoveable(long x, long y);
		xgScene_t(const Ogre::String& name);
		~xgScene_t();
		const Ogre::String& getTypeName(void) const;
		bool insertResource(const char *filePath);
		void setLoadEntityCallback(loadEntityCallback_t me, void *userData){
			loadEntityCallback = me;
			loadEntityCallbackUserData = userData;
		}
		bool save(const char *name, bool editMode);
		bool load(const char *name, bool editMode);
		bool load(xmlElement_t *xml, bool editMode);
		/*
		Очистка сцены редактора
		*/
		void ResetScene(bool editMode);


		virtual void  _findVisibleObjects(Ogre::Camera *cam, Ogre::VisibleObjectsBoundsInfo* visibleBounds, bool onlyShadowCasters);
		virtual void  destroyAllMovableObjects(void);
		virtual void  destroyMovableObject(const Ogre::String &name, const Ogre::String &typeName);

		Ogre::ManualObject *createBoxManualObject(const Ogre::String &name, const Ogre::ColourValue &color, const Ogre::Vector3 &min, const Ogre::Vector3 &max, char *matName);
		/*Deprecated*/
		void updateBoxManualObject(Ogre::ManualObject *box, const Ogre::ColourValue &color, const Ogre::Vector3 &min, const Ogre::Vector3 &max, char *matName);
		const char *getHelpMeshName();
	private:
		std::string resRootPath;
		Actor *m_SelectedObject;
	};
}