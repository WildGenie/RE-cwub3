#include "CWUB3.h"

void HelpLoad(const wchar_t *main_path)
{
	TCHAR buff[MAX_PATH];
	wcscpy(buff,main_path);
	wcscat(buff,TEXT("addins\\*.*"));

	WIN32_FIND_DATA ffbuf;
	HANDLE hfind = FindFirstFile(buff, &ffbuf);
	if (hfind != INVALID_HANDLE_VALUE)
	{
		do
		{
			wcscpy(buff,main_path);
			wcscat(buff,L"addins\\");
			wcscat(buff,ffbuf.cFileName);

			if( isEndWith(buff, L".dll") )
			{
				HINSTANCE m_hModule = LoadLibrary(buff);
				if( isEndWith(buff,L"qqext.dll") )
				{
					if (m_hModule)
					{
						FARPROC fpAddress = GetProcAddress(m_hModule, "Onload");
						if (fpAddress != NULL)
						{
							typedef DWORD (WINAPI * Onload) (LPVOID);
							((Onload)fpAddress)(NULL);
						}
					}
				}
				else if( isEndWith(buff,L"QQScript.dll") )
				{
					if (m_hModule)
					{
						FARPROC fpAddress = GetProcAddress(m_hModule, "InitEngine");
						if (fpAddress != NULL)
						{
							typedef void (__cdecl * InitEngine) ();
							((InitEngine)fpAddress)();
						}
					}
				}
			}
		}
		while (FindNextFile(hfind, &ffbuf));
		FindClose(hfind);
	}
}
BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		g_hModule = hModule;
		LoadSysDll();

		//保持单实例
		wsprintf(tzExePath, (_T("CWUB3.MUTEX.PID%X")), GetCurrentProcessId());
		::CreateMutex(NULL, TRUE, tzExePath);
		if(GetLastError()   ==   ERROR_ALREADY_EXISTS)
		{
			return TRUE;
		}

		//获取关键路径
		GetModuleFileName(0, tzExePath, MAX_PATH);
		GetModuleFileName(hModule, tzIniPath, MAX_PATH);
		GetModuleFileName(hModule, tzDllPath, MAX_PATH);

		*(wcsrchr(tzIniPath, '.')) = 0;
		wcscat(tzIniPath, L".ini");
		//PathRenameExtension(tzIniPath, L".ini");

		//OutputDebugString(tzIniPath);

		if( isEndWith(tzExePath,L"QQ.exe") || isEndWith(tzExePath,L"TM.exe") )
		{
			//修复入口
			RestoreMain();

			//读取配置
			ReadConfig();

			//协助加载
			wchar_t WorkPath[MAX_PATH + 1];
			GetModuleFileName(hModule, WorkPath, MAX_PATH);
			wchar_t *str = wcsrchr(WorkPath, '\\');
			if(str) *(str+1) = 0;
			HelpLoad(WorkPath);

			//去校验
			RemoveCheck();

			//拦截创建程序
			OldCreateProcess = (GCREATEPROCESS)HookEngine.InstallHook((DWORD)CreateProcessW, (DWORD)MyCreateProcess);  //HookEngine.Uninstallhook(OldCreateProcess);
			//OldCreateDirectoryW = (MYCreateDirectoryW)HookEngine.InstallHook((DWORD)CreateDirectoryW, (DWORD)MyCreateDirectoryW);

			//网络过滤
			WS2_32.Load(L"WS2_32.dll");
			SendOldProc = (SEND)HookEngine.InstallHook((DWORD)WS2_32.GetFunc("send"),(DWORD)MySend);

			//皮肤补丁
			//OldMapViewOfFile = (MYMapViewOfFile)HookEngine.InstallHook((DWORD)MapViewOfFile, (DWORD)MyMapViewOfFile);
			//OldCreateFileMapping = (MYCreateFileMapping)HookEngine.InstallHook((DWORD)CreateFileMappingW, (DWORD)MyCreateFileMapping);
			OldCreateFile = (MYCreateFile)HookEngine.InstallHook((DWORD)CreateFileW, (DWORD)MyCreateFile);

			//本地会员、版本伪装、内置看图、隐藏图标
			localVIP();

			//杂项
			Miscellaneous();

			//屏蔽弹窗、应用盒子、节日LOGO、皮肤提醒。插件管理。
			QQWrap();

			//幽香线程：内存整理、老板键
			_beginthread(CWUB3_Thread,0,NULL);


		}
		else
		{
			if( isEndWith(tzExePath,L"bugreport.exe") )
			{
				//修复入口
				RestoreMain();

				OldCreateProcess = (GCREATEPROCESS)HookEngine.InstallHook((DWORD)CreateProcessW, (DWORD)MyCreateProcess);
			}
		}
		break;
	}
	return TRUE;
}