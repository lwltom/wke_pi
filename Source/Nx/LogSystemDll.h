#pragma once

#include <Windows.h>
#define LOG_TITLE_INFO					L"NCWKE_INFO"
#define LOG_TITLE_ERROR					L"NCWKE_ERROR"
#define LOG_TITLE_WARN					L"NCWKE_WARN"

extern "C" typedef  void(WINAPI *FunWriteLogToFile)(const wchar_t *title, const wchar_t *content, const bool IsWriteFile);
class LogSystem
{
public:
	enum ENUM_LOG_LEVEL 
	{
		LOG_NORMAL = 0, LOG_ALL
	};
	FunWriteLogToFile GetFuncWriteLog();
	static LogSystem* instance();
	static void uninstance();
	static void WriteLogToFileMsg(int nLogLevel, const wchar_t* szString, bool bWrite = true);
	static void WriteLogToFileMsgFormat(int nLogLevel, const TCHAR* format, ...);
	static void WriteLogToFileWarn(const wchar_t* szString, bool bWrite = true);
	static void WriteLogToFileError(const wchar_t* szString, bool bWrite = true);
	static void WriteLogToFileErrorFormat(const TCHAR* format, ...);
	void dispose();
	static bool isWriteFile;

protected:

	LogSystem();
	virtual ~LogSystem();
	void QueryLogLevel();
	//void IsWriteFile();
private:

	HMODULE hdl;
	static FunWriteLogToFile		m_pFunWriteLog;
	static LogSystem* m_libInstance;
	int								m_logLevel;		//日志级别， >= 改值才记录日志
	/************************************************************************
			log level:
				0:	//log important info,  default.
				1:	//log all info
	************************************************************************/
};
