/* VG CONFIDENTIAL
* VegaEngine(TM) Package 0.5.5.0
* Copyright (C) 2009-2014 Vega Group Ltd.
* Author: Nick Galko
* E-mail: nick.galko@vegaengine.com
* All Rights Reserved.
*/
#include "EnginePrivate.h"
//Externals
#include "lua\lua.hpp"
#include "luabind\luabind.hpp"
#pragma comment(lib,"lua.lib")
//Core
#include "Scripting.h"
//Engine
#include "SceneManager.h"
#include "ConsoleCommands.h"
#include "EngineScriptingEx.h"
#include "WrappedScriptFunctions.h"
#include "ActorWater.h"
#include "ActorMesh.h"
#include "ActorDynamicSky.h"
#include "ActorSkyDome.h"
#include "ActorSkyBox.h"
#include "ActorSkyPlane.h"
#include "Camera/CameraFree.h"
#include "Camera/CameraFPS.h"
#include "EmitterSound.h"
//API
#include "APIEngineCommands.h"

namespace vega
{
	//-------------------------------------------------------------------------------------
	void _EngineAddScriptFunctionaluty(Scripting* mScript)
	{
		using namespace luabind;
		using namespace vega;
		using namespace ConsoleCommands;
		using namespace Ogre;

		//Engine API
		module(mScript->getLua(), "api")
			[
				//API
				def("RunScript", &API::RunScript),
				def("SetPause", &API::SetPause),
				def("RunConsoleCommand", &API::RunScript),
				def("PlayVideo", &API::PlayVideo),
				def("IsPlayingVideo", &API::IsPlayingVideo),
				def("LoadLevel", &API::LoadLevel),
				def("EngineShoutdown", &API::EngineShoutdown),
				def("AttemptConfig", &API::AttemptConfig),
				def("SetMouseLock", &API::SetMouseLock),
				def("SetBufferedUnBufferedMouseMode", &API::SetBufferedUnBufferedMouseMode),
				def("SetShowGUICursor", &API::SetShowGUICursor)
			];

		module(mScript->getLua())
			[
				//GlobalFunctions
				def("GetEngine", &GetEngine),
				//ConsoleCommands
				def("Console_LoadLevel", &LoadLevel),
				def("Console_PlayV", &PlayV),
				def("Console_EngineShoutdown", &EngineShoutdown),
				def("Console_RunScript", &RunScript),
				def("Console_R_AA", &R_AA),
				def("Console_R_HDR", &R_HDR),
				def("Console_R_SSAO", &R_SSAO),
				//Engine
				class_ <EngineGlobals>("EngineGlobals")
				.def("RunScript", &EngineGlobals::RunScript)
				.def("RunConsoleCommand", &EngineGlobals::RunConsoleCommand)
				.def("plugins", &EngineGlobals::GetPhysics)
				.def("render", &EngineGlobals::GetRender)
				.def("audio", &EngineGlobals::GetAudio)
				.def("sceneManager", &EngineGlobals::GetSceneMgr)
				.def("game", &EngineGlobals::GetGame)
				//Actor
				, class_ <Actor>("Actor")
				.def("setMass", &Actor::setMass)
				.def("setCollisionModel", &Actor::setCollisionModel)
				.def("setMaterialName", &Actor::setMaterialName)
				.def("getName", &Actor::getName)
				.def("getFileName", &Actor::getFileName)
				.def("getPosition", &Actor::getPosition)
				.def("setOrientation", &Actor::setOrientation)
				.def("setCastShadows", &Actor::setCastShadows)
				.def("calculateSizeUsingAxisAlignedBox", &Actor::calculateSizeUsingAxisAlignedBox)
				.def("setYaw", &Actor::setYaw)
				.def("setPosition", (void (Actor::*)(const Ogre::Vector3&)) &Actor::setPosition)
				.def("setPosition", (void (Actor::*)(float _x, float _y, float _z)) &Actor::setPosition)
				.def("setScale", (void (Actor::*)(const Ogre::Vector3&)) &Actor::setScale)
				.def("setScale", (void (Actor::*)(float _x, float _y, float _z)) &Actor::setScale)
				.def("setLinearVelocity", (void (Actor::*)(const Ogre::Vector3&)) &Actor::setLinearVelocity)
				.def("setLinearVelocity", (void (Actor::*)(float _x, float _y, float _z)) &Actor::setLinearVelocity)
				//ActorDynamicSky
				, class_ <ActorDynamicSky, bases<Actor>>("ActorDynamicSky")
				.def(luabind::constructor<>())
				.def("setMaterialName", &ActorDynamicSky::setMaterialName)
				.def("getName", &ActorDynamicSky::getName)
				.def("setCastShadows", &ActorDynamicSky::setCastShadows)
				//ActorWater
				, class_ <ActorWater, bases<Actor>>("ActorWater")
				.def(luabind::constructor<>())
				.def("AddWave", &ActorWater::AddWave)
				//ActorSkyDome
				, class_ <ActorSkyDome, bases<Actor>>("ActorSkyDome")
				.def(luabind::constructor<>())
				.def("setCollisionModel", &ActorSkyDome::setCollisionModel)
				.def("setMaterialName", &ActorSkyDome::setMaterialName)
				.def("getName", &ActorSkyDome::getName)
				.def("getFileName", &ActorSkyDome::getFileName)
				.def("getPosition", &ActorSkyDome::getPosition)
				.def("setOrientation", &ActorSkyDome::setOrientation)
				.def("setCastShadows", &ActorSkyDome::setCastShadows)
				.def("calculateSizeUsingAxisAlignedBox", &ActorSkyDome::calculateSizeUsingAxisAlignedBox)
				.def("setPosition", (void (ActorSkyDome::*)(const Ogre::Vector3&)) &ActorSkyDome::setPosition)
				.def("setPosition", (void (ActorSkyDome::*)(float _x, float _y, float _z)) &ActorSkyDome::setPosition)
				.def("setScale", (void (ActorSkyDome::*)(const Ogre::Vector3&)) &ActorSkyDome::setScale)
				.def("setScale", (void (ActorSkyDome::*)(float _x, float _y, float _z)) &ActorSkyDome::setScale)
				//ActorSkyBox
				, class_<ActorSkyBox, bases<Actor>>("ActorSkyBox")
				.def(luabind::constructor<>())
				.def("setCollisionModel", &ActorSkyBox::setCollisionModel)
				.def("setMaterialName", &ActorSkyBox::setMaterialName)
				.def("getName", &ActorSkyBox::getName)
				.def("getFileName", &ActorSkyBox::getFileName)
				.def("getPosition", &ActorSkyBox::getPosition)
				.def("setOrientation", &ActorSkyBox::setOrientation)
				.def("setCastShadows", &ActorSkyBox::setCastShadows)
				.def("setPosition", (void (ActorSkyBox::*)(const Ogre::Vector3&)) &ActorSkyBox::setPosition)
				.def("setPosition", (void (ActorSkyBox::*)(float _x, float _y, float _z)) &ActorSkyBox::setPosition)
				.def("setScale", (void (ActorSkyBox::*)(const Ogre::Vector3&)) &ActorSkyBox::setScale)
				.def("setScale", (void (ActorSkyBox::*)(float _x, float _y, float _z)) &ActorSkyBox::setScale)
				.def("calculateSizeUsingAxisAlignedBox", &ActorSkyBox::calculateSizeUsingAxisAlignedBox)
				//ActorSkyPlane
				, class_<ActorSkyPlane, bases<Actor>>("ActorSkyPlane")
				.def(luabind::constructor<const char*>())
				.def(luabind::constructor<>())
				.def(luabind::constructor<const char*, const Ogre::Vector4&>())
				.def(luabind::constructor<const char*, float, float, float, float >())
				.def("setCollisionModel", &ActorSkyPlane::setCollisionModel)
				.def("setMaterialName", &ActorSkyPlane::setMaterialName)
				.def("getName", &ActorSkyPlane::getName)
				.def("getFileName", &ActorSkyPlane::getFileName)
				.def("getPosition", &ActorSkyPlane::getPosition)
				.def("setOrientation", &ActorSkyPlane::setOrientation)
				.def("setCastShadows", &ActorSkyPlane::setCastShadows)
				.def("calculateSizeUsingAxisAlignedBox", &ActorSkyPlane::calculateSizeUsingAxisAlignedBox)
				.def("setPosition", (void (ActorSkyPlane::*)(const Ogre::Vector3&)) &ActorSkyPlane::setPosition)
				.def("setPosition", (void (ActorSkyPlane::*)(float _x, float _y, float _z)) &ActorSkyPlane::setPosition)
				.def("setScale", (void (ActorSkyPlane::*)(const Ogre::Vector3&)) &ActorSkyPlane::setScale)
				.def("setScale", (void (ActorSkyPlane::*)(float _x, float _y, float _z)) &ActorSkyPlane::setScale)
				//ActorMesh
				, class_<ActorMesh, bases<Actor>>("ActorMesh")
				.def(luabind::constructor<const char *, const char*, const char*,
				int, float, Vector3, Quaternion, Vector3, Vector3>())
				.def(luabind::constructor<const char*, const char*>())
				.def(luabind::constructor<const char*, const char*, const char*>())
				.def("setMaterialName", &ActorMesh::setMaterialName)
				.def("setMass", &ActorMesh::setMass)
				.def("setCollisionModel", &ActorMesh::setCollisionModel)
				.def("setMaterialName", &ActorMesh::setMaterialName)
				.def("getName", &ActorMesh::getName)
				.def("getFileName", &ActorMesh::getFileName)
				.def("getPosition", &ActorMesh::getPosition)
				.def("setOrientation", &ActorMesh::setOrientation)
				.def("setCastShadows", &ActorMesh::setCastShadows)
				.def("calculateSizeUsingAxisAlignedBox", &ActorMesh::calculateSizeUsingAxisAlignedBox)
				.def("setYaw", &ActorMesh::setYaw)
				.def("setPosition", (void (ActorMesh::*)(const Ogre::Vector3&)) &ActorMesh::setPosition)
				.def("setPosition", (void (ActorMesh::*)(float _x, float _y, float _z)) &ActorMesh::setPosition)
				.def("setScale", (void (ActorMesh::*)(const Ogre::Vector3&)) &ActorMesh::setScale)
				.def("setScale", (void (ActorMesh::*)(float _x, float _y, float _z)) &ActorMesh::setScale)
				.def("setLinearVelocity", (void (ActorMesh::*)(const Ogre::Vector3&)) &ActorMesh::setLinearVelocity)
				.def("setLinearVelocity", (void (ActorMesh::*)(float _x, float _y, float _z)) &ActorMesh::setLinearVelocity)
				.def("buildNavMesh", &ActorMesh::buildNavMesh)
				.def("rebuildNavMesh", &ActorMesh::rebuildNavMesh)
				.def("destroyNavMesh", &ActorMesh::destroyNavMesh)
				//CameraFree
				, class_<CameraFree, bases<Actor>>("CameraFree")
				.def(luabind::constructor<>())
				.def(luabind::constructor<bool>())
				.def("setYaw", &CameraFree::setYaw)
				.def("setPitch", &CameraFree::setPitch)
				//CameraFPS
				, class_<CameraFPS, bases<Actor>>("CameraFPS")
				.def(luabind::constructor<>())
				.def(luabind::constructor<bool>())
				//.def("setYaw", &CameraFree::setYaw)
				//.def("setPitch", &CameraFree::setPitch)
				//EmitterSound
				, class_<EmitterSound, bases<Actor>>("EmitterSound")
				.def(luabind::constructor<const std::string&, const std::string&, bool, bool>())
				.def(luabind::constructor<const std::string&, const std::string&, bool>())
				.def(luabind::constructor<const std::string&, const std::string&>())
				.def("play", &EmitterSound::play)
				.def("isPlaying", &EmitterSound::isPlaying)
				.def("pause", &EmitterSound::pause)
				.def("isPaused", &EmitterSound::isPaused)
				.def("stop", &EmitterSound::stop)
				.def("isStopped", &EmitterSound::isStopped)
				.def("isInitial", &EmitterSound::isInitial)
				.def("isFading", &EmitterSound::isFading)
				.def("fadeIn", &EmitterSound::fadeIn)
				.def("fadeOut", &EmitterSound::fadeOut)
				.def("cancelFade", &EmitterSound::cancelFade)
				.def("setPitch", &EmitterSound::setPitch)
				.def("getPitch", &EmitterSound::getPitch)
				.def("setGain", &EmitterSound::setGain)
				.def("getGain", &EmitterSound::getGain)
				.def("setMaxGain", &EmitterSound::setMaxGain)
				.def("getMaxGain", &EmitterSound::getMaxGain)
				.def("setMinGain", &EmitterSound::setMinGain)
				.def("getMinGain", &EmitterSound::getMinGain)
				.def("getGainScale", &EmitterSound::getGainScale)
				.def("setGainScale", &EmitterSound::setGainScale)
				.def("setGainValues", &EmitterSound::setGainValues)
				.def("setMaxDistance", &EmitterSound::setMaxDistance)
				.def("getMaxDistance", &EmitterSound::getMaxDistance)
				.def("setRolloffFactor", &EmitterSound::setRolloffFactor)
				.def("setReferenceDistance", &EmitterSound::setReferenceDistance)
				.def("getReferenceDistance", &EmitterSound::getReferenceDistance)
				.def("setDistanceValues", &EmitterSound::setDistanceValues)
				//.def("setVelocity", &EmitterSound::setVelocity)
				.def("getVelocity", &EmitterSound::getVelocity)
				.def("setRelativeToListener", &EmitterSound::setRelativeToListener)
				//.def("setPosition", &EmitterSound::setPosition)
				.def("getPosition", &EmitterSound::getPosition)
				//.def("setDirection", &EmitterSound::setDirection)
				.def("getDirection", &EmitterSound::getDirection)
				.def("setOuterConeGain", &EmitterSound::setOuterConeGain)
				.def("getOuterConeGain", &EmitterSound::getOuterConeGain)
				.def("setInnerConeAngle", &EmitterSound::setInnerConeAngle)
				.def("getInnerConeAngle", &EmitterSound::getInnerConeAngle)
				.def("setOuterConeAngle", &EmitterSound::setOuterConeAngle)
				.def("getOuterConeAngle", &EmitterSound::getOuterConeAngle)
				.def("setLoop", &EmitterSound::setLoop)
				.def("isStreaming", &EmitterSound::isStreaming)
				.def("setPriority", &EmitterSound::setPriority)
				.def("getPriority", &EmitterSound::getPriority)
				.def("getSecondDuration", &EmitterSound::getSecondDuration)
				.def("setSecondOffset", &EmitterSound::setSecondOffset)
				.def("getSecondOffset", &EmitterSound::getSecondOffset)
				.def("getDerivedPosition", &EmitterSound::getDerivedPosition)
				.def("getDerivedDirection", &EmitterSound::getDerivedDirection)
				.def("getFileName", &EmitterSound::getFileName)
			];
	}
}