
#include "BossKey.h"

void MemoryClear()
{
    if (Organize)
    {
        SetProcessWorkingSetSize(GetCurrentProcess(), (DWORD) - 1, (DWORD) - 1);
    }
}

void CWUB3_Thread(PVOID pvoid)
{
    SetTimer(NULL, 0, 1000*60, (TIMERPROC)MemoryClear);//60��һ��

    if(GetPrivateProfileString(L"���ܹ���", _T("�ϰ��"), L"", tcsHotkey, MAX_PATH, tzIniPath))
    {
        CreateDialog(g_hModule,MAKEINTRESOURCE(101),NULL,BossKey);
    }

    MSG msg;
    while (GetMessage(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}