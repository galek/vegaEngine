#include "EditorPublicPCH.h"
#include "alarm_t.h"
#include <windows.h>
#include "resource.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <tchar.h>



namespace vega
{
#if 0
	using namespace std;
	alarm_t& alarm_t::get(){
		static alarm_t alarm_t_;
		return alarm_t_;
	}
	alarm_t::alarm_t(void) {}
	alarm_t::~alarm_t(void) {}
	static int retCode;
#ifdef _UNICODE
	static wstring descriptionBuf;
#else
	static string descriptionBuf;
#endif
#ifdef _DEBUG
	const wchar_t *moduleName = L"xgPublicd.dll";
#else
	const wchar_t *moduleName = L"xgPublic.dll";
#endif
	void alarm_t::alarming(bool value, const char* format, ...){
		char buf[1024];
		va_list  arguments;
		va_start(arguments, format);
		vsprintf_s(buf, 1024, format, arguments);
		va_end(arguments);
		if (alarming(value, buf, line, file, ignoreAlways)){
			_asm { int 3 }
		}
	}
	void alarm_t::alarming(bool value, const wchar_t* format, ...){
		wchar_t buf[1024];
		va_list  arguments;
		va_start(arguments, format);
		vswprintf(buf, 1024, format, arguments);
		va_end(arguments);
		if (alarming(value, buf, line, file, ignoreAlways)){
			_asm { int 3 }
		}
	}
	void alarm_t::alarming(bool value, const std::string& desc){
		if (alarming(value, desc, line, file, ignoreAlways)){
			_asm { int 3 }
		}
	}
	static BOOL APIENTRY AlarmProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam);
	bool alarm_t::alarming(bool value, const string& description, int line, const char* file, bool* ignoreAlways){
		return alarming(value, description.c_str(), line, file, ignoreAlways);
	}
	bool alarm_t::alarming(bool value, const wchar_t* description, int line, const char* file, bool* ignoreAlways) {
		if (value){
			/*descriptionBuf.clear();
			descriptionBuf+=_TEXT("file:\015\012\t");
			descriptionBuf+=file;
			descriptionBuf+=_TEXT("\015\012line:\015\012\t");
			wchar_t r[256];
			descriptionBuf+=_itow(line,r,10);
			descriptionBuf+=_TEXT("\015\012description:\015\012\t");
			descriptionBuf+=description;
			DialogBox(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_ALARM_OPTION),NULL,AlarmProc);
			if(retCode==IDC_IGNORE_ALWAYS)*ignoreAlways=true;
			if(retCode==IDC_ABORT)exit(0);
			return retCode==IDC_INTERRUPT;*/
			descriptionBuf.clear();
			descriptionBuf += _TEXT("file:\t");
			//descriptionBuf+=file;

			descriptionBuf += _TEXT("file:\t");
			int fileBufferLen = int(strlen(file) + 1);
			wchar_t *fileBuffer = new wchar_t[(fileBufferLen)* 2];
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, file, fileBufferLen, fileBuffer, (fileBufferLen)* 2);
			descriptionBuf += fileBuffer;

			descriptionBuf += _TEXT("\tline:\t");
			wchar_t r[256];
			if (_itow_s(line, r, 256, 10))
				descriptionBuf += r;
			descriptionBuf += _TEXT("\015\012description:\t");
			descriptionBuf += description;
			descriptionBuf += _TEXT("\015\012");
			DialogBox(GetModuleHandle(moduleName), MAKEINTRESOURCE(IDD_ALARM_OPTION), NULL, AlarmProc);
			delete fileBuffer;
			if (retCode == IDC_IGNORE_ALWAYS)*ignoreAlways = true;
			if (retCode == IDC_ABORT)exit(0);
			return retCode == IDC_INTERRUPT;
		}
		else return false;
	}
	bool alarm_t::alarming(bool value, const char* description, int line, const char* file, bool* ignoreAlways){
		if (value){
			descriptionBuf.clear();
			/*descriptionBuf+=_TEXT("file:\015\012\t");
			int fileBufferLen=int(strlen(file)+1);
			wchar_t *fileBuffer=new wchar_t[(fileBufferLen)*2];
			MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,file,fileBufferLen,fileBuffer,(fileBufferLen)*2);
			descriptionBuf+=fileBuffer;
			descriptionBuf+=_TEXT("\015\012line:\015\012\t");
			wchar_t r[256];
			descriptionBuf+=_itow(line,r,10);
			descriptionBuf+=_TEXT("\015\012description:\015\012\t");*/

			descriptionBuf += _TEXT("file:\t");
			int fileBufferLen = int(strlen(file) + 1);
			wchar_t *fileBuffer = new wchar_t[(fileBufferLen)* 2];
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, file, fileBufferLen, fileBuffer, (fileBufferLen)* 2);
			descriptionBuf += fileBuffer;
			descriptionBuf += _TEXT("\t");
			descriptionBuf += _TEXT("line:\t");
			wchar_t r[256];
			//descriptionBuf+=_itow(line,r,10);
			if (_itow_s(line, r, 256, 10))
				descriptionBuf += r;
			descriptionBuf += _TEXT("\015\012description:\t");

			int descriptionBufferLen = int(strlen(description) + 1);
			wchar_t *descriptionBuffer = new wchar_t[(descriptionBufferLen)* 2];
			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, description, descriptionBufferLen, descriptionBuffer, (descriptionBufferLen)* 2);
			descriptionBuf += descriptionBuffer;
			descriptionBuf += _TEXT("\015\012");
			DialogBox(GetModuleHandle(moduleName), MAKEINTRESOURCE(IDD_ALARM_OPTION), NULL, AlarmProc);
			delete fileBuffer;
			delete descriptionBuffer;
			if (retCode == IDC_IGNORE_ALWAYS)*ignoreAlways = true;
			if (retCode == IDC_ABORT)exit(0);
			return retCode == IDC_INTERRUPT;
		}
		else return false;
	}
	static HWND hwndMainDialog;
	BOOL APIENTRY AlarmProc(HWND hwnd, UINT msg, UINT wParam, LONG lParam) {
		lParam = lParam;//disable warning
		switch (msg) {
		case WM_INITDIALOG:
			SetWindowTextW(GetDlgItem(hwnd, IDC_ALARM_TEXT), descriptionBuf.c_str());
			hwndMainDialog = hwnd;
			break;
		case WM_COMMAND:
		{
			switch (LOWORD(wParam)) {
			case IDC_ABORT:
				retCode = IDC_ABORT;
				EndDialog(hwnd, FALSE);
				break;
			case IDC_IGNORE:
				retCode = IDC_IGNORE;
				EndDialog(hwnd, FALSE);
				break;
			case IDC_IGNORE_ALWAYS:
				retCode = IDC_IGNORE_ALWAYS;
				EndDialog(hwnd, FALSE);
				break;
			case IDC_COPY:
				if (OpenClipboard(NULL)) {
					HGLOBAL hMem;
					wchar_t *pMem;
					size_t len = wcslen(descriptionBuf.c_str()) * 3;
					hMem = GlobalAlloc(GHND | GMEM_DDESHARE, len);

					if (hMem) {
						pMem = (wchar_t*)GlobalLock(hMem);
						_tcscpy_s(pMem, len, descriptionBuf.c_str());
						GlobalUnlock(hMem);
						EmptyClipboard();
						SetClipboardData(CF_TEXT, hMem);
					}

					CloseClipboard();
				}
				retCode = IDC_COPY;
				break;
			case IDC_INTERRUPT:
				retCode = IDC_INTERRUPT;
				EndDialog(hwnd, FALSE);
				break;
			case IDCANCEL:
				retCode = IDC_IGNORE;
				EndDialog(hwnd, FALSE);
				break;
			default: break;
			}
		}
		default:
			break;
		}
		return FALSE;
	}
	/*
	LPVOID lpMsgBuf;
	if (!FormatMessage(
	FORMAT_MESSAGE_ALLOCATE_BUFFER |
	FORMAT_MESSAGE_FROM_SYSTEM |
	FORMAT_MESSAGE_IGNORE_INSERTS,
	NULL,
	GetLastError(),
	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
	(LPTSTR) &lpMsgBuf,
	0,
	NULL ))
	{
	// Handle the error.
	return;
	}

	// Process any inserts in lpMsgBuf.
	// ...

	// Display the string.
	MessageBox( NULL, (LPCTSTR)lpMsgBuf, L"Error", MB_OK | MB_ICONINFORMATION );

	// Free the buffer.
	LocalFree( lpMsgBuf );
	*/
	bool alarm_t::getIgnoreAlways(){
		return *ignoreAlways;
	}
	void alarm_t::set(int line, const char* file, bool* ignoreAlways){
		this->line = line;
		this->file = file;
		this->ignoreAlways = ignoreAlways;
	}
#endif
}