#include "stdafx.h"
#include "ToolKit.h"


CToolKit::CToolKit()
{

	

}


CToolKit::~CToolKit()
{

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

