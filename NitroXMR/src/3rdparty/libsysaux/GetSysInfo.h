#pragma once
#include "stdafx.h"
#include "libsysaux.h"

class CSysInfo 
{
public:
	CSysInfo() {};
	~CSysInfo() {};

	CString GetOSInfo();
	CString GetIPInfo();
	CString GetCPUInfo();
	CString GetMACInfo();
	

	////// IS /////////
	//system
	//BOOL IsWin7();
	//BOOL IsWinXP();
	//BOOL IsWin8_1();
	//BOOL IsWin8_1();
	//BOOL IsWin10();
	//netbar
	BOOL IsNetBar();

	///////////////////
public:
	CString m_IPstring;
	CString m_MACstring;
	CString m_OSstring;

};