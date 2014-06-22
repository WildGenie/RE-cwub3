typedef long (__cdecl * MYRawCreateGFElementByXtml) (wchar_t *,DWORD,DWORD,DWORD);
MYRawCreateGFElementByXtml RawCreateGFElementByXtml = NULL;

typedef int (__cdecl * GetRegSubKeyBoolField) (wchar_t *,wchar_t *,wchar_t *,int *,DWORD ,DWORD);
GetRegSubKeyBoolField GetRegBool = NULL;

typedef int (__cdecl * MYConvertXMLStrToTXData) (wchar_t *,wchar_t * *,struct ITXData *,wchar_t *);
MYConvertXMLStrToTXData ConvertXMLStrToTXData = NULL;

typedef unsigned long (__cdecl* MYGetTextColor)(unsigned long);
MYGetTextColor QQGetTextColor = NULL;


// 插件过滤
typedef int (__cdecl * MYINITPLUGIN) (wchar_t *);
MYINITPLUGIN PluginOldProc = NULL;
int __cdecl MyInitPlugin(wchar_t * path)
{
	int state = GetPrivateProfileInt(L"插件管理", path, 1, tzIniPath);
	
	//保存配置
	SetPrivateProfileInt(L"插件管理",path, state, tzIniPath);

	if (state==0)  return 1;

	// 调用原来的函数
	if(PluginOldProc) return PluginOldProc(path);
	return 1;
}


long __cdecl MyRawCreateGFElementByXtml(wchar_t  *p1,DWORD p2,DWORD p3,DWORD p4)
{
    //if (wcsstr(p1,L"WBlog")) OutputDebugStringW(p1);
    //DbgPrint(L"%s",GetCallStack(&p1));
    
    if(ShowAppBox==0 && wcsstr(p1,L"sp_RichPage")!=0) return 0;
    if(ShowAppBox==0 && wcsstr(p1,L"AppBoxEntryBtn")!=0) return 0;

    if((isBossKey || ShieldPop) && wcsstr(p1,L"PopupTip.xml")!=0) return 0;

    if(RawCreateGFElementByXtml) return RawCreateGFElementByXtml(p1,p2,p3,p4);
    return 0;
}

int __cdecl MyGetRegBool(wchar_t *p1,wchar_t *p2,wchar_t *p3,int *p4,DWORD p5,DWORD p6)
{
    if(isBossKey)
    {
        if(wcscmp(p3,L"bEnableAllSound")==0
        || wcscmp(p3,L"bAutoPopupChatWnd")==0
        || wcscmp(p3,L"bEnableShakeWindowTip")==0
        )
        {
            int res = GetRegBool?GetRegBool(p1,p2,p3,p4,p5,p6):0;
            *p4 = 0;
            return res;
        }
    }

    if(GetRegBool) return GetRegBool(p1,p2,p3,p4,p5,p6);
    return 0;
}

int MyConvertXMLStrToTXData(wchar_t *p1,wchar_t * *p2,struct ITXData *p3,wchar_t *p4)
{
    if(wcsstr(p1,L"default_gtb")!=0)
    {
        char key[]={2,12,15,16,17,18,19};
        for(unsigned int i=0;i<sizeof(key);i++)
        {
            wchar_t filter[256];
            wsprintf(filter,L"<b id=\"%d\" visible=\"1\" privilege=\"1\" />",key[i]);
            wchar_t * str = replace(p1, filter,L"");
            wcscpy(p1,str);
            free(str);
		}
	}

    if(wcsstr(p1,L"QQLoginingPanelBottom")!=0)
    {
        return 0;
    }

    if(!ShowBanber && wcsstr(p1,L"LoginLogoConfig")!=0)
    {
        return 0;
    }
    if(!ShowNewSkins  && wcsstr(p1,L"</Skins>")!=0)
    {
        return 0;
    }
    //
    if(ConvertXMLStrToTXData) return ConvertXMLStrToTXData(p1,p2,p3,p4);
    return 0;
}

unsigned long __cdecl MyGetTextColor(unsigned long QQUIN)
{
    if(QQUIN==GetSelfUin())
    {
        return RGB(0x20,0xA0,0xFF);
    }
    else
    {
        if(QQUIN==GetSelfUin())
        {
            return RGB(0xFF,0xA0,0x20);
        }
    }
    if(QQGetTextColor) return QQGetTextColor(QQUIN);
    return 0;
}


//防止崩溃啊
//DWORD org;

