#pragma once

namespace vega
{
	class Updatable;

	class updateBag_t :public std::list < Updatable* >
	{
	public:
		void AddUpdatable(Updatable*);
		void update(float t);
		~updateBag_t();
	};
}