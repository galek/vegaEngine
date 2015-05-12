#pragma once


namespace vega
{
	class sector_t :public std::vector<sector_t*>, public AxisAlignedBox{
	protected:
		typedef std::vector<sector_t*> sectorBag_t;
		typedef std::vector<SceneNode *> sceneNodeBag_t;
		typedef std::map<int, sceneNodeBag_t*> SceneNodeBagMap_t;
		SceneNodeBagMap_t sceneNodeBagMap;
		sectorBag_t neighborSectorBag;
	public:
		void clear();
		void findVisibleObjects(Camera *cam, RenderQueue * queue, VisibleObjectsBoundsInfo* visibleBounds, bool includeChildren, bool displayNodes, bool onlyShadowCasters, bool neighbor);
		void populateLightList(const Vector3 &position, Real radius, LightList &destList, int lightId);
		bool pushSceneNode(SceneNode *o, int typeId);
		bool pushSector(sector_t *me);
		~sector_t();
	};

	class sectorRoot_t :private sector_t{
		sector_t rootSector;
	public:
		void clear();
		void findVisibleObjects(Camera *cam, RenderQueue * queue, VisibleObjectsBoundsInfo* visibleBounds, bool includeChildren, bool displayNodes, bool onlyShadowCasters);
		void populateLightList(const Vector3 &position, Real radius, LightList &destList, int lightId);
		void beginAddSector();
		void addSector(const AxisAlignedBox &aab);
		void endAddSector();
		bool pushSceneNode(SceneNode *o, int typeId);
		~sectorRoot_t();
	};
}