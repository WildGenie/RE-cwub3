/*
  HOOKLIB by shuax
  2012.03.17
*/
#pragma once

#ifndef __HOOKLIB_H__
#define __HOOKLIB_H__

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#include <windows.h>
#include <malloc.h>
//#include "HookTable.h"
#include "DisassembleProlog.h"


#pragma pack(1) //��֤����1�ֽڶ���
struct FILL_CODE
{
    BYTE push_code;
    DWORD address;
    BYTE retn_code;
}PUSH_CODE;
#pragma pack(4)
/*
struct Tag_Hook
{
    DWORD orig;
    DWORD det;
};
Tag_Hook TagHook[100];
int HookCount = 0;
*/
class HookEngine
{
public:
    HookEngine()
    {
        //��ʼ��
        PUSH_CODE.push_code = 0x68;
        PUSH_CODE.retn_code = 0xC3;
        
        nowLength = 0;
        SaveHook = VirtualAlloc(NULL, 1024, MEM_COMMIT, PAGE_EXECUTE_READWRITE);//���ݳ��ȡ�����ָ�JMPָ��
        /*
        UInt32 outSize = 0; 
        BYTE *outStream = 0;
        if(LZMADec(HookTable,sizeof(HookTable),&outStream,&outSize,0)==LZMA_RESULT_OK)
        {
            tables = (DWORD*)outStream;
            
        }
        */
    }
    
    void* InstallHook(DWORD orig, DWORD det)
    {
		//DbgPrint(L"InstallHook1 %X,%X",orig,det);
        //��鴫�����
        if(orig==0 || det==0) return 0;

        //����ָ��
        void* ptr;
        ptr = ((BYTE*)SaveHook) + nowLength;
        //�����Ҫ����ĳ���
        int ThunkLen = GetPatchLength(orig,ptr,sizeof(PUSH_CODE));

        if(ThunkLen == 0) return 0;
		
		/*
        if(HookCount<100)
        {
            TagHook[HookCount].orig = orig;
            TagHook[HookCount].det  = det;
            HookCount++;
        }
		*/
        
        nowLength += ThunkLen;
        if( nowLength > 1024 ) return 0;
        
        
        //����ԭʼ��ַ��������
        WritePUSH_RET((DWORD)ptr + ThunkLen - sizeof(PUSH_CODE),orig + ThunkLen - sizeof(PUSH_CODE) - 1);
        
        //д����ת���³���
        WritePUSH_RET(orig,det);

        //�Զ����ֽ����NOP
        if(ThunkLen>13) WriteNOP(orig+6,ThunkLen-13);
        
        *(BYTE*)ptr = ThunkLen;
        ptr = (BYTE*)ptr + 1;

		//OutputDebugString(L"InstallHook2");
        return ptr;
    }
	
	template <class T>
    void Uninstallhook(T t)
    {
		union
		{
			DWORD  _addr;
			T  _t;
		} tmp;
		tmp._t = t;
	
		//��鴫�����
		if(tmp._addr==0) return;

		DWORD ptr = tmp._addr - 1;

        int MinLen = *(BYTE*)ptr;

        DWORD base = *(DWORD*)(ptr+MinLen-5) - (MinLen-7);// + 5 + (DWORD)ptr;
        //DbgPrint(L"%X %X",base,(DWORD)ptr);//[776] 7785C43A 6726E1
        
		//memcpy((LPVOID)base, (LPVOID)(ptr + 1), MinLen - 7);
		WriteProcessMemory((void*)-1,(LPVOID)base, (LPVOID)(ptr + 1), MinLen - 7, NULL);
        //WriteProcessMemory((void*)-1,(LPVOID)base, ptr, MinLen, NULL);
        memset((void*)ptr,0xC3,MinLen);
        //free();
    }
private:
    void* SaveHook;
    int nowLength;
    
    int VAtoFileOffset(void *pModuleBase, void *pVA)
    {
        return (DWORD)pVA - (DWORD)pModuleBase;
    }
    void WritePUSH_RET(DWORD TargetProc, DWORD NewProc)
    {
        //д��һ�� 68 XXXXXXXX C3
        PUSH_CODE.address = NewProc;
        //memcpy((LPVOID)TargetProc, &PUSH_CODE, sizeof(PUSH_CODE));
		WriteProcessMemory((void*)-1,(LPVOID)TargetProc, &PUSH_CODE, sizeof(PUSH_CODE), NULL);
    }
    void WriteNOP(DWORD TargetProc,int len)
    {
        //���NOP�����㷴���۲�
        static BYTE NOP[] = {0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90,0x90};
        WriteProcessMemory((void*)-1,(LPVOID)TargetProc, &NOP, len, NULL);
		//memset((void*)TargetProc,0x90,len);
    }
    

