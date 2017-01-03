#include "stdafx.h"
#include "PiMathUnit.h"
#include <limits>

Pi_NameSpace_Begin
bool FlowBigger(double n1, double n2)
{
	return abs(n1 - n2) > std::numeric_limits<float>::epsilon();
}

bool FlowEqual(double n1, double n2)
{
	return abs(n1 - n2) < std::numeric_limits<float>::epsilon();
}


SIZE CPiMath::GetKeepRadio(const SIZE& szSrc, const SIZE& szLimit)
{
	/************************************************************************
	等比缩放
	放大和缩放
	1. 图片整个尺寸小于限制尺寸用原图
	2. 缩小时， 取宽高缩放比例较大的一方作为缩放比
	3. 放大时， 取比例较小的一方作为缩放比
	4. 一边放大， 一边缩小， 则缩小

	缩小， radio > 1
	放大， radio < 1,
	************************************************************************/
	SIZE szBig = { 0, 0 };
	float fRadioCx = 1.0 * szSrc.cx / szLimit.cx;
	float fRadioCy = 1.0 * szSrc.cy / szLimit.cy;
	float nRadio = 0;
	bool bZoomOut = (fRadioCx > 1 || fRadioCy > 1);
	SIZE szDist = { 0 };
	if (bZoomOut)
	{
		nRadio = max(fRadioCx, fRadioCy);
	}
	else
	{
		nRadio = max(fRadioCx, fRadioCy);	//放大时， 比例为0~1范围,  约大的比例系数变更值小
	}
	szDist.cx = szSrc.cx / nRadio;
	szDist.cy = szSrc.cy / nRadio;
	return szDist;
}

Pi_NameSpace_End