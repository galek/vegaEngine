#pragma once

namespace vega
{
	class GAME_API Game :public iGame
	{
	public:
		Game();
		~Game();
		virtual void PreInitialize();
		virtual void CreateScene();
		virtual void Update(float _time);
	private:
		void _enableStohMusic();
	private:
		class EmitterSound*sound;
		class VideoPlayer*vplayer;
	};
};