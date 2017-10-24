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
#include "stdafx.h"


//#pragma comment(lib, "libsysaux.lib")
//#pragma comment( linker, "/SUBSYSTEM:\"WINDOWS\" /entry:\"mainCRTStartup\"" )  //这里修改函数的入口点

//#pragma comment( linker, "/SUBSYSTEM:WINDOWS" )  //这个宏有点小问题

//这里把子系统设为windows 这样就没有控制台了

// dllmain.cpp : 定义 DLL 应用程序的入口点。

HMODULE g_hDllModule; //定义Dll本身的句柄，方便自身函数回调


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

int _stdcall Add(int a, int b)
{
	return a + b;
}