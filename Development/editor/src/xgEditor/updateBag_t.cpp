#include "stdafx.h"
#include "update_t.h"
#include "updateBag_t.h"



namespace vega
{
	void updateBag_t::update(float t)
	{
		for (iterator itr(begin()); itr != end(); ++itr)
			(*itr)->update(t);
		// TODO: Add your specialized code here.
	}

	updateBag_t::~updateBag_t(){
		/*
			int i=size();
			for(iterator itr(begin());itr!=end();)
			itr=erase(itr);*/

	}
}