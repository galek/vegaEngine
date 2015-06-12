#include "EditorPublicPCH.h"
#include <assert.h>
#include "xml.h"
#include <stdarg.h>
#include "alarm_t.h"


namespace vega
{
#define has_alarm 1
	static char nullChar[1];

	rawData_t::rawData_t() {
		data = NULL;
		byteCount = 0;
		skipDelete = false;
	}

	rawData_t::rawData_t(const rawData_t &me) {
		data = NULL;
		byteCount = 0;
		skipDelete = false;
		this->operator =(me);
	}

	rawData_t &rawData_t::operator=(const rawData_t &me) {
		if (this != &me) {
			destroy();
			byteCount = me.byteCount;
			skipDelete = false;

			if (byteCount > 0 && me.data) {
				data = new char[byteCount];
				memcpy(data, me.data, byteCount);
			}
		}
		return *this;
	}

	rawData_t::~rawData_t() {
		destroy();
	}

	void rawData_t::destroy() {
		if (data && !skipDelete) {
			if (data){
				delete[] data;
				data = NULL;
			}
			byteCount = 0;
		}
	}
	bool xmlTree_t::save(std::ofstream &o, int &tabCount){
		for (std::vector<xmlElement_t*>::const_iterator itr(children.begin()); itr != children.end(); ++itr){
			/*for(int i=0;i<tabCount;++i)
			o<<'\t';*/
			if (!(*itr)->save(o, tabCount))
				return false;
		}
		return true;
	}
	xmlTree_t::~xmlTree_t() {
		flush();
	}
	void xmlElement_t::setName(const char   *nameArg) {
		name = nameArg;
	}

	void xmlElement_t::setName(const xgstring &nameArg) {
		name = nameArg;
	}

	const xgstring &xmlElement_t::getName() const {
		return name;
	}
	xmlElement_t *xmlTree_t::getChildByName(const char *name) {
		if (!name) return NULL;

		for (size_t i = 0; i < children.size(); i++)
			if (children[i]->getName() == name)
				return  children[i];
		return NULL;
	}

	xmlElement_t *xmlTree_t::getChild(size_t index) {
		return index >= children.size() ? NULL : children[index];
	}
	xmlElement_t *xmlTree_t::getChildByName(const char *name) const{
		if (!name) return NULL;

		for (size_t i = 0; i < children.size(); i++)
			if (children[i]->getName() == name)
				return  children[i];
		return NULL;
	}

	xmlElement_t *xmlTree_t::getChild(size_t index) const{
		return index >= children.size() ? NULL : children[index];
	}
	size_t xmlTree_t::getChildrenCount() {
		return children.size();
	}

	void xmlTree_t::flush() {
		for (size_t t = 0; t < children.size(); t++)
			if (children[t]){
			delete   children[t];
			children[t] = NULL;
			}
		children.clear();
	}

	void xmlTree_t::print() {
		for (size_t t = 0; t < children.size(); t++)
			children[t]->print();
	}

	void xmlTree_t::addChild(xmlElement_t *child) {
		if (!child) return;
		children.push_back(child);
	}

	xmlElement_t::xmlElement_t() {
		xgstring::operator =("");
		name = "";
	};

	bool xmlElement_t::save(std::ofstream &o, int &tabCount){
		bool f = name.length();
		if (f){
			for (int i = 0; i < tabCount; ++i)
				o << '\t';
			o << '<' << name.c_str() << '>';
		}
		if (!children.size())
			o << xgstring::c_str();
		else {
			o << '\n';
			++tabCount;
		}
		if (!xmlTree_t::save(o, tabCount))
			return false;
		if (children.size())
			--tabCount;
		if (f){
			if (children.size())
				for (int i = 0; i < tabCount; ++i)
					o << '\t';
			o << "</" << name.c_str() << ">\n";
		}
		return true;
	}
	xmlElement_t::xmlElement_t(const xmlElement_t &copy) {
		this->operator=(copy);
	}

	xmlElement_t &xmlElement_t::operator =(const xmlElement_t &copy) {
		if (this != &copy) {
			flush();
			for (size_t i = 0; i < copy.children.size(); i++) {
				xmlElement_t *elem = new xmlElement_t();
				*elem = *copy.children[i];
				children.push_back(elem);
			}

			rawData = copy.rawData;
			xgstring::operator =(copy);
			name = copy.name;
		}
		return *this;
	}

	void xmlElement_t::setValue(const xgstring &val) {
		bool valid = 0;
		for (size_t i = 0; i < val.length(); i++)
			if (val[i] != ' '  &&
				val[i] != '\t' &&
				val[i] != '\n') {
			valid = true;
			break;
			}
		if (valid)
			xgstring::operator=(val);
	}
	void xmlElement_t::print() {
		std::cout << "Name: " << name
			<< ", Value: " << *this << '\n';
		xmlTree_t::print();
	}

