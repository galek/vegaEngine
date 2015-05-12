#include "xgstring.h"
#include <stdarg.h>
#include "Windows.h"


namespace vega
{
	xgstring &xgstring::format(const char* format, ...){
		char buf[1024];
		va_list  arguments;
		va_start(arguments, format);
		vsprintf_s(buf, 1024, format, arguments);
		va_end(arguments);
		*this = buf;
		return *this;
	}
	const char *xgstring::printf(const char* format, ...){
		char buf[1024];
		va_list  arguments;
		va_start(arguments, format);
		vsprintf_s(buf, 1024, format, arguments);
		va_end(arguments);
		*this = buf;
		return *this;
	}
	xgwstring &xgwstring::format(const wchar_t* format, ...){
		wchar_t buf[1024];
		va_list  arguments;
		va_start(arguments, format);
		vswprintf(buf, 1024, format, arguments);
		va_end(arguments);
		*this = buf;
		return *this;
	}
	const wchar_t *xgwstring::printf(const wchar_t * format, ...){
		wchar_t buf[1024];
		va_list  arguments;
		va_start(arguments, format);
		vswprintf(buf, 1024, format, arguments);
		va_end(arguments);
		*this = buf;
		return *this;
	}
	xgwstring::chartowchar_t::chartowchar_t(const char *charString){
		int bufLen = int(strlen(charString) + 1);
		str = new wchar_t[bufLen * 2];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, charString, bufLen, str, bufLen * 2);
	}
	xgstring::wchar_t_to_char::wchar_t_to_char(const wchar_t *charString){
		int bufLen = int(wcslen(charString) + 1);
		str = new char[bufLen * 2];
		WideCharToMultiByte(CP_ACP, 0, charString, -1,
			str, bufLen * 2, NULL, NULL);
	}
	xgstring &xgstring::operator<<(const std::string &v) {
		*this += v;
		return *this;
	}
	xgstring &xgstring::operator<<(const char *v) {
		*this += v;
		return *this;
	}
	xgstring &xgstring::operator<<(const char &v) {
		char buf[1024];
		//*this+=_itoa_s(v,buf,1024,10);
		if (!_itoa_s(v, buf, 1024, 10)){
			*this += buf;
		}
		return *this;
	}
	xgstring &xgstring::operator<<(const int &v) {
		char buf[1024];
		//*this+=_itoa_s(v,buf,1024,10);
		if (!_itoa_s(v, buf, 1024, 10)){
			*this += buf;
		}
		return *this;
	}
	xgstring &xgstring::operator<<(const size_t &v) {
		char buf[1024];
		//*this+=_itoa_s(v,buf,1024,10);
		if (!_itoa_s(v, buf, 1024, 10)){
			*this += buf;
		}
		return *this;
	}
	xgstring &xgstring::operator<<(const long &v) {
		char buf[1024];
		//*this+=_itoa_s(v,buf,1024,10);
		if (!_itoa_s(v, buf, 1024, 10)){
			*this += buf;
		}
		return *this;
	}
	xgstring &xgstring::operator<<(const double &v) {
		char buf[1024];
		//*this+=_gcvt_s( v,1024,20,buf);
		if (!_gcvt_s(buf, 1024, v, 10)){
			*this += buf;
		}
		return *this;
	}
	xgstring &xgstring::operator<<(const float &v) {
		char buf[1024];
		//*this+=_gcvt_s( v,1024,20,buf);
		if (!_gcvt_s(buf, 1024, v, 10)){
			*this += buf;
		}
		return *this;
	}
	xgstring &xgstring::operator>>(bool &v) {
		if (*this == "true")
			v = true;
		else if (*this == "false")
			v = false;
		else
			v = !!atoi(*this);
		return *this;
	}
	xgstring &xgstring::operator>>(unsigned char &v) {
		v = (unsigned char)atoi(*this);
		return *this;
	}
	xgstring &xgstring::operator>>(unsigned short &v) {
		v = (unsigned short)atoi(*this);
		return *this;
	}
	xgstring &xgstring::operator>>(int &v) {
		v = atoi(*this);
		return *this;
	}
	xgstring &xgstring::operator>>(long &v) {
		v = atol(*this);
		return *this;
	}
	xgstring &xgstring::operator>>(char &v) {
		v = (char)atoi(*this);
		return *this;
	}
	xgstring &xgstring::operator>>(double &v) {
		v = atof(*this);
		return *this;
	}
	xgstring &xgstring::operator>>(float &v) {
		v = (float)atof(*this);
		return *this;
	}
	xgstring &xgstring::operator>>(std::string &v) {
		v = *this;
		return *this;
	}


	xgwstring &xgwstring::operator<<(const std::string &v) {
		*this += chartowchar_t(v.c_str());
		return *this;
	}
	xgwstring &xgwstring::operator<<(const char *v) {
		*this += chartowchar_t(v);
		return *this;
	}
	xgwstring &xgwstring::operator<<(const wchar_t *v) {
		*this += v;
		return *this;
	}
	xgwstring &xgwstring::operator<<(const char &v) {
		wchar_t buf[1024];
		//*this+=_itow_s(v,buf,1024,10);
		if (!_itow_s(v, buf, 1024, 10)){
			*this += buf;
		}
		return *this;
	}
	xgwstring &xgwstring::operator<<(const int &v) {
		wchar_t buf[1024];
		//*this+=_itow_s(v,buf,1024,10);
		if (!_itow_s(v, buf, 1024, 10)){
			*this += buf;
		}
		return *this;
	}
	xgwstring &xgwstring::operator<<(const long &v) {
		wchar_t buf[1024];
		//*this+=_itow_s(v,buf,1024,10);
		if (!_itow_s(v, buf, 1024, 10)){
			*this += buf;
		}
		return *this;
	}
	xgwstring &xgwstring::operator<<(const double &v) {
		wchar_t buf[1024];
		swprintf_s(buf, 1024, L"%f", v);
		*this += buf;
		return *this;
	}
	xgwstring &xgwstring::operator<<(const float &v) {
		wchar_t buf[1024];
		swprintf_s(buf, 1024, L"%f", v);
		*this += buf;
		return *this;
	}
	xgwstring &xgwstring::operator>>(int &v) {
		v = _wtoi(*this);
		return *this;
	}
	xgwstring &xgwstring::operator>>(long &v) {
		v = _wtol(*this);
		return *this;
	}
	xgwstring &xgwstring::operator>>(double &v) {
		v = _wtof(*this);
		return *this;
	}
	xgwstring &xgwstring::operator>>(float &v) {
		v = (float)_wtof(*this);
		return *this;
	}
	xgwstring &xgwstring::operator>>(std::wstring &v) {
		v = *this;
		return *this;
	}
}