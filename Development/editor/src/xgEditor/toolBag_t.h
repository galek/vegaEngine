#pragma once

#include "tool_t.h"

namespace vega
{
	class toolBag_t :public std::list < tool_t* > {
		bool deleteToolWhenExist;
	public:
		toolBag_t(bool deleteToolWhenExist) :deleteToolWhenExist(deleteToolWhenExist){}

		void onViewMouseEvent(wxMouseEvent &event){
			for (iterator itr(begin()); itr != end(); ++itr){
				(*itr)->onViewMouseEvent(event);
			}
		}
		~toolBag_t(){
			if (deleteToolWhenExist){
				for (iterator itr(begin()); itr != end(); ++itr){
					delete *itr;
				}
				clear();
			}
		}
	};
}