	xmlElement_t::~xmlElement_t() {
		flush();
	}

	void xmlElement_t::flush() {
		xmlTree_t::flush();
		rawData.destroy();
	}

	xmlStack_t::xmlStack_t(const char *xmlFilePath, bool isOn) {
		XMLVersion = 1.0;
		loggerOn = isOn;
		encoding = "ISO-8859-1";
		state = XML_READY;
		if (xmlFilePath)
			state = loadXMLFile(xmlFilePath);
		else
			state = XML_FILE_NOT_FOUND;
	};
	xmlStack_t::xmlStack_t(char *buf, size_t size){
		if (!parseMem(buf, size)){
			if (has_alarm){
//Nick:DELME				alarm(true, "parse mem failed");
			}
			else{
				assert("parse mem failed");
			}

		}
	}
	xmlStack_t::xmlStack_t(const xmlStack_t &copy) {
		this->operator=(copy);
	}

	xmlStack_t &xmlStack_t::operator =(const xmlStack_t &copy) {
		if (this != &copy) {
			flush();
			for (size_t i = 0; i < copy.children.size(); i++) {
				xmlElement_t *element = new xmlElement_t();
				*element = *copy.children[i];
				addChild(element);
			}
			XMLVersion = copy.XMLVersion;
			encoding = copy.encoding;
			loggerOn = copy.loggerOn;
			state = copy.state;
		}
		return *this;
	}

	xmlStack_t::~xmlStack_t() {
		flush();
	}

	void xmlStack_t::print() {
		std::cout << "Version: " << XMLVersion
			<< ", Encoding: " << encoding << std::endl;
		xmlTree_t::print();
	}

	bool xmlStack_t::saveXMLFile(const char *xmlFilePath){
		if (!xmlFilePath)
			return false;
		std::ofstream fileOutputStream(xmlFilePath, std::ifstream::out | std::ifstream::binary);
		fileOutputStream << "<?xml version=\"1.0\" encoding=\"GB2312\" standalone=\"yes\" ?>\n";
		int tabCount(0);
		return save(fileOutputStream, tabCount);
	}
	bool xmlStack_t::parseMem(char *buf, size_t size){
		if (!buf || !size)
			return false;
		char       *progress = NULL,
			*stream = NULL;
		bufferProgress = 0;
		bufferSize = 0;
		flush();
		state = XML_SUCCESS;
		bufferSize = size;
		stream = buf;
		progress = stream;

		do {
			xmlElement_t *parent = new xmlElement_t();
			progress = parseXMLStream(progress, parent);
			addChild(parent);
		} while (*progress && (state == XML_SUCCESS));

		if (state != XML_SUCCESS) {
			flush();
			if (has_alarm){
				//Nick:DELME				alarm(true, "parse xml from me failed");
			}
			else{
				assert("parse xml from me failed");
			}

			return false;
		}
		return true;
	}
	int xmlStack_t::loadXMLFile(const char *xmlFilePath) {
		const char *verifiedPath = xmlFilePath;
		size_t      pathLength = 0;
		char       *progress = NULL,
			*stream = NULL;

		bufferProgress = 0;
		bufferSize = 0;

		flush();
		if (!verifiedPath)
			return(state = XML_FILE_NOT_FOUND);

		pathLength = strlen(verifiedPath);

		for (size_t t = 0; t < pathLength; t++) {
			if (verifiedPath[t] == '.')
				break;
			logFilePath += verifiedPath[t];
		}

		logFilePath += ".err";

		state = XML_SUCCESS;
		FILE *fileInput = fopen(verifiedPath, "rb");
		if (fileInput){
			fseek(fileInput, 0, SEEK_END);
			bufferSize = ftell(fileInput);
			fseek(fileInput, 0, SEEK_SET);
			if (bufferSize){
				stream = new char[bufferSize + 1];
				progress = stream;
				memset(stream, NULL, bufferSize + 1);
				fread(stream, 1, bufferSize, fileInput);
				fclose(fileInput);
			}
			else{
				fclose(fileInput);
				return XML_FAILED;
			}
		}
		else{
			xgstring des("open xml file \"");
			des << verifiedPath << "\" failed\n";
			if (has_alarm){
				//Nick:DELME				alarm(true, des);
			}
			else{
				assert("open xml file failed");
			}
		}
		do {
			xmlElement_t *parent = new xmlElement_t();
			progress = parseXMLStream(progress, parent);
			addChild(parent);
		} while (*progress && (state == XML_SUCCESS));
		if (stream){
			delete[] stream;
			stream = NULL;
		}
		if (state != XML_SUCCESS) {
			flush();
			return state;
		}
		return XML_SUCCESS;
	}
	void xmlStack_t::errorInfo(const char* string) {
		if (has_alarm){
			//Nick:DELME			alarm(true, string);
		}
		else{
			assert("an orror occur when parse file");
		}
		state = XML_FAILED;
	}
	void xmlStack_t::flush() {
		xmlTree_t::flush();
		state = XML_READY;
	}

