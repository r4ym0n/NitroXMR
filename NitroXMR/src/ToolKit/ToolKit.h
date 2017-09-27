#pragma once
#include "stdafx.h"
class CToolKit;

typedef  INT(CToolKit::*LPFUNC)(VOID);

class CToolKit
{
public:
	CToolKit();
	~CToolKit();

	int execExp(int order);
	enum expTable;
	enum expArch;

private:

	static LPFUNC funcList[16];
	INT exp_ms_11046(VOID);

};
enum CToolKit::expArch
{
	X86 = 0,
	X64 
};

enum CToolKit::expTable
{

	EXP_MS_11046 = 0,

};


