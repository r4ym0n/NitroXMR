#include "stdafx.h"
#include "libsysaux.h"



LIBAUX_API void msgBox()
{
	MessageBox(GetDesktopWindow(), L"ok", L"ok", MB_OK);

	return;

}