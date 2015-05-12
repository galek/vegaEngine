#pragma once

#include <string>

#pragma warning(disable:4267)


namespace vega
{
	class xgstring :public std::string {
		//static char buf[1024];
		class wchar_t_to_char{
			char *str;
		public:
			operator const char *(){
				return str;
			}
			wchar_t_to_char(const wchar_t *charString);
			~wchar_t_to_char(){
				delete str;
			}
		};
	public:
		xgstring(const wchar_t *str){
			std::string::operator=(wchar_t_to_char(str));
		}
		xgstring(const std::wstring &str){
			std::string::operator=(wchar_t_to_char(str.c_str()));
		}
		xgstring() {
		}
		xgstring(const char *str) :std::string(str) {
		}
		xgstring(const std::string &str) :std::string(str) {
		}
		xgstring &operator =(const wchar_t *str) {
			std::string::operator=(wchar_t_to_char(str));
			return *this;
		}
		xgstring &operator =(const std::wstring &str) {
			std::string::operator=(wchar_t_to_char(str.c_str()));
			return *this;
		}
		template<class T>
		xgstring(const T &value) {
			operator<<(value);
		}
		template<class T>
		xgstring &operator=(const T &value) {
			clear();
			return operator<<(value);
		}
		template<class T>
		xgstring &operator=(T &value) {
			clear();
			return operator<<(value);
		}
		template<class T>
		xgstring &operator+(const T &value) {
			return operator<<(value);
		}
		xgstring &operator =(const char *str) {
			std::string::operator=(str);
			return *this;
		}
		xgstring &operator =(const std::string &str) {
			std::string::operator=(str);
			return *this;
		}
		xgstring &operator<<(const std::string &v);
		xgstring &operator<<(const char *v);
		xgstring &operator<<(const char &v);
		xgstring &operator<<(const int &v);
		xgstring &operator<<(const size_t &v);
		xgstring &operator<<(const long &v);
		xgstring &operator<<(const double &v);
		xgstring &operator<<(const float &v);
		xgstring &operator>>(bool &v);
		xgstring &operator>>(unsigned char &v);
		xgstring &operator>>(unsigned short &v);
		xgstring &operator>>(int &v);
		xgstring &operator>>(long &v);
		xgstring &operator>>(char &v);
		xgstring &operator>>(double &v);
		xgstring &operator>>(float &v);
		xgstring &operator>>(std::string &v);
		xgstring &operator>>(std::wstring &v);
		template<class T>
		operator const T() {
			T t;
			operator>>(t);
			return t;
		}
		operator const char *() {
			return c_str();
		}
		xgstring &format(const char* format, ...);
		const char *printf(const char* format, ...);
	};

	class xgwstring :public std::wstring {
	public:
		class chartowchar_t {
			wchar_t *str;
		public:
			operator const wchar_t *(){
				return str;
			}
			chartowchar_t(const char *charString);
			~chartowchar_t(){
				delete[]str;
			}
		};
		//static wchar_t buf[1024];
	public:
		xgwstring() {
		}
		xgwstring(const char *str){
			std::wstring::operator=(chartowchar_t(str));
		}
		xgwstring(const std::string &str){
			std::wstring::operator=(chartowchar_t(str.c_str()));
		}
		template<class T>
		xgwstring(const T &value) {
			operator<<(value);
		}
		template<class T>
		xgwstring &operator=(const T &value) {
			clear();
			return operator<<(value);
		}
		template<class T>
		xgwstring &operator=(T &value) {
			clear();
			return operator<<(value);
		}
		template<class T>
		xgwstring &operator+(const T &value) {
			return operator<<(value);
		}
		xgwstring &operator =(const char *str) {
			std::wstring::operator=(chartowchar_t(str));
			return *this;
		}
		xgwstring &operator =(const std::string &str) {
			std::wstring::operator=(chartowchar_t(str.c_str()));
			return *this;
		}
		xgwstring &operator<<(const std::string &v);
		xgwstring &operator<<(const char *v);
		xgwstring &operator<<(const wchar_t *v);
		xgwstring &operator<<(const char &v);
		xgwstring &operator<<(const int &v);
		xgwstring &operator<<(const long &v);
		xgwstring &operator<<(const double &v);
		xgwstring &operator<<(const float &v);
		xgwstring &operator>>(int &v);
		xgwstring &operator>>(long &v);
		xgwstring &operator>>(double &v);
		xgwstring &operator>>(float &v);
		xgwstring &operator>>(std::wstring &v);
		template<class T>
		operator const T() {
			T t;
			operator>>(t);
			return t;
		}
		operator const wchar_t *() {
			return c_str();
		}
		xgwstring &format(const wchar_t * format, ...);
		const wchar_t *printf(const wchar_t * format, ...);
	};

	inline xgstring &xgstring::operator>>(std::wstring &v) {
		v = (xgwstring::chartowchar_t(c_str()));
		return *this;
	}
}