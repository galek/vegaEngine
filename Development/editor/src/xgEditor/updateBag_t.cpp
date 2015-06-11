#include "stdafx.h"
#include "Updatable.h"
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

	void updateBag_t::AddUpdatable(Updatable*_upd)
	{
		this->push_back(_upd);
	}

}