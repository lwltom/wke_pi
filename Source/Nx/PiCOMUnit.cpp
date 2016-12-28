#include "stdafx.h"
#include "PiCOMUnit.h"
#include <objbase.h>

CPiCOMUnit::CPiCOMUnit()
{
	HRESULT hREt = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (SUCCEEDED(hREt))
	{
		hREt = 0;
	}
}


CPiCOMUnit::~CPiCOMUnit()
{
	CoUninitialize();
}
