#include "AiPrivate.h"
#include "OgreRecastNavmeshPruner.h"
#include "libs/Detour/DetourAssert.h"



namespace vega
{
	class PolyRefArray
	{
		dtPolyRef* m_data;
		int m_size, m_cap;
		inline PolyRefArray(const PolyRefArray&);
		inline PolyRefArray& operator=(const PolyRefArray&);
	public:

		inline PolyRefArray() : m_data(0), m_size(0), m_cap(0) {}
		inline PolyRefArray(int n) : m_data(0), m_size(0), m_cap(0) { resize(n); }
		inline ~PolyRefArray() { dtFree(m_data); }
		void resize(int n)
		{
			if (n > m_cap)
			{
				if (!m_cap) m_cap = n;
				while (m_cap < n) m_cap *= 2;
				dtPolyRef* newData = (dtPolyRef*)dtAlloc(m_cap*sizeof(dtPolyRef), DT_ALLOC_TEMP);
				if (m_size && newData) memcpy(newData, m_data, m_size*sizeof(dtPolyRef));
				dtFree(m_data);
				m_data = newData;
			}
			m_size = n;
		}
		inline void push(int item) { resize(m_size + 1); m_data[m_size - 1] = item; }
		inline dtPolyRef pop() { if (m_size > 0) m_size--; return m_data[m_size]; }
		inline const dtPolyRef& operator[](int i) const { return m_data[i]; }
		inline dtPolyRef& operator[](int i) { return m_data[i]; }
		inline int size() const { return m_size; }
	};


	class NavmeshFlags
	{
		struct TileFlags
		{
			inline void purge() { dtFree(flags); }
			unsigned char* flags;
			int nflags;
			dtPolyRef base;
		};

		const dtNavMesh* m_nav;
		TileFlags* m_tiles;
		int m_ntiles;

	public:
		NavmeshFlags() :
			m_nav(0), m_tiles(0), m_ntiles(0)
		{
		}

		~NavmeshFlags()
		{
			for (int i = 0; i < m_ntiles; ++i)
				m_tiles[i].purge();
			dtFree(m_tiles);
		}

		bool init(const dtNavMesh* nav)
		{
			m_ntiles = nav->getMaxTiles();
			if (!m_ntiles)
				return true;
			m_tiles = (TileFlags*)dtAlloc(sizeof(TileFlags)*m_ntiles, DT_ALLOC_TEMP);
			if (!m_tiles)
			{
				return false;
			}
			memset(m_tiles, 0, sizeof(TileFlags)*m_ntiles);

			// Alloc flags for each tile.
			for (int i = 0; i < nav->getMaxTiles(); ++i)
			{
				const dtMeshTile* tile = nav->getTile(i);
				if (!tile->header) continue;
				TileFlags* tf = &m_tiles[i];
				tf->nflags = tile->header->polyCount;
				tf->base = nav->getPolyRefBase(tile);
				if (tf->nflags)
				{
					tf->flags = (unsigned char*)dtAlloc(tf->nflags, DT_ALLOC_TEMP);
					if (!tf->flags)
						return false;
					memset(tf->flags, 0, tf->nflags);
				}
			}

			m_nav = nav;

			return false;
		}

		inline void clearAllFlags()
		{
			for (int i = 0; i < m_ntiles; ++i)
			{
				TileFlags* tf = &m_tiles[i];
				if (tf->nflags)
					memset(tf->flags, 0, tf->nflags);
			}
		}

		inline unsigned char getFlags(dtPolyRef ref)
		{
			dtAssert(m_nav);
			dtAssert(m_ntiles);
			// Assume the ref is valid, no bounds checks.
			unsigned int salt, it, ip;
			m_nav->decodePolyId(ref, salt, it, ip);
			return m_tiles[it].flags[ip];
		}

		inline void setFlags(dtPolyRef ref, unsigned char flags)
		{
			dtAssert(m_nav);
			dtAssert(m_ntiles);
			// Assume the ref is valid, no bounds checks.
			unsigned int salt, it, ip;
			m_nav->decodePolyId(ref, salt, it, ip);
			m_tiles[it].flags[ip] = flags;
		}

	};



	OgreRecastNavmeshPruner::OgreRecastNavmeshPruner(AI *recast, dtNavMesh *navMesh)
		: mNavmesh(navMesh)
		, mRecast(recast)
	{
		mFlags = new NavmeshFlags();
		mFlags->init(mNavmesh);
	}

	bool OgreRecastNavmeshPruner::floodNavmesh(Ogre::Vector3 startPoint)
	{
		unsigned char flag = 1; // Flag to assign to all marked polys (must not be already assigned)

		// First find nearest navmesh poly to startPoint
		dtPolyRef startPoly;
		Ogre::Vector3 foundPt;
		if (!mRecast->findNearestPolyOnNavmesh(startPoint, foundPt, startPoly))
			return false;

		floodNavmesh(startPoly, flag);
		return true;
	}

	void OgreRecastNavmeshPruner::floodNavmesh(dtPolyRef start, unsigned char flag)
	{
		// If already visited, skip.
		if (mFlags->getFlags(start))
			return;

		PolyRefArray openList;
		openList.push(start);

		while (openList.size())
		{
			const dtPolyRef ref = openList.pop();
			// Get current poly and tile.
			// The API input has been cheked already, skip checking internal data.
			const dtMeshTile* tile = 0;
			const dtPoly* poly = 0;
			mNavmesh->getTileAndPolyByRefUnsafe(ref, &tile, &poly);

			// Visit linked polygons.
			for (unsigned int i = poly->firstLink; i != DT_NULL_LINK; i = tile->links[i].next)
			{
				const dtPolyRef neiRef = tile->links[i].ref;
				// Skip invalid and already visited.
				if (!neiRef || mFlags->getFlags(neiRef))
					continue;
				// Mark as visited
				mFlags->setFlags(neiRef, flag);
				// Visit neighbours
				openList.push(neiRef);
			}
		}
	}

	void OgreRecastNavmeshPruner::disableUnvisitedPolys()
	{
		for (int i = 0; i < mNavmesh->getMaxTiles(); ++i)
		{
			const dtMeshTile* tile = ((const dtNavMesh*)mNavmesh)->getTile(i);
			if (!tile->header) continue;
			const dtPolyRef base = mNavmesh->getPolyRefBase(tile);
			for (int j = 0; j < tile->header->polyCount; ++j)
			{
				const dtPolyRef ref = base | (unsigned int)j;
				if (!mFlags->getFlags(ref))
				{
					unsigned short f = 0;

					// Assign DISABLED flag to not-connected polygons
					mNavmesh->getPolyFlags(ref, &f);
					mNavmesh->setPolyFlags(ref, f | SAMPLE_POLYFLAGS_DISABLED);
				}
			}
		}
	}

	void OgreRecastNavmeshPruner::clearSelection()
	{
		mFlags->clearAllFlags();
	}

	void OgreRecastNavmeshPruner::pruneSelected()
	{
		disableUnvisitedPolys();
		//    delete m_flags;
		//    m_flags = 0;
	}
}