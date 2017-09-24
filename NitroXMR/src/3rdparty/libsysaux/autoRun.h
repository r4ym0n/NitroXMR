#pragma once
#include "libsysaux.h"

//这里声明之前的随机执行路径
extern TCHAR executePath[MAX_PATH];

//看了看 主要有几种实现方法
//采用了简单的协注册表的方法
//功能写着写着报毒的就多了 Q.Q~


BOOL autoRun();
