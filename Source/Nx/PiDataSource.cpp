#include "stdafx.h"
#include "PiDataSource.h"
#include "PiDragDropImpl.h"
#include <string>
#include <gdiplus.h>
#include <assert.h> 
#include "GDIPUtil.h"
#pragma comment(lib, "GdiPlus.lib")

using std::string;
using std::wstring;


CPiDataSource::CPiDataSource()
{
	m_hWnd			= NULL;
	m_dc			= NULL;
	m_hDragBitmap	= NULL;
	HRESULT hR = OleInitialize(NULL);
	hR = 0;
	m_bClientPos = true;
}


CPiDataSource::~CPiDataSource()
{
	if (m_hDragBitmap)
	{
		DeleteObject(m_hDragBitmap);
		m_hDragBitmap = NULL;
	}

	if (m_hWnd && m_dc)
	{
		::ReleaseDC(m_hWnd, m_dc);
		m_dc = NULL;
	}
	OleUninitialize();
}

bool CPiDataSource::Drag(tcpchar szPath)
{
	if (!szPath)
	{
		return false;
	}

	CPiIDropSource* pdsrc = new CPiIDropSource;
	if (pdsrc == NULL) return 0;
	pdsrc->AddRef();


	CPiIDataObject* pdobj = new CPiIDataObject(pdsrc);
	if (pdobj == NULL) return 0;
	pdobj->AddRef();


	DROPFILES*     pDrop;
	UINT           uBuffSize = 0;
	HGLOBAL        hgDrop;
	wstring			strFile(szPath);
	TCHAR*         pszBuff;

	uBuffSize += strFile.length() + 1;
	uBuffSize = sizeof(DROPFILES) + sizeof(TCHAR) * (uBuffSize + 1);

	hgDrop = GlobalAlloc(GHND | GMEM_SHARE, uBuffSize);

	if (NULL == hgDrop)
		return false;

	pDrop = (DROPFILES*)GlobalLock(hgDrop);

	if (NULL == pDrop)
	{
		GlobalFree(hgDrop);
		return false;
	}

	// Fill in the DROPFILES struct.

	pDrop->pFiles = sizeof(DROPFILES);

#ifdef _UNICODE
	// If we're compiling for Unicode, set the Unicode flag in the struct to
	// indicate it contains Unicode strings.

	pDrop->fWide = TRUE;
#endif
	pszBuff = (TCHAR*)(LPBYTE(pDrop) + sizeof(DROPFILES));

	lstrcpy(pszBuff, (LPCTSTR)strFile.c_str());
	pszBuff = 1 + _tcschr(pszBuff, '\0');

	GlobalUnlock(hgDrop);

	// Put the data in the data source.


	FORMATETC fmtetc = { 0 };
	STGMEDIUM medium = { 0 };
	/*fmtetc.dwAspect = DVASPECT_CONTENT;
	fmtetc.lindex = -1;
	//////////////////////////////////////
	fmtetc.cfFormat = CF_BITMAP;
	fmtetc.tymed = TYMED_GDI;
	medium.tymed = TYMED_GDI;
	pdobj->SetData(&fmtetc, &medium, FALSE);*/

	{
		//file
		fmtetc.cfFormat = CF_HDROP;
		fmtetc.ptd = NULL;
		fmtetc.dwAspect = DVASPECT_CONTENT;
		fmtetc.lindex = -1;
		fmtetc.tymed = TYMED_HGLOBAL;

		medium.tymed = TYMED_HGLOBAL;
		medium.hGlobal = hgDrop;
		pdobj->SetData(&fmtetc, &medium, TRUE);
	}

	//HBITMAP hBitmap = (HBITMAP)OleDuplicateData(m_hDragBitmap, fmtetc.cfFormat, NULL);
	HBITMAP hBitmap = m_hDragBitmap;
	if (hBitmap)
	{
		//medium.hBitmap = hBitmap;
		//medium.hBitmap = m_hDragBitmap;
		//pdobj->SetData(&fmtetc, &medium, FALSE);
		//////////////////////////////////////
		BITMAP bmap;
		GetObject(hBitmap, sizeof(BITMAP), &bmap);
		RECT rc = { 0, 0, bmap.bmWidth, bmap.bmHeight };
		/*fmtetc.cfFormat = CF_ENHMETAFILE;
		fmtetc.tymed = TYMED_ENHMF;
		HDC hMetaDC = CreateEnhMetaFile(m_dc, NULL, NULL, NULL);
		HDC hdcMem = CreateCompatibleDC(m_dc);
		HGDIOBJ hOldBmp = ::SelectObject(hdcMem, hBitmap);
		::BitBlt(hMetaDC, 0, 0, rc.right, rc.bottom, hdcMem, 0, 0, SRCCOPY);
		::SelectObject(hdcMem, hOldBmp);
		medium.hEnhMetaFile = CloseEnhMetaFile(hMetaDC);
		DeleteDC(hdcMem);
		medium.tymed = TYMED_ENHMF;
		pdobj->SetData(&fmtetc, &medium, TRUE);*/
		//////////////////////////////////////
		CDragSourceHelper dragSrcHelper;
		POINT ptDrag = { 0 };
		ptDrag.x = bmap.bmWidth / 2;
		ptDrag.y = bmap.bmHeight / 2;
		dragSrcHelper.InitializeFromBitmap(hBitmap, ptDrag, rc, pdobj); //will own the bmp
	}


	DWORD dwEffect; 
	HRESULT hr = ::DoDragDrop(pdobj, pdsrc, DROPEFFECT_COPY /*| DROPEFFECT_MOVE*/, &dwEffect);
	pdsrc->Release();
	pdobj->Release();
	::DeleteObject(m_hDragBitmap);
	m_hDragBitmap = NULL;

	return true;
}

