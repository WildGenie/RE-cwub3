void InjectDll(HANDLE hProcess,HANDLE hThread,TCHAR *tzPath)
{
    // 获取函数入口点
    CONTEXT cn;
    cn.ContextFlags = CONTEXT_INTEGER;
    ::GetThreadContext(hThread,&cn);
    
    //检查入口点是否是E8
    BYTE read[5];
    ReadProcessMemory(hProcess,(LPVOID)cn.Eax,read,sizeof(read),0);
    if(read[0]==0xE8)
    {
        LPVOID pAddr = VirtualAllocEx(hProcess,NULL,1024,MEM_COMMIT,PAGE_EXECUTE_READWRITE);

        DWORD des = (DWORD)cn.Eax + *(DWORD *)&read[1] + 5;
        
        int len_str = (_tcslen(tzPath) + 1)*sizeof(TCHAR);

        //写入dll路径
        ::WriteProcessMemory(hProcess, pAddr, tzPath, len_str, NULL);
        
        //写入GetKernelBase函数
        const BYTE GetKernelBase[] ={0x56, 0x31, 0xC9, 0x64, 0x8B, 0x71, 0x30, 0x8B, 0x76, 0x0C, 0x8B, 0x76, 0x1C, 0x8B, 0x46, 0x08, 0x8B, 0x7E, 0x20, 0x8B, 0x36, 0x66, 0x39, 0x4F, 0x18, 0x75, 0xF2, 0x5E, 0xC3};
        ::WriteProcessMemory(hProcess, (LPVOID)((DWORD)pAddr + len_str), GetKernelBase, sizeof(GetKernelBase), NULL);

        BYTE ShellCode[] =
        {
            0x68,0x90,0x90,0x90,0x90,	//push patch

            0x68,0x90,0x90,0x90,0x90,	//push Return

            0xE8,0xD4,0xFF,0xFF,0xFF,	//call GetKernelBase
            0x05,0x90,0x90,0x90,0x90,	//add eax,xxxx

            0xFF,0xE0,					//JMP eax,LoadLibrary
        };

        HMODULE kernel32 = GetModuleHandle(L"Kernel32.dll");
        *(DWORD *)&ShellCode[1] = (DWORD)pAddr;
        *(DWORD *)&ShellCode[6] = des;
        *(DWORD *)&ShellCode[16] = (DWORD)GetProcAddress(kernel32, "LoadLibraryW") - (DWORD)kernel32; //(DWORD)LoadLibraryW - (DWORD)pAddr - len_str - 5 - 7;
        
        //写入LoadLibrary
        ::WriteProcessMemory(hProcess,(LPVOID)((DWORD)pAddr + len_str + sizeof(GetKernelBase)),ShellCode,sizeof(ShellCode),NULL);

        BYTE JmpCode[] =
        {
            0xE8,0x90,0x90,0x90,0x90,	//call xxxx
        };
        
        *(DWORD *)&JmpCode[1] = (DWORD)pAddr + len_str + sizeof(GetKernelBase) - (DWORD)cn.Eax - 5;
        
        //写入主函数跳转
        ::WriteProcessMemory(hProcess,(LPVOID)cn.Eax,&JmpCode,sizeof(JmpCode),NULL);
    }
}