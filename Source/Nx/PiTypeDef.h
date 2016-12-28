#pragma once
#include <string>
#include <tchar.h>
#include <list>
#include <map>
#include <wchar.h>
#include <utility>
#include <algorithm>
#include <vector>
#include <sstream>

using std::string;
using std::wstring;
using std::list;
using std::map;
using std::pair;
using std::vector;
using std::stringstream;
using std::wstringstream;
using std::ostringstream;
using std::wostringstream;

typedef unsigned long	    UNLONG;
typedef unsigned int	    UNINT;
typedef unsigned short 	    UNSHORT;
typedef unsigned char	    UNCHAR;
typedef long long           LLONG;
// typedef const char*         PCSTR;
// typedef const char*         PCSTR;
typedef wchar_t             wchar;
typedef unsigned char byte;
typedef const wchar_t*             pcwchar;
typedef __int64    INT64;
typedef unsigned __int64    UNINT64;
typedef const int           CINT;
typedef const UNINT           CUnInt;
typedef const void*         PCVOID;


//字符集 数据类型定义


#ifndef UNICODE
typedef char                tchar;
typedef char*               tpchar;
typedef const char*        tcpchar;
typedef string              tstring;
typedef stringstream tstringstream;
#define tmemset              memset
typedef ostringstream tostringstream;
#else
typedef wchar_t             tchar;
typedef wchar_t*            tpchar;
typedef wstring             tstring;
typedef const wchar_t*        tcpchar;
#define tmemset              wmemset
typedef wstringstream tstringstream;
typedef wostringstream tostringstream;

#endif
typedef const tstring& StringCRef;

typedef list<tstring>    LST_STRING;
typedef vector<tstring>    ARR_STRING;
typedef list<int>		LST_INT;

typedef map<tstring, tstring>   MAP_STRING;
typedef map<string, string>   MAP_STRINGA;
typedef pair<tstring, tstring> PAIR_STRING;
typedef list<pair<tstring, tstring>>    LST_PAIR_STR;

typedef vector<pair<tstring, tstring>>    ARR_PAIR_STR;
typedef __time64_t      TimeType;
//#define TimeType time_t


/************************************************************************
	宏定义                                                                      
************************************************************************/
//#define PI_M_LINK_LIB(libName) (#pragma comment(lib, libName))


#define Pi_M_NameSpace_Begin(name) namespace name{
#define Pi_M_NameSpace_End }
#define Pi_M_NameSpace_Using(name) using namespace name;


#define Pi_NameSpace_Begin namespace ns_PiPi{
#define Pi_NameSpace_End }
#define Pi_NameSpace_Using using namespace ns_PiPi;

/****************** dll 项目导出宏************************************
		
#if defined(__cplusplus)
#define NCCOMMON_EXTERN_C extern "C"
#else
#define NCCOMMON_EXTERN_C
#endif

#ifdef DLLCOMMON_EXPORTS
#define NCCOMMON_SYMBOL __declspec(dllexport)
#else
#define NCCOMMON_SYMBOL __declspec(dllimport)
#endif // DLLCOMMON_EXPORTS

#define NCCOMMON_API NCCOMMON_EXTERN_C NCCOMMON_SYMBOL
#define NCCOMMON_CALL _cdecl

************************************************************************/