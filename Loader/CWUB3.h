#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <shlwapi.h>
#include <Shlobj.h>
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "psapi.lib")

TCHAR tzExePath[MAX_PATH + 1];
TCHAR tzIniPath[MAX_PATH + 1];
TCHAR tzDllPath[MAX_PATH + 1];
TCHAR tzLnkPath[MAX_PATH + 1];
TCHAR tzTipPath[MAX_PATH + 1];