/*
typedef int (__stdcall* MYCrash)(DWORD,DWORD,DWORD,DWORD);
MYCrash calledx = NULL;

int WINAPI QQCrash2(DWORD p1,DWORD p2,DWORD p3,DWORD p4)
{
	_asm
	{
		PUSH retfalse;
		push    fs:[0];
		mov    fs:[0],esp
	}
	calledx(p1,p2,p3,p4);
	return 0;


	retfalse:
		return 1;

}
*/
/*
void _declspec(naked) QQCrash()
{
	_asm
	{
		push eax;

		mov eax,DWORD PTR [esp+4]
		mov DWORD PTR ss:[esp-4],eax;

		mov eax,over
		mov DWORD PTR ss:[esp+4],eax;

		pop eax;
		push fs:[0];
		mov fs:[0],esp;

		sub esp,4

		push eax;
		push 1;
		push EDI;
		call edx;

		pop    DWORD PTR fs:[0] ;
		add    esp,4;

		jmp org;
	}

over:
	TerminateProcess((void*)-1,0);
	_asm
	{
		mov eax,1
		ret
	}
	//
}
*/
void QQWrap()
{
    HookLib AppUtil;
    HookLib AFUtil;
	HookLib Common;
	HookLib GF;

    AppUtil.Load(L"AppUtil.dll");
    AFUtil.Load(L"AFUtil.dll");
	Common.Load(L"Common.dll");
	GF.Load(L"GF.dll");
    
    // 去除QQ影音提示
    DWORD IsVideoFile = (DWORD)AFUtil.GetFunc("?IsVideoFile@Misc@Util@@YAHVCTXStringW@@@Z");
    if(!IsVideoFile)
    {
        IsVideoFile = (DWORD)AppUtil.GetFunc("?IsVideoFile@Misc@Util@@YAHVCTXStringW@@@Z");
    }
    if(IsVideoFile)
    {
        BYTE ret0[] = {0x31,0xC0,0xC3};
        WriteProcessMemory((void*)-1,(void*)IsVideoFile, &ret0, sizeof(ret0), NULL);
    }
    
    RawCreateGFElementByXtml = (MYRawCreateGFElementByXtml)HookEngine.InstallHook((DWORD)GF.GetFunc("?RawCreateGFElementByXtml@GF@Util@@YAJPA_WPAPAUIGFElement@@PAU3@0H@Z"),(DWORD)MyRawCreateGFElementByXtml);
    if(RawCreateGFElementByXtml==0)
    {
        RawCreateGFElementByXtml = (MYRawCreateGFElementByXtml)HookEngine.InstallHook((DWORD)GF.GetFunc("?RawCreateGFElementByXtml@GF@Util@@YAJPA_WPAPAUIGFElement@@PAU3@0@Z"),(DWORD)MyRawCreateGFElementByXtml);
    }
    
    //老板键隐藏窗口
    GetRegBool = (GetRegSubKeyBoolField)HookEngine.InstallHook((DWORD)AppUtil.GetFunc("?GetRegSubKeyBoolField@API@Registry@@YAHPA_W00PAHW4__MIDL___MIDL_itf_IRegistry_0000_0003@@W4__MIDL___MIDL_itf_IRegistry_0000_0004@@@Z"),(DWORD)MyGetRegBool);
    
    //XML2TXData
    ConvertXMLStrToTXData = (MYConvertXMLStrToTXData)HookEngine.InstallHook((DWORD)Common.GetFunc("?ConvertXMLStrToTXData@Convert@Util@@YAHPA_WPAPA_WPAUITXData@@0@Z"), (DWORD)MyConvertXMLStrToTXData);
    
    //自定义群颜色
    //QQGetTextColor = (MYGetTextColor)HookEngine.InstallHook((DWORD)AFUtil.GetFunc("?GetTextColor@CUserFlags@Contact@Util@@SAKK@Z"), (DWORD)MyGetTextColor);
    //if(QQGetTextColor==0) QQGetTextColor = (MYGetTextColor)HookEngine.InstallHook((DWORD)AFUtil.GetFunc("?GetTextColor@CGroupGrade@Group@Util@@SAKK@Z"), (DWORD)MyGetTextColor);

	//插件过滤
	PluginOldProc = (MYINITPLUGIN)HookEngine.InstallHook((DWORD)Common.GetFunc("?InitPluginCoreConfig@Boot@Util@@YAHPA_W@Z"), (DWORD)MyInitPlugin);

	/*
	//防止崩溃
	{
		BYTE IM_Crash[] = {0x50, 0x6A, 0x01, 0x57, 0xFF, 0xD2, 0x8B, 0x45};//50 6A 01 57 FF D2 8B 45
		long crash = MemorySearch(L"IM.dll",IM_Crash, sizeof(IM_Crash));

		//DbgPrint(L"%X",imageview);
		if(crash!=-1)
		{
			org = crash + 6;
			HookEngine.InstallHook(crash,(DWORD)QQCrash);
		}
	}
	*/
}