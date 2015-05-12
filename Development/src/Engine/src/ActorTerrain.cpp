#include "EnginePrivate.h"

#if 0

//#define DEBUG_TERRAIN
namespace EngineModule
{
	ActorTerrain::ActorTerrain(float mapSize,float  worldSize,float scale,std::string _HeightMapName)
		:mTerrainGlobals(nullptr),mTerrainGroup(nullptr),mMapSize(mapSize),mWorldSize(worldSize),
		mScale(scale),mHeightMapName(_HeightMapName)
	{
		Debug("ActorTerrain:%f,%f,%f",mMapSize,mWorldSize,mScale);
		setupTerrain();
	}
	void ActorTerrain::setupTerrain()
	{
	using namespace Ogre;


        // add a bright light above the scene
 /*Nick       mLight = mSceneMgr->createLight();
        mLight->setType(Light::LT_POINT);
        mLight->setPosition(-10, 40, 20);
        mLight->setSpecularColour(ColourValue::White);
*/


        mTerrainGlobals = new TerrainGlobalOptions();
		mTerrainGroup = new TerrainGroup(engine.mGSceneMgr, Terrain::ALIGN_X_Z, 65, 100);
        mTerrainGroup->setOrigin(Ogre::Vector3(0,0,0));


        // Configure global
        mTerrainGlobals->setMaxPixelError(8);
        // testing composite map
        mTerrainGlobals->setCompositeMapDistance(3000);
        //mTerrainGlobals->setUseRayBoxDistanceCalculation(true);
        //mTerrainGlobals->getDefaultMaterialGenerator()->setDebugLevel(1);
        //mTerrainGlobals->setLightMapSize(256);


        //matProfile->setLightmapEnabled(false);
        // Important to set these so that the terrain knows what to use for derived (non-realtime) data
//NICK        mTerrainGlobals->setLightMapDirection(mLight->getDerivedDirection());
		mTerrainGlobals->setCompositeMapAmbient(engine.mGSceneMgr->getAmbientLight());
        //mTerrainGlobals->setCompositeMapAmbient(ColourValue::Red);
        mTerrainGlobals->setCompositeMapDiffuse(ColourValue::White);


        // Configure default import settings for if we use imported image
        Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
        defaultimp.terrainSize = 65;
        defaultimp.worldSize = 100;
        defaultimp.inputScale = 20;
        defaultimp.minBatchSize = 33;
        defaultimp.maxBatchSize = 65;
        // textures
        defaultimp.layerList.resize(3);
        defaultimp.layerList[0].worldSize = 100;
        defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
        defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
        defaultimp.layerList[1].worldSize = 30;
        defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
        defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
        defaultimp.layerList[2].worldSize = 200;
        defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
        defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");


        Image img;
        img.load("terrain.png", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
        mTerrainGroup->defineTerrain(0, 0,&img);
        //mTerrainGroup->defineTerrain(-1, 0,&img);
        //mTerrainGroup->defineTerrain(0, -1,&img);
        //mTerrainGroup->defineTerrain(0, 0,&img);


        mTerrainGroup->loadAllTerrains(true);
        TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();
        while(ti.hasMoreElements())
        {
                Terrain* terrain = ti.getNext()->instance;
                TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
                TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
                float minHeight0 = 70;
                float fadeDist0 = 40;
                float minHeight1 = 70;
                float fadeDist1 = 15;
                float * pBlend0 = blendMap0->getBlendPointer();
                float* pBlend1 = blendMap1->getBlendPointer();
                for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y)
                {
                        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x)
                        {
                                float tx, ty;


                                blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
                                float height = terrain->getHeightAtTerrainPosition(tx, ty);
                                float val = (height - minHeight0) / fadeDist0;
                                val = Ogre::Math::Clamp(val, (float)0, (float)1);
                                *pBlend0++ = val;


                                val = (height - minHeight1) / fadeDist1;
                                val = Ogre::Math::Clamp(val, (float)0, (float)1);
                                *pBlend1++ = val;
                        }
                }
                blendMap0->dirty();
                blendMap1->dirty();
                //blendMap0->loadImage("blendmap1.png", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
                blendMap0->update();
                blendMap1->update();


                // set up a colour map
                
                /*if (!terrain->getGlobalColourMapEnabled())
                {
                terrain->setGlobalColourMapEnabled(true);
                Image colourMap;
                colourMap.load("RustedMetal.jpg", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
                terrain->getGlobalColourMap()->loadImage(colourMap);
                }*/
                
        }
        mTerrainGroup->freeTemporaryResources();        


        ti = mTerrainGroup->getTerrainIterator();
        while (ti.hasMoreElements())
        {
                Terrain * t = ti.getNext()->instance;
                engine.physics->mRenderSystem->createTerrain(t);
        }


        //mRaySceneQuery = mSceneMgr->createRayQuery(Ogre::Ray());
	}
}
#endif