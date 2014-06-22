
void RemoveCheck()
{
    //去除安全提示
    BYTE key10[] = {0x83, 0xF8, 0x02, 0x8B, 0x37, 0x75, 0x3B};//83 F8 02 8B 37 75 3B
    long AFUtil = MemorySearch(L"AFUtil.dll",key10, sizeof(key10));
    if(AFUtil!=-1)
    {
        BYTE patch[] = {0xE9, 0x81, 0x00, 0x00, 0x00, 0x90};//E9 81 00 00 00 90
        WriteProcessMemory((void*)-1,(void*)(AFUtil+5), &patch, sizeof(patch), NULL);
    }
    
    //去校验之MsgMgr.dll
    BYTE key1[] = {0x83, 0xC4, 0x28, 0x3B, 0xC3, 0x74, 0x75};//83 C4 28 3B C3 74 75
    long MsgMgr = MemorySearch(L"MsgMgr.dll",key1, sizeof(key1));
    if(MsgMgr!=-1)
    {
        BYTE patch = 0xEB;
        WriteProcessMemory((void*)-1,(void*)(MsgMgr+5), &patch, sizeof(patch), NULL);
    }
    
    //去校验之AppUtil.dll
    BYTE key2[] = {0x3B, 0xC6, 0x74, 0x7A};//3B C6 74 7A
    long AppUtil = MemorySearch(L"AppUtil.dll",key2, sizeof(key2));
    if(AppUtil!=-1)
    {
        BYTE patch = 0xEB;
        WriteProcessMemory((void*)-1,(void*)(AppUtil+2), &patch, sizeof(patch), NULL);
        //第二次搜索
        AppUtil = MemorySearch(L"AppUtil.dll",key2, sizeof(key2));
        if(AppUtil!=-1)
        {
            WriteProcessMemory((void*)-1,(void*)(AppUtil+2), &patch, sizeof(patch), NULL);
        }
    }
    
    //去除勿使用外挂提示
    BYTE bSafeTips[] = {0x83, 0xC4, 0x04, 0x3B, 0xC6, 0x0F, 0x84};//83 C4 04 3B C6 0F 84 17
    long iSafeTips = MemorySearch(L"AppFramework.dll",bSafeTips, sizeof(bSafeTips));
    if(iSafeTips!=-1)
    {
        unsigned short patch = 0xE990;
        WriteProcessMemory((void*)-1,(void*)(iSafeTips+5), &patch, sizeof(patch), NULL);
    }
    
    /*
    BYTE keySkin[] = {0x85, 0xC0, 0x74, 0x5C, 0xC7, 0x45};//85 C0 74 5C C7 45
    long SkinMgr = MemorySearch(L"SkinMgr.dll",keySkin, sizeof(keySkin));
    if(SkinMgr!=-1)
    {
        BYTE patch = 0xEB;
        WriteProcessMemory((void*)-1,(void*)(SkinMgr+2), &patch, sizeof(patch), NULL);
    }
    
    
    BYTE keyCommon[] = {0x84, 0xC0, 0x74, 0x68, 0x8D, 0x8D};//84 C0 74 68 8D 8D
    long Common = MemorySearch(L"Common.dll",keyCommon, sizeof(keyCommon));
    if(Common!=-1)
    {
        BYTE patch = 0xEB;
        WriteProcessMemory((void*)-1,(void*)(Common+2), &patch, sizeof(patch), NULL);
    }
    */

	/*
	*/
    BYTE keyCommon[] = {0xFF, 0xD6, 0x83, 0xF8, 0xFF, 0x8D};//FF D6 83 F8 FF 8D
    long Common = MemorySearch(L"Common.dll",keyCommon, sizeof(keyCommon));
    if(Common!=-1)
    {
		WORD patch = 0x9090;
		WriteProcessMemory((void*)-1,(void*)(Common + 8), &patch, sizeof(patch), NULL);

		BYTE patch2 = 0x06;
		WriteProcessMemory((void*)-1,(void*)(Common + 13), &patch2, sizeof(patch2), NULL);
	}

    BYTE key3[] = {0xE9, 0xF2, 0xFE, 0xFF, 0xFF};//E9 F2 FE FF FF
    long ChatFrameApp = MemorySearch(L"ChatFrameApp.dll",key3, sizeof(key3));
    if(ChatFrameApp!=-1)
    {
        WORD patch = 0xF9EB;
        WriteProcessMemory((void*)-1,(void*)(ChatFrameApp+5), &patch, sizeof(patch), NULL);
    }
    
    BYTE pluglist[] = {0x8B, 0x75, 0x08, 0x56, 0x8D, 0x55, 0xD4};// 8B 75 08 56 8D 55 D4
    long ChatFrameApp2 = MemorySearch(L"ChatFrameApp.dll",pluglist, sizeof(pluglist));
    if(ChatFrameApp2!=-1)
    {
        BYTE patch[5];
        memset(patch,0x90,sizeof(patch));
        WriteProcessMemory((void*)-1,(void*)(ChatFrameApp2+12), &patch, sizeof(patch), NULL);
    }
    
    BYTE key4[] = {0x83, 0x7E, 0x70, 0x00, 0x75, 0x36};//83 7E 70 00 75 36
    long IM = MemorySearch(L"IM.dll",key4, sizeof(key4));
    if(IM!=-1)
    {
        BYTE patch = 0xEB;
        WriteProcessMemory((void*)-1,(void*)(IM+4), &patch, sizeof(patch), NULL);
    }
    /*
    BYTE IM1[] = {0x74, 0x1A, 0x8B, 0xCF, 0xFF, 0xD6};//74 1A 8B CF FF D6
    long IM_1 = MemorySearch(L"IM.dll",IM1, sizeof(IM1));
    if(IM_1!=-1)
    {
        BYTE patch = 0xEB;
        WriteProcessMemory((void*)-1,(void*)(IM_1), &patch, sizeof(patch), NULL);
    }
    */
    
    BYTE key5[] = {0x52, 0x8D, 0x45, 0xBC, 0x50};// 52 8D 45 BC 50
    long AppMisc = MemorySearch(L"AppMisc.dll",key5, sizeof(key5));
    if(AppMisc!=-1)
    {
        BYTE patch[5];
        memset(patch,0x90,sizeof(patch));
        WriteProcessMemory((void*)-1,(void*)(AppMisc+5), &patch, sizeof(patch), NULL);
    }
    
    /*
    BYTE AppMisc1[] = {0x74, 0x42, 0x3B, 0xC3, 0x74, 0x3E};//74 42 3B C3 74 3E
    long AppMisc_1 = MemorySearch(L"AppMisc.dll",AppMisc1, sizeof(AppMisc1));
    if(AppMisc_1!=-1)
    {
        BYTE patch[5];
        memset(patch,0x90,5);
        WriteProcessMemory((void*)-1,(void*)(AppMisc_1-26), &patch, sizeof(patch), NULL);
    }

    BYTE AppMisc2[] = {0x75, 0x5C, 0x6A, 0x65, 0x56, 0x68};//75 5C 6A 65 56 68
    long AppMisc_2 = MemorySearch(L"AppMisc.dll",AppMisc2, sizeof(AppMisc2));
    if(AppMisc_2!=-1)
    {
        unsigned short patch = 0x6DEB;
        WriteProcessMemory((void*)-1,(void*)(AppMisc_2), &patch, sizeof(patch), NULL);
    }
    */
    /*
    BYTE key8[] = {0xB9, 0x29, 0x01, 0x00, 0x00, 0x51};//B9 29 01 00 00 51
    long MainFrame = MemorySearch(L"MainFrame.dll",key8, sizeof(key8));
    if(MainFrame!=-1)
    {
        unsigned short patch = 0x0104-10;
        WriteProcessMemory((void*)-1,(void*)(MainFrame+1), &patch, sizeof(patch), NULL);
    }
    */
    /*
    BYTE key9[] = {0x89, 0x50, 0x10, 0xC7, 0x40, 0x18};//89 50 10 C7 40 18
    long LoginPanel = MemorySearch(L"LoginPanel.dll",key9, sizeof(key9));
    if(LoginPanel!=-1)
    {
        unsigned short patch = 0x00FA-10;
        WriteProcessMemory((void*)-1,(void*)(LoginPanel+6), &patch, sizeof(patch), NULL);
    }
    */
    BYTE key12[] = {0xC7, 0x40, 0x18, 0x27, 0x01, 0x00, 0x00};//C7 40 18 27 01 00 00
    int LoginPanel = MemorySearch(L"LoginPanel.dll",key12, sizeof(key12));
    if(LoginPanel!=-1)
    {
        unsigned short patch = 0x00FA;
        WriteProcessMemory((void*)-1,(void*)(LoginPanel+3), &patch, sizeof(patch), NULL);
        
        patch = 0x01DB;
        WriteProcessMemory((void*)-1,(void*)(LoginPanel+18), &patch, sizeof(patch), NULL);
    }

    BYTE key6[] = {0x66, 0x3D, 0x14, 0x00, 0x0F, 0x85};//66 3D 14 00 0F 85
    BYTE key7[] = {0xC2, 0x0C, 0x00, 0x8D, 0x4D, 0xD4};//C2 0C 00 8D 4D D4
    long SNSApp1 = MemorySearch(L"..\\Plugin\\Com.Tencent.SNSApp\\Bin\\SNSApp.dll",key6, sizeof(key6));
    long SNSApp2 = MemorySearch(L"..\\Plugin\\Com.Tencent.SNSApp\\Bin\\SNSApp.dll",key7, sizeof(key7));
    //DbgPrint(L"%d %d:%d",SNSApp1,SNSApp2,SNSApp2-SNSApp1);
    if(SNSApp1!=-1 && SNSApp2!=-1 && SNSApp2>SNSApp1 && SNSApp2-SNSApp1<0x400)
    {
        WriteOffset(0xE9,SNSApp1 + 4,SNSApp2 + 3);
    }

    /*
    BYTE keyQQ[] = {0x85, 0xFF, 0x74, 0x5C, 0x8D}; // 85 FF 74 5C 8D
    long QQProtectPatch = MemorySearch(L"QQ.exe",keyQQ, sizeof(keyQQ));
    if(QQProtectPatch!=-1)
    {
        unsigned short patch = 0x9090;
        WriteProcessMemory((void*)-1,(void*)(QQProtectPatch+2), &patch, sizeof(patch), NULL);
    }
    */
    
    BYTE DelMsimg32[] = {0x33, 0xC9, 0x83, 0xF8, 0xFF}; // 33 C9 83 F8 FF
    long QQDelPatch = MemorySearch(L"QQ.exe",DelMsimg32, sizeof(DelMsimg32));
    if(QQDelPatch==-1)
    {
        QQDelPatch = MemorySearch(L"HummerEngine.dll",DelMsimg32, sizeof(DelMsimg32));
    }
    if(QQDelPatch!=-1)
    {
        unsigned short patch = 0xE990;
        WriteProcessMemory((void*)-1,(void*)(QQDelPatch+12), &patch, sizeof(patch), NULL);
    }

    BYTE DelMsimg32_new[] = {0x85, 0xC0, 0x75, 0x12}; // 85 C0 75 12
    long QQDelPatch_new = MemorySearch(L"HummerEngine.dll",DelMsimg32_new, sizeof(DelMsimg32_new));
    if(QQDelPatch_new!=-1)
    {
        unsigned short patch = 0x42EB;
        WriteProcessMemory((void*)-1,(void*)(QQDelPatch_new+2), &patch, sizeof(patch), NULL);
    }

    //去除微博广告
    BYTE WBlogKey[] = {0x8B, 0x4D, 0x10, 0x53, 0x89, 0x06}; // 8B 4D 10 53 89 06
    long WBlog = MemorySearch(L"..\\Plugin\\Com.Tencent.WBlog\\Bin\\WBlog.dll", WBlogKey, sizeof(WBlogKey));
    if(WBlog!=-1)
    {
        unsigned short patch = 0xE990;
        WriteProcessMemory((void*)-1,(void*)(WBlog-6), &patch, sizeof(patch), NULL);
    }
}