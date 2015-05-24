#include "GamePrivate.h"
//API
#include "APIEngineCommands.h"
//Core
#include "CoreSystems.h"
//Render
#include "Render.h"
//Engine
#include "EngineGlobals.h"
#include "world\Actor.h"
#include "ActorLight.h"
#include "ActorMesh.h"
#include "ActorSkyBox.h"
#include "ActorDynamicSky.h"
#include "ActorWater.h"
#include "camera\cameraFPS.h"
#include "EmitterSound.h"
#include "EmitterParticle.h"
#include "VideoPlayer.h"
#include "Scripting.h"
#include "SceneManager.h"

namespace vega
{
	Game::Game()
	{}

	Game::~Game()
	{
		SAFE_DELETE(sound);
	}

	void Game::PreInitialize(){
		//File system is not Initialized
	}

	void Game::CreateScene()	{
		GetEngine()->sceneManager->SetAmbientLight(0.15, 0.00, 0.00);
		auto l1 = ActorLight::DirectLight("gsds");
		l1->setDiffuse(0.5f, 0.45f, 0.1f);
		l1->setDirection(Ogre::Vector3(1, -0.5, -0.2));
		l1->setShadowFarClipDistance(250);
		l1->setShadowFarDistance(75);
		//Turn this on to have the directional light cast shadows
		l1->setCastShadows(false);
		API::RunScriptFunctionByName("EngineMain.lua", "EngineMain");
	}

	void Game::_enableStohMusic(){
	}

	void Game::Update(float _time){
	}
}