    bool AntiHook(DWORD orig,int length,void* bak)
    {
        wchar_t stack[MAX_PATH + 1];
        MEMORY_BASIC_INFORMATION mbi;
        VirtualQuery((LPVOID)orig, &mbi, sizeof(mbi));

        GetModuleFileName((HMODULE)mbi.AllocationBase, stack, MAX_PATH);
        
        HMODULE h_module = GetModuleHandle(stack);
        DWORD offset = VAtoFileOffset(h_module,(void*)orig);

        HANDLE hfile = CreateFile(stack,
            GENERIC_READ,
            FILE_SHARE_READ, NULL, OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL, NULL);
        if (hfile != INVALID_HANDLE_VALUE)
        {
            HANDLE hfilemap = CreateFileMapping(hfile, NULL, PAGE_READONLY|SEC_IMAGE, 0, 0, NULL);
            CloseHandle(hfile);

            unsigned char *buf = (unsigned char*) MapViewOfFile(hfilemap, FILE_MAP_READ, 0, 0, 0);
            CloseHandle(hfilemap);
        
            memcpy(bak,buf+offset,length);
            UnmapViewOfFile(buf);

            return true;
        }
        return false;
    }
    
    int GetPatchLength(DWORD func_start, void* thunk, int max_need_len=6)
    {
        BYTE temp[100];
        if(!AntiHook(func_start,100,temp)) return 0;

        int actual_oplen = DisassembleProlog((PBYTE)temp,max_need_len);
        
        if(actual_oplen==0) return 0;
        
        //*(BYTE*)thunk = actual_oplen;
        
        thunk = (BYTE*)thunk + 1;
        memcpy(thunk,temp,actual_oplen);
        
        actual_oplen = actual_oplen + 1 + max_need_len;
         
        return actual_oplen;
    }
} HookEngine;


class HookLib
{
private:
    HMODULE m_hModule;  // ԭʼģ����
public:
    void Load(const TCHAR *tzDllPath);
    FARPROC GetFunc(PCSTR pszProcName);
};
void HookLib::Load(const TCHAR *tzDllPath)
{
    m_hModule = NULL;
    m_hModule = GetModuleHandle(tzDllPath);
    if(!m_hModule) m_hModule = LoadLibrary(tzDllPath);
}
FARPROC HookLib::GetFunc(PCSTR pszProcName)
{
    FARPROC fpAddress = GetProcAddress(m_hModule, pszProcName);
    //DbgPrint(L"��ȡ��ַ��%08X",fpAddress);
    return fpAddress;
}
/*
template <typename Type>
void GetFuncToProc(Type t,const TCHAR *tzPath,PCSTR pszProcName)
{
    union
    {
        DWORD  _addr;
        Type  _t;
    } tmp;
    tmp._t = t;
    
    HookLib temp;
    temp.Load(tzPath);
    tmp._addr = (DWORD)temp.GetFunc(pszProcName);
}

//��װ�麯��hook
template <typename Type, typename Class>
DWORD VirtualHook(Type t, Class c,DWORD newAddr)
{
    union
    {
        DWORD   _addr;
        Type  _t;
    } tmp;
    tmp._t = t;
    DWORD* oldAddr = (DWORD*)*(DWORD*)c + (tmp._addr-1)/4;
    DWORD oldFunc = *oldAddr;
    
    DWORD dwOldProtect;
    if(::VirtualProtect((LPVOID)oldAddr, sizeof(DWORD), PAGE_EXECUTE_READWRITE, &dwOldProtect))
    {
        DbgPrint(L"%X\n",oldFunc);
        *oldAddr = newAddr;
        ::VirtualProtect((LPVOID)oldAddr, sizeof(DWORD), dwOldProtect, 0);
        return oldFunc;
    }

    
    return (DWORD)InstallHook(oldFunc,newAddr);
    
}

//��װ��ͨ��Ա����hook
template <typename Type, typename Class>
void* ClassHook(Type t, Class c,DWORD newAddr)
{
    union
    {
        DWORD   _addr;
        Type  _t;
    } tmp;
    tmp._t = t;
    return InstallHook(tmp._addr,newAddr);
}
*/
#endif // __HOOKLIB_H__
