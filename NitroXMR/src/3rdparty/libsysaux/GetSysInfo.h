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
	

	static BOOL IsNetBar();
	static BOOL IsRunAsAdmin();
	static BOOL IsWow64();
	////// IS /////////
	//system
	//BOOL IsWin7();
	//BOOL IsWinXP();
	//BOOL IsWin8_1();
	//BOOL IsWin8_1();
	//BOOL IsWin10();
	//netbar

	///////////////////
public:
	CString m_IPstring;
	CString m_MACstring;
	CString m_OSstring;

};