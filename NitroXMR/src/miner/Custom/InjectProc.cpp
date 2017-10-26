#include "stdafx.h"
#include "InjectProc.h"

// 远程插入线程
// char szDllFullPath[] DLL文件完整路径.
// DWORD dwRemoteProcessID 要插入的进程ID号
// 返回: TRUE 插入进程成功
// FALSE 失败
BOOL InjectDll(TCHAR szDllFullPath[], DWORD dwRemoteProcessID)
{

	
	HANDLE hRemoteProcess;

	if (EnableDebugPriv(SE_DEBUG_NAME) == 0)
	{
		return FALSE;
	}

	// 打开远程线程
	if ((hRemoteProcess = OpenProcess(PROCESS_CREATE_THREAD |
		PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwRemoteProcessID)) ==
		NULL)
	{
		return FALSE;
	}

	TCHAR * pszLibFileRemote;

	//使用VirtualAllocEx函数在远程进程内存地址空间分配DLL文件名缓冲区

	pszLibFileRemote = (TCHAR *)VirtualAllocEx(
		hRemoteProcess, NULL, lstrlen(szDllFullPath)*sizeof(TCHAR) + sizeof(TCHAR), MEM_COMMIT, PAGE_READWRITE);

	if (pszLibFileRemote == NULL)
	{
		MessageBox(GetDesktopWindow(), L"Failed 1", L"info", MB_OK);

		return FALSE;
	}

	if (WriteProcessMemory(hRemoteProcess, pszLibFileRemote, (void
		*)szDllFullPath, lstrlen(szDllFullPath)*sizeof(TCHAR) + sizeof(TCHAR), NULL) == 0)
	{
		MessageBox(GetDesktopWindow(), L"Failed 2", L"info", MB_OK);

		return FALSE;
	}
	
	/*LPTHREAD_START_ROUTINE  pfnStartAddr = (LPTHREAD_START_ROUTINE)
		GetProcAddress(GetModuleHandle(TEXT("user32")), "MessageBoxW");
	*/LPTHREAD_START_ROUTINE  pfnStartAddr = (LPTHREAD_START_ROUTINE)
	
		GetProcAddress(GetModuleHandle(TEXT("kernel32")), "LoadLibraryW");
	if (pfnStartAddr == NULL)
	{
		MessageBox(GetDesktopWindow(),  L"Failed 3", L"info", MB_OK);
		

		return FALSE;
	}

	

	// 通过建立远程连接的地址:pfnStartAddr
	// 传递参数 pszLibFileRemote 远程启动DLL
	// 启动远程线程 LoadLibraryA 通过远程线程调用用户的DLL文件

	HANDLE hRemoteThread;
	if ((hRemoteThread =
		CreateRemoteThread(hRemoteProcess, NULL, 0, pfnStartAddr, pszLibFileRemote, 0, NULL))
		== NULL)
	{
		TCHAR errNO[16];
	
		//CreateRemoteThread(hRemoteProcess, NULL, 0, pfnStartAddr, NULL, 0, NULL);
		
		wsprintf(errNO, L"%d", GetLastError());
		MessageBox(GetDesktopWindow(), errNO, L"info", MB_OK);
		MessageBox(GetDesktopWindow(), L"Failed 4", L"info", MB_OK);
		return FALSE;
	}
	else
	{
		MessageBox(GetDesktopWindow(), L"success", L"success", MB_OK);

	}
	return TRUE;
}

//这个函数也会用到的:


////////////////////////////////////////////////////////////////////////

// 获取进程ID号
// 如无此进程则返回 0;
// char szProcName[] 进程名: .exe文件.
DWORD GetProcID(TCHAR szProcName[])
{
	HANDLE th = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe = { sizeof(pe) };
	DWORD dwProcID = 0;

	BOOL bOK = Process32First(th, &pe);
	while (bOK)
	{
		bOK = Process32Next(th, &pe);

		LPCTSTR lpszExeFile = wcschr(pe.szExeFile, '//');
			if (lpszExeFile == NULL)
				lpszExeFile = pe.szExeFile;
			else
				lpszExeFile++;

		if (wcscmp(szProcName, lpszExeFile) == 0)
		{
			dwProcID = pe.th32ProcessID;
			break;
		}
	}


	return dwProcID;
}


////////////////////////////////////////////////////////////////////////

// 提升系统权限到DEBUG模式
int EnableDebugPriv(TCHAR szName[])
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;

	// 打开进程环令牌
	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))

	{
		return 0;
	}
	if (!LookupPrivilegeValue(NULL, szName, &luid))
	{
		return 0;
	}
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid = luid;
	// 调整权限
	if (!AdjustTokenPrivileges(hToken, 0, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL))

	{
		return 0;
	}

	return 1;
}