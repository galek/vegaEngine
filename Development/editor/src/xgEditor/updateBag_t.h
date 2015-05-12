#pragma once


namespace vega
{
	class update_t;

	//##ModelId=44C8B56A0251
	class updateBag_t :public std::list < update_t* >
	{
	public:
		//##ModelId=44C8B57603B9
		void update(float t);
		~updateBag_t();
	};
}