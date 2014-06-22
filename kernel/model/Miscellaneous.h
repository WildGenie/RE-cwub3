void __cdecl TXLog_DoTXLogVW(void)
{
    return;
}

typedef LONG NTSTATUS, *PNTSTATUS;
#ifndef NT_SUCCESS
#define NT_SUCCESS(x) ((x)>=0)
#define STATUS_SUCCESS ((NTSTATUS)0)
#endif

typedef NTSTATUS (WINAPI* PFN_LdrLoadDll)(PWCHAR PathToFile OPTIONAL, ULONG Flags OPTIONAL, PUNICODE_STRING ModuleFileName, PHANDLE ModuleHandle);
PFN_LdrLoadDll s_pfnLdrLoadDll = NULL;//跳转地址
DWORD LdrLoadDll;//原始地址
NTSTATUS WINAPI MyLdrLoadDll(PWCHAR PathToFile OPTIONAL, ULONG Flags OPTIONAL, PUNICODE_STRING ModuleFileName, PHANDLE ModuleHandle)
{
    if(GetModuleHandle(ModuleFileName->Buffer) == 0)
    {
		//DbgPrint(L"MyLdrLoadDll:%s",ModuleFileName->Buffer);
        if(wcsstr(ModuleFileName->Buffer,L"SafeBase")!=0)
        {
            return -1;
        }
        if(wcsstr(ModuleFileName->Buffer,L"SSOPlatform.dll")!=0)
        {
            NTSTATUS res = 0;
            if(s_pfnLdrLoadDll) res = s_pfnLdrLoadDll(PathToFile, Flags, ModuleFileName, ModuleHandle);
            else return res;
            
            if(NT_SUCCESS(res))
            {
                HookEngine.InstallHook((DWORD)GetProcAddress(GetModuleHandle(L"SSOCommon.dll"), "?TXLog_DoTXLogVW@@YAXPAUtagLogObj@@PB_W1PAD@Z"), (DWORD)TXLog_DoTXLogVW);
                HookEngine.InstallHook((DWORD)GetProcAddress(GetModuleHandle(L"SSOCommon.dll"), "?GetLogByFilter@TXLog@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@ABVCTXStringW@@K0K@Z"), (DWORD)TXLog_DoTXLogVW);
            }
            return res;
        }
    }

    if(s_pfnLdrLoadDll) return s_pfnLdrLoadDll(PathToFile, Flags, ModuleFileName, ModuleHandle);
    return 0;
}

typedef int (__cdecl* QQIsFileExist)(wchar_t const *);
QQIsFileExist MyIsFileExist = NULL;//跳转地址
int __cdecl IsFileExist(wchar_t const *path)
{
    //DbgPrint(path);
    if(isEndWith(path,L"skin.ini"))
    {
        //DbgPrint(L"%s",path);
        return 1;
    }
    if(MyIsFileExist) return MyIsFileExist(path);
    
    return 0;
}

int __cdecl IsTencentTrusted(void)
{
    return 1;
}
wchar_t MyAbout[MAX_PATH];
void ChangeMyAbout(long offset)
{
	if(*(BYTE*)offset==0x68)
	{
		DWORD addr = *(DWORD*)(offset+1);
		wchar_t * str = (wchar_t *)addr;
		wcscpy(MyAbout,str);
		wcscat(MyAbout,L" With CWUB3");
		//DbgPrint(L"%X %s",addr,MyAbout);

		addr = (DWORD)&MyAbout;
		WriteProcessMemory((void*)-1,(void*)(offset+1), &addr, sizeof(addr), NULL);
	}
}
void Miscellaneous()
{
    HookLib Common;
    Common.Load(L"Common.dll");
    
    HookEngine.InstallHook((DWORD)Common.GetFunc("?IsTencentTrusted@Misc@Util@@YAHPB_W@Z"), (DWORD)IsTencentTrusted);

    MyIsFileExist = (QQIsFileExist)HookEngine.InstallHook((DWORD)Common.GetFunc("?IsFileExist@FS@@YAHPB_W@Z"), (DWORD)IsFileExist);
    
    HookEngine.InstallHook((DWORD)Common.GetFunc("?TXLog_DoTXLogVW@@YAXPAUtagLogObj@@PB_W1PAD@Z"), (DWORD)TXLog_DoTXLogVW);
    HookEngine.InstallHook((DWORD)Common.GetFunc("?GetLogByFilter@TXLog@@YA?AV?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@ABVCTXStringW@@K0K@Z"), (DWORD)TXLog_DoTXLogVW);
    
    s_pfnLdrLoadDll = (PFN_LdrLoadDll)HookEngine.InstallHook((DWORD)GetProcAddress(GetModuleHandle(L"ntdll.dll"), "LdrLoadDll"), (DWORD)MyLdrLoadDll);

	BYTE key1[] = {0xC6, 0x45, 0xFC, 0x03, 0xFF, 0xD3, 0x8D, 0x8D};//C6 45 FC 06 FF D3 8D 8D
	long AFBase = MemorySearch(L"AFBase.dll",key1, sizeof(key1));
	if(AFBase!=-1)
	{
		ChangeMyAbout(AFBase-16);
	}
	/*
	else
	{
		BYTE key2[] = {0xC6, 0x45, 0xFC, 0x03, 0xFF, 0xD3, 0x8D, 0x4D, 0xA8};//C6 45 FC 03 FF D3 8D 4D A8
		long AFUtil = MemorySearch(L"AFUtil.dll",key2, sizeof(key2));
		if(AFUtil!=-1)
		{
			ChangeMyAbout(AFUtil-13);
		}
	}
	*/
	/*
	68802CFD  |.  68 24CC8568   PUSH OFFSET 6885CC24                     ; /Arg2 = UNICODE "QQ2012 Beta3"
	68802D02  |.  68 B4CC8568   PUSH OFFSET 6885CCB4                     ; |Arg1 = UNICODE "__PNAME_V_SV"
	68802D07  |.  8D8D 30FFFFFF LEA ECX,[LOCAL.52]                       ; |
	68802D0D  |.  C645 FC 06    MOV BYTE PTR SS:[LOCAL.1],6              ; |
	68802D11  |.  FFD3          CALL EBX                                 ; \Common.?PropertyStr@CFmtString@@QAEHPB_W0@Z
	68802D13  |.  8D8D 2CFFFFFF LEA ECX,[LOCAL.53]


	65B27C80  |.  68 2C74BA65     PUSH OFFSET 65BA742C                     ; UNICODE "QQ2012 Beta2"
	65B27C85  |.  68 B874BA65     PUSH OFFSET 65BA74B8                     ; UNICODE "__PNAME_V_SV"
	65B27C8A  |.  8D4D C8         LEA ECX,[LOCAL.14]
	65B27C8D  |.  C645 FC 06      MOV BYTE PTR SS:[LOCAL.1],6
	65B27C91  |.  FFD3            CALL EBX
	65B27C93  |.  8D4D C4         LEA ECX,[LOCAL.15]
	*/
}