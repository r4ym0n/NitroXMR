#pragma once

#include "stdafx.h"


#include "GetSysInfo.h"

//test
LIBAUX_API void msgBox();

//系统初始配置
LIBAUX_API BOOL SysConfig();

//检查互斥体
BOOL checkMutex();

BOOL autoRun();
