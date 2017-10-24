#include "stdafx.h"
#include "ToolKit.h"


CToolKit::CToolKit()
{

	

}


CToolKit::~CToolKit()
{

}

BOOL CToolKit::IsAdmin()
{
	bAdmin = CSysInfo::IsRunAsAdmin();
	return bAdmin;
}


int CToolKit::execExp(int order)
{	

	int ret = (this->*funcList[order])();
	return ret;
}

//这里对EXP 进行队列
LPFUNC CToolKit::funcList[16] = {
	&CToolKit::exp_ms_11046,
};

