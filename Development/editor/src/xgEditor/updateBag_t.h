#pragma once


namespace vega
{
	class update_t;

	class updateBag_t :public std::list < update_t* >
	{
	public:
		void update(float t);
		~updateBag_t();
	};
}