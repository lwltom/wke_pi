#pragma once
#include <windows.h>
#include "PiTypeDef.h"
class CPiGdip
{
public:
	CPiGdip();
	~CPiGdip();
private:
	ULONG_PTR m_gdiplusToken;
};
class CGDIPUtil
{
public:
	CGDIPUtil(void);
	~CGDIPUtil(void);
public:
	
	static int GetEncoderClsid( const WCHAR* format, CLSID* pClsid );
	static void ClipPic( LPCTSTR strPicSize, LPCTSTR strPicSrc );
	static HBITMAP GetBitmapFromImage(tcpchar szPath);
};
