//伪造版本号
/*
CPU Disasm
Address   Hex dump          Command                                                                                    Comments
69E100ED  |.  8BF8          MOV EDI,EAX
69E100EF  |.  FF15 9880E169 CALL DWORD PTR DS:[<&Common.??1CTXBSTR@@QAE@XZ>]
69E100F5  |.  85FF          TEST EDI,EDI

69E100F7      EB 61         JMP SHORT 69E1015A

69E100F9  |.  FF75 F0       PUSH DWORD PTR SS:[EBP-10]                                                                 ; Arg1 => [ARG.EBP-10]
69E100FC  |.  8BCE          MOV ECX,ESI
69E100FE  |.  E8 10E6FFFF   CALL 69E0E713
69E10103  |.  8BCE          MOV ECX,ESI
69E10105  |.  E8 53F8FFFF   CALL 69E0F95D                                                                              ; [HummerEngine.69E0F95D



CPU Disasm
Address   Hex dump          Command                                           Comments
65149175  |.  FF15 44041865 CALL DWORD PTR DS:[<&KERNEL32.GetModuleFileNameW> ; \KERNEL32.GetModuleFileNameW

*/
void ChangeVersion(void)
{
    BYTE code1[] = {0xB8,0xB1,0x65,0x00,0x00,0xC3};//B8 ED 64 00 00 C3        65 B1
    WriteProcessMemory((void*)-1, (LPVOID)KernelUtil.GetFunc("?GetPubNo@Version@@YAKXZ"), &code1, sizeof(code1), NULL);

    if(FakeVer==2)
    {
        BYTE code2[] = {0xB0,0x2F,0x90,0x90,0x90,0xC3};//B0 26 90 90 90 C3       2F 59
        WriteProcessMemory((void*)-1, (LPVOID)KernelUtil.GetFunc("?GetMajorVer@Version@@YAEXZ"), &code2, sizeof(code2), NULL);

        code2[1]= 0x59;
        WriteProcessMemory((void*)-1, (LPVOID)KernelUtil.GetFunc("?GetMinorVer@Version@@YAEXZ"), &code2, sizeof(code2), NULL);
    }
}

typedef unsigned long (__cdecl * MYGETSELFUIN) (void);
MYGETSELFUIN GetSelfUin = 0;

// 本地会员
typedef int (__cdecl * MYISFLAGVALID) (unsigned long, unsigned long);
MYISFLAGVALID VipOldProc = NULL;
int __cdecl MyIsFlagValid(unsigned long QQUIN, unsigned long Flag)
{
	if ( Flag == 4 && LocalVIP && QQUIN == GetSelfUin() && !isEndWith(GetCallStack(&QQUIN),L"MsgMgr.dll"))
	{
		return 1;
	}
	// 调用原来的函数 
	if(VipOldProc) return VipOldProc(QQUIN, Flag);
	
	return 0;
}


// 获取服务器开关
typedef int (__cdecl * QQIsServerControlBitOn) (unsigned long);
QQIsServerControlBitOn OldIsServerControlBitOn = NULL;
int __cdecl IsServerControlBitOn(unsigned long FLAG)
{
	switch(FLAG)
	{
		//case 46:	return 0;
		//case 48:	return 0;
		case 131:	return 0;//QQ秀
		//case 163:	return 1;//升级
		//case 166:	return 1;
	}
	
	int ret = 0;
	//if(isEndWith(GetCallStack(&FLAG),L"QQShow.dll")) return 0;
	if(OldIsServerControlBitOn) ret = OldIsServerControlBitOn(FLAG);
	
	//DbgPrint(L"%d:%d %s",FLAG,ret,GetCallStack(&FLAG));
	return ret;
}

//显示隐藏图标
typedef int (__cdecl * QQIsMaskFlagValid) (unsigned long,unsigned char,unsigned long,int &);
QQIsMaskFlagValid OldIsMaskFlagValid = NULL;
long __cdecl IsMaskFlagValid(unsigned long qq,unsigned char p2,unsigned long p3,int &p4)
{
	long res = 0;
	if(OldIsMaskFlagValid)
	{
		res = OldIsMaskFlagValid(qq,p2,p3,p4);
		if(ShowHideicon && qq != GetSelfUin())
		{
			p4 = 0;
		}
	}
	return res;
}

//外置看图器
typedef int (__stdcall * QQImageView) (DWORD p1,wchar_t *p2);
QQImageView ImageView = NULL;
int __stdcall MyImageView(DWORD p1,wchar_t *p2)
{
    DWORD ret = (DWORD)ShellExecute(NULL, L"open", p2, NULL, NULL, SW_SHOWNORMAL);
    if(ret<32)
    {
        if(ImageView) return ImageView(p1,p2);
    }

    return 0;
}

// 无限发图
int __cdecl CheckMsgImage(DWORD p1, DWORD p2)
{
	return 1;
}

void localVIP()
{
    KernelUtil.Load(L"KernelUtil.dll");
	GetSelfUin = (MYGETSELFUIN)KernelUtil.GetFunc("?GetSelfUin@Contact@Util@@YAKXZ");
    
    //版本号伪装
    if(FakeVer)
    {
        ChangeVersion();
    }
    
	//本地会员
	VipOldProc = (MYISFLAGVALID)HookEngine.InstallHook((DWORD)KernelUtil.GetFunc("?IsFlagValid@Contact@Util@@YAHKK@Z"), (DWORD)MyIsFlagValid);
	OldIsMaskFlagValid = (QQIsMaskFlagValid)HookEngine.InstallHook((DWORD)KernelUtil.GetFunc("?IsMaskFlagValid@Contact@Util@@YAJKEKAAH@Z"), (DWORD)IsMaskFlagValid);
    
	HookEngine.InstallHook((DWORD)KernelUtil.GetFunc("?CheckMsgImage@Group@Util@@YAHPAUITXMsgPack@@AAVCTXStringW@@@Z"), (DWORD)CheckMsgImage);
	OldIsServerControlBitOn = (QQIsServerControlBitOn)HookEngine.InstallHook((DWORD)KernelUtil.GetFunc("?IsServerControlBitOn@Misc@Util@@YAHK@Z"), (DWORD)IsServerControlBitOn);
    
    //图片查看器
    if( !ShowImageView )
    {
        BYTE imagekey[] = {0x8B, 0x5D, 0x0C, 0x56, 0x57, 0x53, 0x68};//8B 5D 0C 56 57 53 68
        long imageview = MemorySearch(L"AppMisc.dll",imagekey, sizeof(imagekey));

        //DbgPrint(L"%X",imageview);
        if(imageview!=-1)
        {
			imageview -= 4;
            //DbgPrint(L"%X",*(DWORD*)(imageview-4));
            if( *(unsigned short*)imageview == 0x8B55 )
            {
                ImageView = (QQImageView)HookEngine.InstallHook(imageview,(DWORD)MyImageView);
            }
        }
    }
}