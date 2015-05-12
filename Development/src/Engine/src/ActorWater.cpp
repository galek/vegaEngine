/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"

//LWater
#include "../Hydrax/include/Hydrax.h"
#include "../Hydrax/include/Noise/FFT/FFT.h"
#include "../Hydrax/include/Noise/Perlin/Perlin.h"
#include "../Hydrax/include/Noise/Noise.h"
#include "../Hydrax/include/Modules/ProjectedGrid/ProjectedGrid.h"
#include "../Hydrax/include/Modules/RadialGrid/RadialGrid.h"
#include "../Hydrax/include/Modules/SimpleGrid/SimpleGrid.h"

namespace vega
{
	ActorWater::ActorWater()
		:water(nullptr)
	{
		mNode = nullptr;
		mEntity = nullptr;
		mName = "Water";//for Actor Base
		//hydrax package
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("..\\Engine\\Content\\Water.npk", "Zip", "Hydrax", true);
		Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Hydrax");

		_Init("Default.wsf");
	}
	//-----------------------------------------------------------------------------
	void ActorWater::AddWave(Ogre::Vector2 dir, float A, float T, float p)
	{
		TODO("TODO:надо сделать актер воды");
	}
	//-----------------------------------------------------------------------------
	void ActorWater::LoadCfg(const char _name)
	{}
	//-----------------------------------------------------------------------------
	ActorWater::~ActorWater(){
		SAFE_DELETE(water);
	}
	//-----------------------------------------------------------------------------
	void ActorWater::Update(float evt)	{
		water->update(evt);			
	}
	//-----------------------------------------------------------------------------
	void ActorWater::_Init(const std::string &_profile) {
		mG = GetEngine();
		Debug("WaterSystem::Init");
		water = new Hydrax(mG->mGSceneMgr, mG->mGCamera, mG->mGWindow->getViewport(0));
		Module::ProjectedGrid* mModule = new Module::ProjectedGrid( // модуль проекционной сетки
			water,  // указатель на главный класс LWater
			new Noise::Perlin(/* без особых параметров */),  // модуль для создания ряби
			Ogre::Plane(Ogre::Vector3(0, 1, 0), Ogre::Vector3(0, 0, 0)),  // водная поверхность
			MaterialManager::NM_RTT,  // режим карты нормалей
			Module::ProjectedGrid::Options());  // опции сетки
		TODO("Возможность задавать уровень моря");
		water->setModule(mModule);
		water->loadCfg(_profile);

		water->create();

		GetEngine()->GetSceneMgr()->_ContainActorWater(this);
		Debug("WaterSystem::Init finished");
	}
}