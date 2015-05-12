#include "GamePrivate.h"

#include "Game.h"


namespace vega
{
	GameApp::GameApp(){
		SetGame(new Game());
	}
}