#pragma warning(disable:4800)

HINSTANCE g_hModule;

int isBossKey = 0;

bool ShowHideicon;
//bool ShowHelpBtn;
bool ShowFaceBtn;
bool ShowSafeEntry;
bool ShowUpdateEntry;
bool ShowLevelBtn;
bool ShowAppBox;
bool ShowBanber;
bool ShowNewSkins;
bool ShowImageView;

bool ShieldPop;
bool LocalVIP;
bool Organize;
int FakeVer;

bool need_update_rdb;
void ReadConfig()
{
	ShieldPop = GetPrivateProfileInt(L"���ܹ���", L"���ε���", 1, tzIniPath);
	LocalVIP = GetPrivateProfileInt(L"���ܹ���", L"���ػ�Ա", 1, tzIniPath);
	Organize = GetPrivateProfileInt(L"���ܹ���", L"�ڴ�����", 0, tzIniPath);
	FakeVer = GetPrivateProfileInt(L"���ܹ���", L"�汾αװ", 0, tzIniPath);

	ShowHideicon = GetPrivateProfileInt(L"��۹���", L"����ͼ��", 0, tzIniPath);
	ShowBanber =  GetPrivateProfileInt(L"��۹���", L"QQBanner", 1, tzIniPath);
	ShowNewSkins =   GetPrivateProfileInt(L"��۹���", L"Ƥ������", 0, tzIniPath);
	ShowAppBox =  GetPrivateProfileInt(L"��۹���", L"Ӧ�ú���", 1, tzIniPath);
	ShowImageView = GetPrivateProfileInt(L"��۹���", L"���ÿ�ͼ", 1, tzIniPath);
	ShowFaceBtn = GetPrivateProfileInt(L"��۹���", L"ħ������", 1, tzIniPath);
	ShowSafeEntry = GetPrivateProfileInt(L"��۹���", L"��ȫ���", 0, tzIniPath);
	ShowLevelBtn = GetPrivateProfileInt(L"��۹���", L"�ȼ���ť", 0, tzIniPath);
	ShowUpdateEntry = GetPrivateProfileInt(L"��۹���", L"�������", 1, tzIniPath);
	//ShowHelpBtn = GetPrivateProfileInt(L"��۹���", L"������ť", 1, tzIniPath);

	if(!ShowUpdateEntry)
	{
		BYTE PageSafeUpdate[] = {0xC7, 0x45, 0xFC, 0x21, 0x00, 0x00};//C7 45 FC 21 00 00
		long ConfigCenter = MemorySearch(L"ConfigCenter.dll",PageSafeUpdate, sizeof(PageSafeUpdate));
		//DbgPrint(L"ConfigCenter %X",ConfigCenter);
		if(ConfigCenter!=-1)
		{
			//DbgPrint(L"%X %X %X",ConfigCenter,ConfigCenter - 0x1A,*(BYTE*)( ConfigCenter + 0xC0 ));
			long start = ConfigCenter - 0x1A;
			long end = ConfigCenter + 0xC0;
			if( *(BYTE*)start == 0x68 && *(BYTE*)end == 0x68 )
			{
				WriteOffset(0xE9,start,end);
			}
			//WriteProcessMemory(g_process,(void*)(IM_1), &patch, sizeof(patch), NULL);
		}
	}

	need_update_rdb = false;
	int SkinsCache2 = (ShowFaceBtn) + (ShowSafeEntry<<1) + (ShowLevelBtn<<2) + (ShowUpdateEntry<<3);
	int SkinsCache = GetPrivateProfileInt(L"��۹���", L"Ƥ������", -1, tzIniPath);
	if(SkinsCache==-1 || SkinsCache!=SkinsCache2)
	{
		/*
		TCHAR AppData[MAX_PATH+1];
		SHGetSpecialFolderPath(NULL, AppData, CSIDL_APPDATA, FALSE);
		wcscat(AppData,L"\\Tencent\\QQ\\rdo.cache");

		DeleteFile(AppData);
		*/

		SetPrivateProfileInt(L"��۹���", L"Ƥ������", SkinsCache2, tzIniPath);
		need_update_rdb = true;
	}
}