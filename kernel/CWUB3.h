#pragma once

#define  _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <process.h>
#include <shlwapi.h>
#include <shlobj.h>
#pragma comment(lib, "Shlwapi.lib")

TCHAR tzIniPath[MAX_PATH + 1];
TCHAR tzDllPath[MAX_PATH + 1];
TCHAR tzExePath[MAX_PATH + 1];
TCHAR tcsHotkey[MAX_PATH + 1];

//#include "core/CoreLZMA.h"
#include "core/CoreUtil.h"
#include "core/CoreHook.h"
#include "core/InjectDll.h"
#include "core/config.h"
#include "core/CrackCheck.h"


HookLib WS2_32;
HookLib KernelUtil;

#define WM_MYHOTKEY (WM_APP+32)

#include "model/CreateProcess.h"
#include "model/UrlFilter.h"
#include "model/SkinsPatch.h"
#include "model/CWUBThread.h"
#include "model/localVIP.h"
#include "model/qqwrap.h"
#include "model/Miscellaneous.h"


// 声明导出函数
#pragma comment(linker, "/EXPORT:vSetDdrawflag=_My_Fun1,@1")
#pragma comment(linker, "/EXPORT:AlphaBlend=_My_Fun2,@2")
#pragma comment(linker, "/EXPORT:DllInitialize=_My_Fun3,PRIVATE")
#pragma comment(linker, "/EXPORT:GradientFill=_My_Fun4,@4")
#pragma comment(linker, "/EXPORT:TransparentBlt=_My_Fun5,@5")

// 函数实现,写成宏,简化代码量
#define IMPL_STUB_FUNC(n) \
	DWORD g_dwFunPtr##n=0; \
	extern "C" void _declspec(naked) My_Fun##n() \
{ \
	__asm jmp DWORD PTR[g_dwFunPtr##n] \
}

// 实现5个跳板函数
IMPL_STUB_FUNC(1);
IMPL_STUB_FUNC(2);
IMPL_STUB_FUNC(3);
IMPL_STUB_FUNC(4);
IMPL_STUB_FUNC(5);

#define INIT_STUB_FUNC(n,name) \
	g_dwFunPtr##n = (DWORD)GetProcAddress(hDll,name);


// 加载系统dll,初始化函数指针
void LoadSysDll()
{
	TCHAR szDLL[MAX_PATH+1];
	GetSystemDirectory(szDLL,MAX_PATH);
	lstrcat(szDLL,TEXT("\\msimg32.dll"));

	HINSTANCE hDll=LoadLibrary(szDLL);
	if (hDll!=NULL)
	{
		INIT_STUB_FUNC(1,"vSetDdrawflag");
		INIT_STUB_FUNC(2,"AlphaBlend");
		INIT_STUB_FUNC(3,"DllInitialize");
		INIT_STUB_FUNC(4,"GradientFill");
		INIT_STUB_FUNC(5,"TransparentBlt");

		//lpAlphaBlend = (LPALPHABLEND)g_dwFunPtr2;
	}
}