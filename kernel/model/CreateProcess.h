
typedef BOOL (WINAPI* GCREATEPROCESS)(
					LPCTSTR lpApplicationName,
					LPTSTR lpCommandLine,
					LPSECURITY_ATTRIBUTES lpProcessAttributes,
					LPSECURITY_ATTRIBUTES lpThreadAttributes,
					BOOL bInheritHandles,
					DWORD dwCreationFlags,
					LPVOID lpEnvironment,
					LPCTSTR lpCurrentDirectory,
					LPSTARTUPINFO lpStartupInfo,
					LPPROCESS_INFORMATION lpProcessInformation
					);
GCREATEPROCESS OldCreateProcess = NULL;

BOOL WINAPI MyCreateProcess(
					LPCTSTR lpApplicationName,
					LPTSTR lpCommandLine,
					LPSECURITY_ATTRIBUTES lpProcessAttributes,
					LPSECURITY_ATTRIBUTES lpThreadAttributes,
					BOOL bInheritHandles,
					DWORD dwCreationFlags,
					LPVOID lpEnvironment,
					LPCTSTR lpCurrentDirectory,
					LPSTARTUPINFO lpStartupInfo,
					LPPROCESS_INFORMATION lpProcessInformation
					)
{
	if( isEndWith(lpApplicationName,L"QQ.exe") || isEndWith(lpApplicationName,L"bugreport.exe") )
	{
		if(OldCreateProcess)
		{
			BOOL ret = OldCreateProcess(lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles,CREATE_SUSPENDED,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation);
			if(ret)
			{
				//×¢Èëdll
				InjectDll(lpProcessInformation->hProcess,lpProcessInformation->hThread,tzDllPath);

				//»Ö¸´³ÌÐòÖ´ÐÐ
				ResumeThread(lpProcessInformation->hThread);
				CloseHandle(lpProcessInformation->hProcess);
			}
			return ret;
		}
		return 0;
	}
	if(wcsstr(lpCommandLine,L"QQExternal.exe")!=0)
	{
		return 0;
	}
	if(wcsstr(lpCommandLine,L"auclt.exe")!=0)
	{
		if(wcsstr(lpCommandLine,L"/MU")==0) return 0;
	}
	if(wcsstr(lpCommandLine,L"txupd.exe")!=0)
	{
		if(wcsstr(lpCommandLine,L"/manual")==0) return 0;
	}
	if(wcsstr(lpCommandLine,L"Tencentdl.exe")!=0)
	{
		return 0;
	}

	
	//OutputDebugStringW(lpCommandLine);
    if(OldCreateProcess) return OldCreateProcess(lpApplicationName,lpCommandLine,lpProcessAttributes,lpThreadAttributes,bInheritHandles,dwCreationFlags,lpEnvironment,lpCurrentDirectory,lpStartupInfo,lpProcessInformation);
    return 0;
}


typedef DWORD (WINAPI* MYFileAttributes)(LPCTSTR lpFileName);
MYFileAttributes OldGetFileAttributes = NULL;

DWORD WINAPI MyFileAttributes(LPCTSTR lpFileName)
{
	//OutputDebugStringW(lpFileName);
	if(_wcsicmp(lpFileName,tzDllPath)==0) return 0xFFFFFFFF;
	if(OldGetFileAttributes) return OldGetFileAttributes(lpFileName);
	return 0xFFFFFFFF;
}

typedef BOOL (WINAPI* MYCreateDirectoryW)(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes);
MYCreateDirectoryW OldCreateDirectoryW = NULL;
BOOL WINAPI MyCreateDirectoryW(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	if(wcsstr(lpPathName,L"Plugin"))
	{
		OutputDebugStringW(lpPathName);
		return TRUE;
	}
	if(OldCreateDirectoryW) return OldCreateDirectoryW(lpPathName,lpSecurityAttributes);
	return FALSE;
}
