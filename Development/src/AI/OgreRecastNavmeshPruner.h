#pragma once
#include "OgreRecast.h"

namespace vega
{
	/**
	 * Tool to flood-fill the navmesh polygons starting at a specific polygon and marking
	 * all reachable neighbours. This allows to prune off unreachable parts of the navmesh.
	 *
	 * Based on NavMeshPruneTool from the original recast sample.
	 * The navmesh prune tool allows to disable unreachable polygons. There is currently no
	 * way to discard the disabled polys, because the search is done on the final navmesh
	 * data (when it is already rasterized) and it is really hard to modify.
	 **/
	class OgreRecastNavmeshPruner
	{
	public:
		OgreRecastNavmeshPruner(AI *recast, dtNavMesh *navMesh);
		bool floodNavmesh(Ogre::Vector3 startPoint);
		void clearSelection(void);
		void pruneSelected(void);

	protected:
		void floodNavmesh(dtPolyRef start, unsigned char flag);
		void disableUnvisitedPolys(void);

		AI *mRecast;
		dtNavMesh *mNavmesh;
		class NavmeshFlags* mFlags;

	};
}