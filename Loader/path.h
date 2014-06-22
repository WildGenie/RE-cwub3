#include <tlhelp32.h>
#include <psapi.h>

#include "InjectDll.h"

bool CheckPath(wchar_t *path)
{
    DWORD dwAttributes = GetFileAttributes(path);
    if (dwAttributes != 0xFFFFFFFF)
    {
        if ((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) return true;
    }
    return false;
}


bool SearchFromDir()
{
    GetCurrentDirectory(MAX_PATH, tzExePath);
    wcscat(tzExePath, L"\\QQ.exe");
    return CheckPath(tzExePath);
}

bool SearchFromReg()
{
    HKEY hKey;
    DWORD len = MAX_PATH;
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Tencent\\QQ2009", 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        if (RegQueryValueEx(hKey, L"Install", 0, 0, (unsigned char*)tzExePath, &len) == ERROR_SUCCESS)
        {
            wcscat(tzExePath, L"\\Bin\\QQ.exe");
            RegCloseKey(hKey);
            return CheckPath(tzExePath);
        }
    }
    return false;
}

bool GetProcessPathNameFromId(DWORD pid)
{
    HMODULE hModule;
    DWORD cbNeeded;

    HANDLE hProcess = OpenProcess(  PROCESS_ALL_ACCESS, FALSE, pid );
    // 句柄为空返回
    if (!hProcess)
    {
        return false;
    }
    EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbNeeded);
    //static wchar_t szModName[1024];
    GetModuleFileNameExW( hProcess, hModule, tzExePath, MAX_PATH);

    //wcscpy(exeString, szModName);

    // 关闭相应的资源
    CloseHandle( hProcess );
    return true;
}

bool SearchFromMem()
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot (TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);
    for (Process32First (hProcessSnap, &pe32); Process32Next (hProcessSnap, &pe32);)
    {
        if (_wcsicmp(pe32.szExeFile, L"QQ.exe") == 0 || _wcsicmp(pe32.szExeFile, L"TM.exe") == 0)
        {
            GetProcessPathNameFromId(pe32.th32ProcessID);
            //MessageBox(0,exeString,exeString,0);
            CloseHandle(hProcessSnap);
            return true;
        }
    }
    CloseHandle(hProcessSnap);
    return false;
}

bool SearchFromDlg()
{
    OPENFILENAME ofn;
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = tzExePath;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"QQ主程序 (QQ.exe)\0QQ.exe\0TM主程序 (TM.exe)\0TM.exe\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;

    if (GetOpenFileName(&ofn) != FALSE)
    {
        //MessageBox(0,exeString,exeString,0);
        return true;
    }

    return false;
}

bool SearchQQDir()
{
    if (SearchFromDir()) return true;
    if (SearchFromReg()) return true;
    if (SearchFromMem()) return true;
    if (SearchFromDlg()) return true;
    return false;
}


void CreakLnk(wchar_t *path,const wchar_t *name)
{
	HRESULT hr = CoInitialize(NULL);
	
	if (SUCCEEDED(hr))
	{
		IShellLink *pisl;
		
		hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (void**) & pisl);
		
		if (SUCCEEDED(hr))
		{
			IPersistFile* pIPF;
			pisl->SetPath(path);
			*(wcsrchr(path, '\\')+1) = 0;

			pisl->SetDescription(L"点击此处可启动幽香3");
			pisl->SetWorkingDirectory(path);
			hr = pisl->QueryInterface(IID_IPersistFile, (void**) & pIPF);
			
			if (SUCCEEDED(hr))
			{
				wchar_t lnk[MAX_PATH];
				SHGetSpecialFolderPath(NULL, lnk, CSIDL_DESKTOPDIRECTORY, FALSE);
				wcscat(lnk, L"\\");
				wcscat(lnk, name);
				wcscat(lnk, L".lnk");
				pIPF->Save(lnk, TRUE);
				//pIPF->SaveCompleted()
				pIPF->Release();
			}
			pisl->Release();
		}
		CoUninitialize();
	}
}
/*
char* Unicode2Ansi(const wchar_t* wszString)
{
	int ansiLen = ::WideCharToMultiByte(CP_ACP, 0, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
	char* szAnsi = (char*)malloc(sizeof(char) * (ansiLen + 1));
	::WideCharToMultiByte(CP_ACP, 0, wszString, wcslen(wszString), szAnsi, ansiLen, NULL, NULL);
	szAnsi[ansiLen] = '\0';
	return szAnsi;
}*/

typedef int (__stdcall *MSGBOXWAPI)(IN HWND hWnd, IN LPCWSTR lpText, IN LPCWSTR lpCaption, IN UINT uType, IN WORD wLanguageId, IN DWORD dwMilliseconds);
int MessageBoxTimeoutW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
	static MSGBOXWAPI MsgBoxTOW = NULL;
	if (!MsgBoxTOW)
	{
		HMODULE hUser32 = GetModuleHandle(L"user32.dll");
		if (hUser32)
		{
			MsgBoxTOW = (MSGBOXWAPI)GetProcAddress(hUser32,"MessageBoxTimeoutW");
		}
		else
		{
			return 0;
		}
	}
	if (MsgBoxTOW)
	{
		return MsgBoxTOW(hWnd, lpText, lpCaption, uType, 0, 8*1000);
	}
	return 0;
}

void InjectQQ(wchar_t *path)
{
	//启动进程
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};
	si.cb = sizeof(STARTUPINFO);

	if( !CreateProcessW(NULL, path, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, 0, &si, &pi) )
	{
		//失败
		MessageBoxTimeoutW(0, L"进程加载失败，请检查配置文件中的路径设置！", L"幽香3", MB_ICONERROR);
		WritePrivateProfileString(L"功能管理", L"文件路径", L"", tzIniPath);
	}
	else
	{
        //注入dll
        InjectDll(pi.hProcess,pi.hThread,tzDllPath);
        
        //恢复程序执行
        ResumeThread(pi.hThread);
        CloseHandle(pi.hProcess);
	}
}