void CPiDataSource::GeneralPic(RECT rtPic)
{
	if (!m_dc || rtPic.left == rtPic.right)
	{
		return;
	}
	SIZE szPic = { rtPic.right - rtPic.left, rtPic.bottom - rtPic.top };
	//RECT rc = {0, 0, 100, 100};
	int cx = szPic.cx;
	int cy = szPic.cy;

	HDC hPaintDC = ::CreateCompatibleDC(m_dc);
	HBITMAP hPaintBitmap = ::CreateCompatibleBitmap(m_dc, cx, cy);
	assert(hPaintDC);
	assert(hPaintBitmap);
	HBITMAP hOldPaintBitmap = (HBITMAP) ::SelectObject(hPaintDC, hPaintBitmap);
	//pControl->DoPaint(hPaintDC, rc);			//限制内容
	{
		//draw Drag Image
		/*DWORD color = 4290305275;
		Gdiplus::Graphics graphics(hPaintDC);
		Gdiplus::SolidBrush brush(Gdiplus::Color((LOBYTE((color) >> 24)), GetBValue(color), GetGValue(color), GetRValue(color)));
		graphics.FillRectangle(&brush, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top);*/

	}

	{
		//获取dc里指定区域的图片
		::BitBlt(hPaintDC, 0, 0, rtPic.right - rtPic.left, rtPic.bottom - rtPic.top, m_dc, rtPic.left, rtPic.top, SRCCOPY);

	}
	BITMAPINFO bmi = { 0 };
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = cx;
	bmi.bmiHeader.biHeight = cy;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = cx * cy * sizeof(DWORD);
	LPDWORD pDest = NULL;
	HDC hCloneDC = ::CreateCompatibleDC(m_dc);
	HBITMAP hBitmap = ::CreateDIBSection(m_dc, &bmi, DIB_RGB_COLORS, (LPVOID*)&pDest, NULL, 0);
	assert(hCloneDC);
	assert(hBitmap);
	if (hBitmap != NULL)
	{
		HBITMAP hOldBitmap = (HBITMAP) ::SelectObject(hCloneDC, hBitmap);
		::BitBlt(hCloneDC, 0, 0, cx, cy, hPaintDC, 0, 0, SRCCOPY);
		::SelectObject(hCloneDC, hOldBitmap);
		::DeleteDC(hCloneDC);
		::GdiFlush();
	}

	// Cleanup
	::SelectObject(hPaintDC, hOldPaintBitmap);
	::DeleteObject(hPaintBitmap);
	::DeleteDC(hPaintDC);
	m_hDragBitmap = hBitmap;
	
}

void CPiDataSource::GeneralPic(tcpchar szPath)
{
	HBITMAP hBitmap = CGDIPUtil::GetBitmapFromImage(szPath);
	if (!hBitmap)
	{
		return;
	}
	if (m_hDragBitmap)
	{
		DeleteObject(m_hDragBitmap);
		m_hDragBitmap = NULL;
	}
	m_hDragBitmap = hBitmap;
	return;	
}

void CPiDataSource::SetWindow(HWND hWnd)
{
	if (hWnd == m_hWnd)
	{
		return;
	}
	m_hWnd = hWnd;
	if (m_bClientPos)
	{
		m_dc = ::GetDC(m_hWnd);
	}
	else
	{
		m_dc = ::GetWindowDC(m_hWnd);
	}
}

bool CPiDataSource::BeginDrag(tcpchar szPath, RECT rt)
{
	if (!CanDrag())
	{
		return false;
	}
	if (IsFrstDrag())
	{
		OnFirstDrag();
	}
	
	GeneralPic(rt);
	bool bRet = Drag(szPath);

	m_bBtnDown = false;
	return true;
}

bool CPiDataSource::PrepareDrag()
{
	m_bBtnDown = true;
	if (!m_bBtnDown || m_bDraging)
	{
		return false;
	}
	//first drag
	m_bDraging = true;
	return true;
}

bool CPiDataSource::IsFrstDrag()
{
	if (!m_bBtnDown || m_bDraging)
	{
		return false;
	}
	return true;
}

void CPiDataSource::OnFirstDrag()
{
	bool bRet = m_bDraging;
	
}

bool CPiDataSource::CanDrag()
{
	return m_bBtnDown;
}

void CPiDataSource::CancelDrag()
{
	m_bBtnDown = false;
}

void CPiDataSource::SetClientPos(bool bClient)
{
	m_bClientPos = bClient;
}

void CPiDataSource::SetDragImage(HBITMAP hBitDrag)
{
	m_hDragBitmap = hBitDrag;
}
