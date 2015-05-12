#pragma once

#include "xgstring.h"
#include "alarm_t.h"
#include <iostream>
#include <fstream>
#include <vector>
#define XML_FILE_NOT_FOUND  0x0001
#define XML_SUCCESS         0x0002   
#define XML_FAILED          0x0004
#define XML_READY           0x0008
#define XML_MATCH           0x0010
#define XML_BUFFER_OVERFLOW 0x0020
#pragma warning(disable:4251)


namespace vega
{
	class rawData_t {
	public:
		bool  skipDelete;
		char *data;
		int   byteCount;

		rawData_t();
		rawData_t(const rawData_t &me);
		rawData_t &operator=(const rawData_t &me);
		~rawData_t();
		void destroy();
	};

	class xmlElement_t;

	class xmlTree_t {
	protected:
		std::vector<xmlElement_t*> children;
	public:
		virtual ~xmlTree_t();
		virtual bool save(std::ofstream &o, int &tabCount);
		void        addChild(xmlElement_t *child);
		xmlElement_t *getChildByName(const char *name);
		xmlElement_t *getChild(size_t index);
		xmlElement_t *getChildByName(const char *name)const;
		xmlElement_t *getChild(size_t index)const;
		size_t      getChildrenCount();
		void        flush();
		void        print();
	};

	class xmlElement_t : public xmlTree_t, public xgstring {
		xgstring name;
	public:
		xmlElement_t(const std::string &name, const xgstring &val) :name(name){ setValue(val); }
		xmlElement_t(const char *name, const xgstring &val) :name(name){ setValue(val); }
		xmlElement_t();
		~xmlElement_t();

		xmlElement_t(const xmlElement_t &copy);
		xmlElement_t &operator =(const xmlElement_t &copy);
		void   setName(const char   *nameArg);
		void   setName(const xgstring &name);

		bool save(std::ofstream &o, int &tabCount);
		const xgstring &getName()     const;
		void setValue(const xgstring &val);
		const xgstring &getValue() const;
		void print();
		void flush();
		template<class T>
		xmlElement_t &get(const char *name, T &value, bool mustExist = true){
			if (mustExist){
				//Nick:DELME				alarm(!getChildByName(name), (string("the must exist xml elment child: ")
				//Nick:DELME		+ name + " doest'st exist at the xml elment" + name).c_str());
				//Nick:DELME	(*getChildByName(name)) >> (value);
			}
			else if (getChildByName(name)){
				(*getChildByName(name)) >> (value);
			}
			return *this;
		}
		template<class T>
		xmlElement_t &get(size_t index, T &value, bool mustExist = true){
			if (mustExist){
				alarm(!getChild(index), (xgstring("the must exist xml elment child: ")
					<< index << " doest'st exist at the xml elment" + index).c_str());
				(*getChild(index)) >> (value);
			}
			else if (getChild(index)){
				(*getChild(index)) >> (value);
			}
			return *this;
		}
		template<class T>
		const xmlElement_t &set(const char *name, const T &value, bool mustExist = true)const{
			if (mustExist){
				alarm(!getChildByName(name), (string("the must exist xml elment child: ")
					+ name + " doest'st exist at the xml elment" + name).c_str());
				(*(xgstring*)getChildByName(name)) = (value);
			}
			else if (getChildByName(name)){
				(*(xgstring*)getChildByName(name)) = (value);
			}
			return *this;
		}
		template<class T>
		const xmlElement_t &set(size_t index, const T &value, bool mustExist = true)const{
			if (mustExist){
				alarm(!getChild(index), (xgstring("the must exist xml elment child: ")
					<< index << " doest'st exist at the xml elment" + index).c_str());
				(*(xgstring*)getChild(index)) = (value);
			}
			else if (getChild(index)){
				(*(xgstring*)getChild(index)) = (value);
			}
			return *this;
		}
		rawData_t rawData;
	};

	class xmlStack_t : public xmlTree_t {
	public:
		xmlStack_t(const char *xmlFilePath = NULL, bool loggerOn = true);
		xmlStack_t(const xmlStack_t &copy);
		xmlStack_t(char *buf, size_t size);
		~xmlStack_t();
		xmlStack_t &operator =(const xmlStack_t &copy);

		void   print();
		void   flush();
		bool parseMem(char *buf, size_t size);
		int    loadXMLFile(const char *xmlFilePath);
		bool	saveXMLFile(const char *xmlFilePath);
	private:
		void   errorInfo(const char* string);

		void   getStreamedValue(char **stream, xgstring&);
		void   getStringValue(char **stream, xgstring&);
		void   getIdentifier(char **stream, xgstring&);

		bool   moveAndCheckProgress(int jump = 1);
		bool   eatWhiteSpaces(char **stream);
		bool   eatXMLHeader(char **stream);
		bool   eatComment(char **stream);
		bool   fillRawData(char **stream, rawData_t *dataStruct, int count = -1);
		char*  parseXMLStream(char *stream, xmlElement_t *parent);
		int    getRemainingBytes();

		xgstring encoding,
			logFilePath;
		float  XMLVersion;
		bool   loggerOn;
		int    bufferProgress,
			bufferSize,
			state;
	};
}