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
	bool BeginDrag(tcpchar szPath);

	/************************************************************************
		fun:	从位置或图片路径生成缩略图
		param:	
		memo:	
	************************************************************************/
	void GeneralPic(RECT rtPic);
	void GeneralPic(tcpchar szPath);
	/************************************************************************
		fun:	设置图片缩略图句柄， 通过该句柄绘制拖动中图片的尺寸
		param:
		memo:
	************************************************************************/
	void SetDragImage(HBITMAP hBitDrag);

	bool Drag(tcpchar szPath);
	void SetWindow(HWND hWnd);
	bool IsFrstDrag();
	void OnFirstDrag();
	bool CanDrag();
	void CancelDrag();
	void SetClientPos(bool bClient);
	/************************************************************************
		fun:	设置缩略图图片最大的尺寸
		param:	
		memo:	
	************************************************************************/
	void SetDragImageSize(int cx, int cy);
	SIZE GetDragImgDistSize(const SIZE& szSrc);
private:
	HWND				m_hWnd;			//ref
	HDC					m_dc;
	HBITMAP				m_hDragBitmap;
	SIZE				m_szDragImg;
	bool m_bDraging;
	bool m_bBtnDown;
	bool m_bClientPos;
};