	bool xmlStack_t::moveAndCheckProgress(int jump) {
		if ((bufferProgress += jump) > bufferSize) {
			state = XML_BUFFER_OVERFLOW;
			return false;
		}

		return true;
	}
	template <class T>
	static inline T clamp(T x, T min, T max) {
		return(x < min) ? min : (x > max) ? max : x;
	}
	int  xmlStack_t::getRemainingBytes() {
		return clamp(bufferSize - bufferProgress, 0, bufferSize);
	}

	bool xmlStack_t::eatWhiteSpaces(char **stream) {
		while ((**stream) != NULL &&
			(**stream == ' ' || **stream == '\n' ||
			**stream == '\t' || **stream == '\r')) {
			(*stream)++;
			if (!moveAndCheckProgress()) return false;
		}
		return(**stream) != NULL;
	}

	bool xmlStack_t::eatComment(char **stream) {
		eatWhiteSpaces(stream);
		if (*(*stream + 0) == '<' && *(*stream + 1) == '!' &&
			*(*stream + 2) == '-' && *(*stream + 3) == '-') {
			(*stream) = strstr((*stream) += 4, "-->") + 3;
			eatWhiteSpaces(stream);
			return true;
		}
		return false;
	}

	void xmlStack_t::getIdentifier(char **stream, xgstring& identifier) {
		int    counter = 0;
		eatWhiteSpaces(stream);

		while ((**stream >= 'A'    &&
			**stream <= 'Z') ||

			**stream == '_' ||
			**stream == '.' ||
			**stream == ':' ||
			(**stream >= 'a'   &&
			**stream <= 'z') ||

			(**stream >= '0'   &&
			**stream <= '9')) {
			counter++;
			(*stream)++;
			if (!moveAndCheckProgress()) return;
		}

		if (counter)
			identifier.assign((*stream) - counter, counter);
		eatWhiteSpaces(stream);
	}

	void xmlStack_t::getStreamedValue(char **stream, xgstring& streamedVals) {
		int    counter = 0;

		if (!eatWhiteSpaces(stream)) return;

		while (**stream != '<' && **stream != NULL) {
			counter++;
			(*stream)++;
			if (!moveAndCheckProgress()) return;
		}

		if (counter)
			streamedVals.assign((*stream) - counter, counter);
		else
			streamedVals.clear();
		if (**stream == NULL)
			state = XML_FAILED;
		else
			eatWhiteSpaces(stream);
	}

	bool xmlStack_t::fillRawData(char **stream, rawData_t *dataStruct, int bytesCount) {
		if (!dataStruct || !(*stream))
			return false;

		int  charCounter = 0;
		char *subStream = bytesCount > 0 ? new char[bytesCount + 1] : (*stream);

		if (bytesCount > 0) {
			if (getRemainingBytes() <= bytesCount) {
				state = XML_BUFFER_OVERFLOW;
				return false;
			}
			dataStruct->byteCount = bytesCount;
			memset(subStream, NULL, bytesCount + 1);
			memcpy(subStream, (*stream), bytesCount);
			(*stream) += bytesCount;
			moveAndCheckProgress(bytesCount);
		}
		else {
			while (!(*(subStream + 0) == '<' && *(subStream + 1) == '/' &&
				*(subStream + 2) == 'R' && *(subStream + 3) == 'a' &&
				*(subStream + 4) == 'w' && *(subStream + 5) == 'D' &&
				*(subStream + 6) == 'a' && *(subStream + 7) == 't' &&
				*(subStream + 8) == 'a')) {
				charCounter++; subStream++;
				if (!moveAndCheckProgress())
					return false;
			}

			if (!charCounter)
				return false;

			subStream = new char[charCounter + 1];
			memset(subStream, NULL, charCounter + 1);
			memcpy(subStream, (*stream), charCounter);
			(*stream) += charCounter;
			dataStruct->byteCount = charCounter;
		}
		dataStruct->data = subStream;
		return true;
	}

