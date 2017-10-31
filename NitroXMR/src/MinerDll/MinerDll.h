#pragma once
#include "stdafx.h"

BOOL createMinerThread();
DWORD WINAPI MyMain(LPVOID lpParam);
BOOL IsClient();
TCHAR* waitClient();
