#pragma once
#ifndef GAME_H
#define GAME_H


namespace vega
{
	class GAME_API Game:public iGame
	{
	public:
		Game();
		~Game();
		virtual void			Initialize();
		virtual void			CreateScene();
		virtual void			Update(float _time);
	private:
		void _enableStohMusic();
	private:
		class EmitterSound*sound;
		class VideoPlayer*vplayer;
	};
};
#endif