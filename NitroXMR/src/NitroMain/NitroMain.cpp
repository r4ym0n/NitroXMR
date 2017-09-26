// NitroMain.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "App.h"
#include "libsysaux.h"

int main(int argc, char **argv)
{
	sysConfig();

	App minerApp(argc, argv);
    return minerApp.exec();
}

