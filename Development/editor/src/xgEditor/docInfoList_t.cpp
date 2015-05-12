#include "stdafx.h"
#include "docInfoList_t.h"
#include "xml.h"

namespace vega
{
	/**
	*/
	docInfoList_t::docInfoList_t(void)
	{
	}
	/**
	*/
	docInfoList_t::~docInfoList_t(void)
	{
	}
	/**
	*/
	bool docInfoList_t::loadPluginListFile(const char *fileName){
		xmlStack_t xs;
		if (XML_SUCCESS == xs.loadXMLFile(fileName)){
			xmlElement_t *root = xs.getChildByName("docList");
			if (root){
				xmlElement_t *e;
				size_t count = root->getChildrenCount();
				docInfo_t t;
				for (size_t i = 0; i < count; ++i){
					e = root->getChild(i);
					if (e->getName() == "doc"){
						e->get("desc", t.desc).
							get("dllName", t.dllName).
							get("entryFunName", t.entryFunName).
							get("name", t.name);
						push_back(t);
					}
				}
				return true;
			}
		}
		return false;
	}
	/**
	*/
}