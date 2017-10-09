#include "stdafx.h"
#include "libsysaux.h"



LIBAUX_API VOID msgBox()
{
	MessageBox(GetDesktopWindow(), L"ok", L"ok", MB_OK);

	return;

}

LIBAUX_API BOOL SysConfig()
{
	//checkMutex();
	//copySelf();
	//autoRun();
	if (CSysInfo::IsRunAsAdmin())
	{
		MessageBox(NULL, L"admin", L"info", MB_OK);
	}
	if (CSysInfo::IsWow64())
	{
		MessageBox(NULL, L"X64", L"info", MB_OK);
	}
	if (CSysInfo::IsNetBar())
	{
		MessageBox(NULL, L"NetBar", L"info", MB_OK);
	}
	CSysInfo sysInfo;
	CString OS = sysInfo.GetOSInfo();
	CString IP = sysInfo.GetIPInfo();
	
	
	MessageBox(NULL, OS + L"\n", L"info", MB_OK);
	MessageBox(NULL, IP + L"\n", L"info", MB_OK);
	return TRUE;
}

//检查互斥体,主要做到不和启动项重复运行
BOOL checkMutex()
{
	HANDLE hMutex = NULL;

	//创建互斥对象 
	if (NULL != (hMutex = CreateMutex(NULL, FALSE, L"dig out")))
	{
		
		//如果mutex互斥体已经存在
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			MessageBox(NULL, TEXT("Another Instance is RUNNING!!"),
				TEXT("ERROE"), MB_OK | MB_ICONINFORMATION);
			
			//关闭互斥对象，退出程序; 
			exit(-1);
		}
	}
	return TRUE;
}

