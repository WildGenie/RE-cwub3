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

		//���ֵ�ʵ��
		wsprintf(tzExePath, (_T("CWUB3.MUTEX.PID%X")), GetCurrentProcessId());
		::CreateMutex(NULL, TRUE, tzExePath);
		if(GetLastError()   ==   ERROR_ALREADY_EXISTS)
		{
			return TRUE;
		}

		//��ȡ�ؼ�·��
		GetModuleFileName(0, tzExePath, MAX_PATH);
		GetModuleFileName(hModule, tzIniPath, MAX_PATH);
		GetModuleFileName(hModule, tzDllPath, MAX_PATH);

		*(wcsrchr(tzIniPath, '.')) = 0;
		wcscat(tzIniPath, L".ini");
		//PathRenameExtension(tzIniPath, L".ini");

		//OutputDebugString(tzIniPath);

		if( isEndWith(tzExePath,L"QQ.exe") || isEndWith(tzExePath,L"TM.exe") )
		{
			//�޸����
			RestoreMain();

			//��ȡ����
			ReadConfig();

			//Э������
			wchar_t WorkPath[MAX_PATH + 1];
			GetModuleFileName(hModule, WorkPath, MAX_PATH);
			wchar_t *str = wcsrchr(WorkPath, '\\');
			if(str) *(str+1) = 0;
			HelpLoad(WorkPath);

			//ȥУ��
			RemoveCheck();

			//���ش�������
			OldCreateProcess = (GCREATEPROCESS)HookEngine.InstallHook((DWORD)CreateProcessW, (DWORD)MyCreateProcess);  //HookEngine.Uninstallhook(OldCreateProcess);
			//OldCreateDirectoryW = (MYCreateDirectoryW)HookEngine.InstallHook((DWORD)CreateDirectoryW, (DWORD)MyCreateDirectoryW);

			//�������
			WS2_32.Load(L"WS2_32.dll");
			SendOldProc = (SEND)HookEngine.InstallHook((DWORD)WS2_32.GetFunc("send"),(DWORD)MySend);

			//Ƥ������
			//OldMapViewOfFile = (MYMapViewOfFile)HookEngine.InstallHook((DWORD)MapViewOfFile, (DWORD)MyMapViewOfFile);
			//OldCreateFileMapping = (MYCreateFileMapping)HookEngine.InstallHook((DWORD)CreateFileMappingW, (DWORD)MyCreateFileMapping);
			OldCreateFile = (MYCreateFile)HookEngine.InstallHook((DWORD)CreateFileW, (DWORD)MyCreateFile);

			//���ػ�Ա���汾αװ�����ÿ�ͼ������ͼ��
			localVIP();

			//����
			Miscellaneous();

			//���ε�����Ӧ�ú��ӡ�����LOGO��Ƥ�����ѡ��������
			QQWrap();

			//�����̣߳��ڴ������ϰ��
			_beginthread(CWUB3_Thread,0,NULL);


		}
		else
		{
			if( isEndWith(tzExePath,L"bugreport.exe") )
			{
				//�޸����
				RestoreMain();

				OldCreateProcess = (GCREATEPROCESS)HookEngine.InstallHook((DWORD)CreateProcessW, (DWORD)MyCreateProcess);
			}
		}
		break;
	}
	return TRUE;
}