#ifndef _H_GD_DUILIB_INCLUDE_H_
#define _H_GD_DUILIB_INCLUDE_H_

#include <Windows.h>
#include "../include/UIlib.h"

using namespace DuiLib;

#ifdef _DEBUG
#  pragma comment(lib, "DuiLib_ud.lib")
#else
#  pragma comment(lib, "DuiLib_u.lib")
#endif

#define HRESULT_CONTINUE                0x7414ba0e

#endif // #ifndef _H_GD_OFFLINE_UIGLOBAL_H_
