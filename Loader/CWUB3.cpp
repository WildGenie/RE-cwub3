#include "CWUB3.h"
#include "path.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 避免重复运行
	HANDLE hMutex = CreateMutex(NULL , TRUE , L"SHUAX.MUTEX.CWUB3");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		return 0;
	}
	
	// 获得ini配置文件目录
	GetModuleFileName(0, tzLnkPath, MAX_PATH);
	GetModuleFileName(0, tzIniPath, MAX_PATH);
	GetModuleFileName(0, tzDllPath, MAX_PATH);
	PathRenameExtension(tzIniPath, L".ini");
	PathRenameExtension(tzDllPath, L".dll");

	//检查基本文件
	if(!CheckPath(tzDllPath))
	{
		MessageBoxTimeoutW(0, L"幽香3核心文件 CWUB3.dll 缺失，请检查是否被杀毒软件隔离或删除！", L"幽香3", MB_TOPMOST | MB_ICONERROR);
		return 0;
	}

	//如果路径已经存在
	if ( GetPrivateProfileString(L"功能管理", L"文件路径", L"", tzExePath, MAX_PATH, tzIniPath) && CheckPath(tzExePath) )
	{
		InjectQQ(tzExePath);
	}
	else
	{
		//寻找QQ
		if( SearchQQDir() )
		{
			WritePrivateProfileString(L"功能管理", L"文件路径", tzExePath, tzIniPath);

			//创建快捷方式
			CreakLnk(tzLnkPath,L"幽香3");

			int ret = MessageBoxTimeoutW(0, L"恭喜你已经完成了幽香3的安装，快去桌面找找快捷方式吧。\n\n是否立刻启动幽香3？", L"幽香3",  MB_YESNO | MB_TOPMOST | MB_ICONINFORMATION);

			wcscpy(tzTipPath,tzIniPath);
			*(wcsrchr(tzTipPath, '\\')+1) = 0;
			wcscat(tzTipPath,L"Bubble.exe");
			ShellExecute(NULL, L"", tzTipPath, NULL, NULL, SW_SHOWNORMAL);

			if(ret==IDYES)
			{
				InjectQQ(tzExePath);
			}
		}
	}


	ReleaseMutex(hMutex); 
	CloseHandle(hMutex); 
	return 0;
}