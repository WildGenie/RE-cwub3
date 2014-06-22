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
	ShieldPop = GetPrivateProfileInt(L"功能管理", L"屏蔽弹窗", 1, tzIniPath);
	LocalVIP = GetPrivateProfileInt(L"功能管理", L"本地会员", 1, tzIniPath);
	Organize = GetPrivateProfileInt(L"功能管理", L"内存整理", 0, tzIniPath);
	FakeVer = GetPrivateProfileInt(L"功能管理", L"版本伪装", 0, tzIniPath);

	ShowHideicon = GetPrivateProfileInt(L"外观管理", L"隐藏图标", 0, tzIniPath);
	ShowBanber =  GetPrivateProfileInt(L"外观管理", L"QQBanner", 1, tzIniPath);
	ShowNewSkins =   GetPrivateProfileInt(L"外观管理", L"皮肤提醒", 0, tzIniPath);
	ShowAppBox =  GetPrivateProfileInt(L"外观管理", L"应用盒子", 1, tzIniPath);
	ShowImageView = GetPrivateProfileInt(L"外观管理", L"内置看图", 1, tzIniPath);
	ShowFaceBtn = GetPrivateProfileInt(L"外观管理", L"魔法表情", 1, tzIniPath);
	ShowSafeEntry = GetPrivateProfileInt(L"外观管理", L"安全入口", 0, tzIniPath);
	ShowLevelBtn = GetPrivateProfileInt(L"外观管理", L"等级按钮", 0, tzIniPath);
	ShowUpdateEntry = GetPrivateProfileInt(L"外观管理", L"更新入口", 1, tzIniPath);
	//ShowHelpBtn = GetPrivateProfileInt(L"外观管理", L"帮助按钮", 1, tzIniPath);

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
	int SkinsCache = GetPrivateProfileInt(L"外观管理", L"皮肤缓存", -1, tzIniPath);
	if(SkinsCache==-1 || SkinsCache!=SkinsCache2)
	{
		/*
		TCHAR AppData[MAX_PATH+1];
		SHGetSpecialFolderPath(NULL, AppData, CSIDL_APPDATA, FALSE);
		wcscat(AppData,L"\\Tencent\\QQ\\rdo.cache");

		DeleteFile(AppData);
		*/

		SetPrivateProfileInt(L"外观管理", L"皮肤缓存", SkinsCache2, tzIniPath);
		need_update_rdb = true;
	}
}