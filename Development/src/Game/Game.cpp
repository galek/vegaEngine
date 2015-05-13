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
		API::RunScriptFunctionByName("EngineMain.lua", "EngineMain");
	}

	void Game::_enableStohMusic(){
	}

	void Game::Update(float _time){
	}
}