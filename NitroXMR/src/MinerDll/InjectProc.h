#pragma once
#include "stdafx.h"

//注入
BOOL InjectDll(PCWSTR szDllFullPath, DWORD dwRemoteProcessID);

// 进程名解析Pid
DWORD GetProcID(TCHAR szProcName[]);

// 提升当前进程权限
int EnableDebugPriv(TCHAR szName[]);
