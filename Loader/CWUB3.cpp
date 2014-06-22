#include "CWUB3.h"
#include "path.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �����ظ�����
	HANDLE hMutex = CreateMutex(NULL , TRUE , L"SHUAX.MUTEX.CWUB3");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hMutex);
		return 0;
	}
	
	// ���ini�����ļ�Ŀ¼
	GetModuleFileName(0, tzLnkPath, MAX_PATH);
	GetModuleFileName(0, tzIniPath, MAX_PATH);
	GetModuleFileName(0, tzDllPath, MAX_PATH);
	PathRenameExtension(tzIniPath, L".ini");
	PathRenameExtension(tzDllPath, L".dll");

	//�������ļ�
	if(!CheckPath(tzDllPath))
	{
		MessageBoxTimeoutW(0, L"����3�����ļ� CWUB3.dll ȱʧ�������Ƿ�ɱ����������ɾ����", L"����3", MB_TOPMOST | MB_ICONERROR);
		return 0;
	}

	//���·���Ѿ�����
	if ( GetPrivateProfileString(L"���ܹ���", L"�ļ�·��", L"", tzExePath, MAX_PATH, tzIniPath) && CheckPath(tzExePath) )
	{
		InjectQQ(tzExePath);
	}
	else
	{
		//Ѱ��QQ
		if( SearchQQDir() )
		{
			WritePrivateProfileString(L"���ܹ���", L"�ļ�·��", tzExePath, tzIniPath);

			//������ݷ�ʽ
			CreakLnk(tzLnkPath,L"����3");

			int ret = MessageBoxTimeoutW(0, L"��ϲ���Ѿ����������3�İ�װ����ȥ�������ҿ�ݷ�ʽ�ɡ�\n\n�Ƿ�������������3��", L"����3",  MB_YESNO | MB_TOPMOST | MB_ICONINFORMATION);

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