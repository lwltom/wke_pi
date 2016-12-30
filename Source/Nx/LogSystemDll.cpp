#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "LogSystemDll.h"
#include <tchar.h>
#include <shlwapi.h>
using namespace std;


bool				LogSystem::isWriteFile		= true;
FunWriteLogToFile	LogSystem::m_pFunWriteLog	= nullptr;
LogSystem*			LogSystem::m_libInstance	= nullptr;

LogSystem::LogSystem()
	:m_logLevel(0)
{
	hdl = LoadLibraryA("LogSystem.dll");
	
	m_pFunWriteLog = reinterpret_cast<FunWriteLogToFile>(GetProcAddress(hdl, "WriteLogToFile"));
	QueryLogLevel();
	//IsWriteFile();
}

LogSystem::~LogSystem()
{
	this->dispose();
}

FunWriteLogToFile LogSystem::GetFuncWriteLog()
{
	return m_pFunWriteLog;
}


LogSystem* LogSystem::instance()
{
	if (LogSystem::m_libInstance == nullptr)
	{
		LogSystem::m_libInstance = new LogSystem();
	}
	return LogSystem::m_libInstance;
}

void LogSystem::dispose()
{
	FreeLibrary(hdl);
}

void LogSystem::uninstance()
{
	delete LogSystem::m_libInstance;
}

void LogSystem::WriteLogToFileError(const wchar_t* szString, bool bWrite /*= true*/)
{
	if (LogSystem::instance()->GetFuncWriteLog())
	{
		wchar_t buff[100] = { 0 };
		_stprintf_s(buff, 100, L"%s TID %d", LOG_TITLE_ERROR, GetCurrentThreadId());
		LogSystem::instance()->GetFuncWriteLog()(buff, szString, bWrite);
	}

}

void LogSystem::WriteLogToFileMsg(int nLogLevel, const wchar_t* szString, bool bWrite /*= true*/)
{
	LogSystem* pLog = LogSystem::instance();
	if (pLog->GetFuncWriteLog()
		&& ( pLog->m_logLevel >= nLogLevel) )
	{

		wchar_t buff[100] = { 0 };
		_stprintf_s(buff, 100, L"%s TID %d", LOG_TITLE_INFO, GetCurrentThreadId());
		LogSystem::instance()->GetFuncWriteLog()(buff, szString, bWrite);
	}
}

void LogSystem::WriteLogToFileMsgFormat(int nLogLevel, const TCHAR* format, ...)
{
	TCHAR *buff = (TCHAR*)0;
	va_list arglist;
	va_start(arglist, format);
	int len = _vsctprintf(format, arglist) + 1;
	buff = (TCHAR*)malloc(len * sizeof(TCHAR));
	if (buff)
	{
		memset(buff, 0, len * sizeof(TCHAR));
		_vstprintf_s(buff, len, format, arglist);
		WriteLogToFileMsg(nLogLevel, buff, true);
		free(buff);
	}
	va_end(arglist);
}

void LogSystem::WriteLogToFileErrorFormat(const TCHAR* format, ...)
{
	TCHAR *buff = (TCHAR*)0;
	va_list arglist;
	va_start(arglist, format);
	int len = _vsctprintf(format, arglist) + 1;
	buff = (TCHAR*)malloc(len * sizeof(TCHAR));
	if (buff)
	{
		memset(buff, 0, len * sizeof(TCHAR));
		_vstprintf_s(buff, len, format, arglist);
		WriteLogToFileError(buff, true);
		free(buff);
	}
	va_end(arglist);
}

void LogSystem::QueryLogLevel()
{
	//TODO: 从配置文件获取log级别, 文件路径:..\Debug\NCBrowser.debug
	/*CPathLight path;
	path = path.GetSelfModuleFolder();
	path = path.GetParentPathFromPath();
	path += _T("Debug\\NCBrowser.debug");*/
	wstring strPath;
	{
		TCHAR szFilePath[MAX_PATH + 1];
		GetModuleFileName(NULL, szFilePath, MAX_PATH);
		(_tcsrchr(szFilePath, _T('\\')))[1] = 0; //删除文件名，只获得路径
		//strPath = szFilePath;

		//去掉结尾斜杠
		::PathRemoveBackslash(szFilePath);
		::PathRemoveFileSpec(szFilePath);

		wstring strTemp(szFilePath);

		strTemp += _T("\\");
		strTemp += _T("Debug\\NCBrowser.debug");
		strPath = strTemp;
	}
	m_logLevel = PathFileExists(strPath.c_str()) == TRUE ? LOG_ALL : m_logLevel;

	//m_logLevel = path.IsFileExist() ? LOG_ALL : m_logLevel;
}

/*
void LogSystem::IsWriteFile()
{
	CDuiString path= CPaintManagerUI::GetInstancePath()+_T("writelog");
	fstream _file;
	_file.open(LPCTSTRToString(path.GetData()), ios::in);
	if (!_file)
		isWriteFile = false;
	else
		isWriteFile = true;
}*/