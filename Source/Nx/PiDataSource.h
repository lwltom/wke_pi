#pragma once
/*******************************************************************
*  文件名称:
*  简要描述:
*
*  创建日期: 2016-12-27
*  作者: liwanliang
*  说明: 让数据拖动出去的模块
*
*  修改日期:
*  作者:
*  说明:
******************************************************************/

#include "PiTypeDef.h"
#include <windows.h>

class CPiDataSource
{
public:
	CPiDataSource();
	~CPiDataSource();
public:
	bool PrepareDrag();
	bool BeginDrag(tcpchar szPath, RECT rt);
	void GeneralPic(RECT rtPic);
	void GeneralPic(tcpchar szPath);
	bool Drag(tcpchar szPath);
	void SetWindow(HWND hWnd);
	bool IsFrstDrag();
	void OnFirstDrag();
	bool CanDrag();
	void CancelDrag();
	void SetClientPos(bool bClient);
	void SetDragImage(HBITMAP hBitDrag);
private:
	HWND				m_hWnd;			//ref
	HDC					m_dc;
	HBITMAP				m_hDragBitmap;
	bool m_bDraging;
	bool m_bBtnDown;
	bool m_bClientPos;
};