	void xmlStack_t::getStringValue(char **stream, xgstring &value) {
		int    counter = 0;

		if (!eatWhiteSpaces(stream)) return;
		(*stream)++;
		if (!moveAndCheckProgress())     return;
		if (!eatWhiteSpaces(stream)) return;
		(*stream)++;
		if (!moveAndCheckProgress())     return;

		while (**stream != '\"' && **stream != NULL) {
			counter++; (*stream)++;
			if (!moveAndCheckProgress())   return;
		}

		if (counter)
			value.assign((*stream) - counter, counter);
		if (**stream == NULL)
			state = XML_FAILED;
		else {
			(*stream)++;           //Get rid of the second quote sign
			if (!moveAndCheckProgress())     return;
			if (!eatWhiteSpaces(stream)) return;
		}
	}

	bool xmlStack_t::eatXMLHeader(char **stream) {
		//int   check = 0;
		xgstring token, value;

		if (*(*stream + 0) == '<' && *(*stream + 1) == '?' &&
			*(*stream + 2) == 'x' && *(*stream + 3) == 'm' &&
			*(*stream + 4) == 'l') {
			(*stream) += 5;
			if (!moveAndCheckProgress(5))  return false;

			while (**stream != '?') {
				getIdentifier(stream, token);
				getStringValue(stream, value);

				if ((const xgstring &)token == "version")
					XMLVersion = float(atof(value));
				if ((const xgstring &)token == "encoding")
					encoding = value;
			}
		}
		else
			return false;
		(*stream) += 2;
		eatWhiteSpaces(stream);
		return true;
	}

	char* xmlStack_t::parseXMLStream(char *stream, xmlElement_t *parent) {
		if (!stream || !parent)
			return nullChar;

		xgstring token;

		if (state == XML_FAILED || state == XML_BUFFER_OVERFLOW)
			return nullChar;

		if (!eatWhiteSpaces(&stream))
			return nullChar;

		if (*stream == '<' &&
			(!eatComment(&stream) &&
			!eatXMLHeader(&stream))) {
			stream++;
			if (!moveAndCheckProgress())  return nullChar;
			getIdentifier(&stream, token);
			parent->setName(token);

			if (state == XML_FAILED || state == XML_BUFFER_OVERFLOW)
				return nullChar;

			if (*stream == '>') {
				stream++;
				if (!moveAndCheckProgress())  return nullChar;
			}
			else {
				while ((*stream != '/' && *(stream + 1) != '>') &&
					(*stream != '>')) {
					xmlElement_t *newTag = new xmlElement_t();
					getIdentifier(&stream, token);
					newTag->setName(token);

					if (state == XML_FAILED || state == XML_BUFFER_OVERFLOW)
						return nullChar;

					getStringValue(&stream, token);
					newTag->setValue(token);

					if (state == XML_FAILED || state == XML_BUFFER_OVERFLOW)
						return nullChar;
					parent->addChild(newTag);
				}

				if (*stream == '/' && *(stream + 1) == '>') {
					stream += 2;
					if (!moveAndCheckProgress(2))  return nullChar;
					return stream;
				}
				stream++;
			}

			if (parent->getName() != "rawData_t") {
				getStreamedValue(&stream, token);
				parent->setValue(token);
			}
			else {
				xmlElement_t *info = parent->getChildByName("length");
				bool       sucess = (!info) ? fillRawData(&stream, &parent->rawData) :
					fillRawData(&stream, &parent->rawData,
					(const int)(*info));
				if (!sucess)
					errorInfo("\n<!>Failed to load raw data, check your XML file.\n");
				if (state == XML_FAILED || state == XML_BUFFER_OVERFLOW)
					return nullChar;
			}

			if (!eatWhiteSpaces(&stream))
				return nullChar;

			eatComment(&stream);

			while (*stream == '<' && *(stream + 1) != '/') {
				xmlElement_t *newTag = new xmlElement_t();
				stream = parseXMLStream(stream, newTag);

				if (!stream || !eatWhiteSpaces(&stream))
					return nullChar;
				parent->addChild(newTag);
			}

			if (*stream == '<' && *(stream + 1) == '/') {
				stream += 2;
				if (!moveAndCheckProgress(2))  return nullChar;

				getIdentifier(&stream, token);
				if (state == XML_FAILED || state == XML_BUFFER_OVERFLOW)
					return nullChar;
			}

			if (token != parent->getName() || *stream != '>') {
				errorInfo(xgstring("\n<!>Start tag <") + parent->getName() + "> & End Tag <" + token + "> do not match,"\
					"check your XML file.\n");
				state = XML_FAILED;
				return nullChar;
			}
			stream++;
			if (!eatWhiteSpaces(&stream)) return nullChar;

		}
		return(state == XML_SUCCESS) ? stream : nullChar;
	}
}