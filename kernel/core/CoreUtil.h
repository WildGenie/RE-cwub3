/*
void DbgPrint(const wchar_t *format, ...)
{
	va_list argptr;
	TCHAR buffer[1024];
	va_start(argptr, format);
	_vsnwprintf(buffer, 1024, format, argptr);
	va_end(argptr);

	OutputDebugString(buffer);
}

bool IsVistaSystem(void) 
{ 
	static int result = -1;

	if(result==-1)
	{
		OSVERSIONINFO osvi; 
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		::GetVersionEx(&osvi); 
		if ( osvi.dwMajorVersion >= 6) result = 1; 
		else result = 0;
	}

	return result!=0; 
}
*/
bool IsSystemWin7()
{
	static int result = -1;

	if(result==-1)
	{
		OSVERSIONINFO osvi; 
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		::GetVersionEx(&osvi); 
		if (osvi.dwMajorVersion == 6 &&   
			osvi.dwMinorVersion >= 1 ) 
		{
			result = true; 
		}
		else result = false;

	}

	return result!=0; 
}

void WriteOffset(BYTE type,DWORD TargetProc, DWORD NewProc)
{
    //*(BYTE*)TargetProc = type;
	WriteProcessMemory((HANDLE)-1, (LPVOID)TargetProc, &type, sizeof(type), NULL);
    DWORD offset = NewProc - TargetProc - 5;
	//*(DWORD*)(TargetProc+1) = offset;
    WriteProcessMemory((HANDLE)-1, (LPVOID)(TargetProc + 1), &offset, sizeof(offset), NULL);
}

wchar_t * GetCallStack(void *param)
{
	static wchar_t stack[MAX_PATH+1];
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery((LPVOID)*((DWORD*)param-1), &mbi, sizeof(mbi));
	GetModuleFileName((HMODULE)mbi.AllocationBase, stack, MAX_PATH);

	//DbgPrint(L"%X",((DWORD*)param-1));
	return stack;
}

bool isEndWith(const wchar_t *path,const wchar_t* ext)
{
	if(!path || !ext) return false;
    int len1 = wcslen(path);
    int len2 = wcslen(ext);
    if(len2>len1) return false;
    return !_memicmp(path + len1 - len2,ext,len2*sizeof(wchar_t));
}
/*
bool isStartWith(const wchar_t *path,const wchar_t* ext)
{
    int len1 = wcslen(path);
    int len2 = wcslen(ext);
    if(len2>=len1) return false;
    return !_memicmp(path,ext,len2*sizeof(wchar_t));
}
*/

wchar_t* Ansi2Unicode(const char* szAnsi,int length = -1)
{
	if(length=-1) length = strlen(szAnsi);
	int wcsLen = ::MultiByteToWideChar(936, NULL, szAnsi, length, NULL, 0);
	wchar_t* wszString = (wchar_t*)malloc(sizeof(wchar_t) * (wcsLen + 1));
	::MultiByteToWideChar(936, NULL, szAnsi, length, wszString, wcsLen);
	wszString[wcsLen] = 0;
	return wszString;
}

char* Unicode2Ansi(const wchar_t* wszString)
{
	int ansiLen = ::WideCharToMultiByte(936, NULL, wszString, wcslen(wszString), NULL, 0, NULL, NULL);
	char* szAnsi = (char*)malloc(sizeof(char) * (ansiLen + 1));
	::WideCharToMultiByte(936, NULL, wszString, wcslen(wszString), szAnsi, ansiLen, NULL, NULL);
	szAnsi[ansiLen] = 0;
	return szAnsi;
}

wchar_t* UTF82Unicode(const char* szUTF8,int length)
{
	int wcsLen = ::MultiByteToWideChar(CP_UTF8, NULL, szUTF8, length, NULL, 0);
	wchar_t* wszString = (wchar_t*)malloc(sizeof(wchar_t) * (wcsLen + 1));
	::MultiByteToWideChar(CP_UTF8, NULL, szUTF8, length, wszString, wcsLen);
	wszString[wcsLen] = 0;
	return wszString;
}

void SetPrivateProfileInt(LPCTSTR lpAppName,LPCTSTR lpKeyName,INT nDefault,LPCTSTR lpFileName)
{
	TCHAR buffer[256];
	wsprintf(buffer,_T("%d"),nDefault);
	WritePrivateProfileString(lpAppName,lpKeyName, buffer, lpFileName);
}

wchar_t *replace(wchar_t *source,const wchar_t *sub,const wchar_t *rep)
{
	if (0 == source) return _wcsdup(L"");
	if (0 == *sub) return _wcsdup(source);


	wchar_t *pc1, *pc2;
	const wchar_t *pc3;

	int isub = wcslen(sub);
	int irep = wcslen(rep);
	int isource = wcslen(source);

	wchar_t* result = (wchar_t *)malloc(sizeof(wchar_t) * (( irep > isub ? isource / isub * irep : isource ) + 10) );
	pc1 = result;
	while (*source != 0)
	{
		pc2 = source;
		pc3 = sub;

		while (*pc2 == *pc3 && *pc3 != 0 && *pc2 != 0)
			pc2++, pc3++;
		// 寻找子串
		if (0 == *pc3)
		{
			pc3 = rep;
			//追加替代串到结果串
			while (*pc3 != 0)
				*pc1++ = *pc3++;
			pc2--;
			source = pc2;
		}
		else
			*pc1++ = *source;
		source++;
	}
	*pc1 = 0;
	return result;
}


typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;


int memstr(BYTE* p, int m,BYTE* s, int n)
{
    long mask;
    int skip;
    int i, j, mlast, w;

    w = n - m;

    if (w < 0)
        return -1;

    /* look for special cases */
    if (m <= 1)
    {
        if (m <= 0)//如果模式串为空
            return -1;
        /* use special case for 1-character strings */
        for (i = 0; i < n; i++)
            if (s[i] == p[0])
                return i;

        return -1;
    }

    mlast = m - 1;

    /* create compressed boyer-moore delta 1 table */
    skip = mlast - 1;
    /* process pattern[:-1] */
    for (mask = i = 0; i < mlast; i++)
    {
        mask |= (1 << (p[i] & 0x1F));
        if (p[i] == p[mlast])
            skip = mlast - i - 1;
    }

    /* process pattern[-1] outside the loop */
    mask |= (1 << (p[mlast] & 0x1F));

    for (i = 0; i <= w; i++)   // w == n - m;
    {
        /* note: using mlast in the skip path slows things down on x86 */
        if (s[i+m-1] == p[m-1])    //(Boyer-Moore式的后缀搜索)
        {
            /* candidate match */
            for (j = 0; j < mlast; j++)
                if (s[i+j] != p[j])
                    break;
            if (j == mlast) /* got a match! */
                return i;
            /* miss: check if next character is part of pattern */
            if (!(mask & (1 << (s[i+m] & 0x1F))))  //(Sunday式的基于末字符的下一字符)
                i = i + m;
            else
                i = i + skip; //(Horspool式的基于末字符)
        }
        else
        {
            /* skip: check if next character is part of pattern */
            if (!(mask & (1 << (s[i+m] & 0x1F))))
                i = i + m;
        }
    }
    return -1;
}


bool CheckPE(void *image)
{
    PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)image;
    if(pDosHeader->e_magic!=IMAGE_DOS_SIGNATURE)
    {
        //不是有效的DOS文件
        return false;
    }
    PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader+pDosHeader->e_lfanew);//指向pNtHeader的指针
    if ( pNtHeader->Signature != IMAGE_NT_SIGNATURE )
    {
        //不是有效的NT文件
        return false;
    }
    //检查完成
    //OutputDebugStringA("CheckPE");
    return true;
}

long MemorySearch(const wchar_t *path,BYTE* key, int length)
{
    //OutputDebugStringA("MemorySearch");
	/*
    if(CheckSigned && VerifySignature(path)==0)
    {
        //OutputDebugString(path);
        return -1;
    }
	*/
    //OutputDebugStringA("VerifySignature");
    HMODULE m_hModule = GetModuleHandle(path);
    if(!m_hModule) m_hModule = LoadLibrary(path);
    if(m_hModule)
    {
        //if(CheckPE(m_hModule))
        {
            PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)m_hModule;
            PIMAGE_NT_HEADERS pNtHeader = (PIMAGE_NT_HEADERS)((DWORD)pDosHeader+pDosHeader->e_lfanew);
            PIMAGE_FILE_HEADER pPEHead=(PIMAGE_FILE_HEADER)&pNtHeader->FileHeader;
            PIMAGE_SECTION_HEADER section = (PIMAGE_SECTION_HEADER)((DWORD)pPEHead + sizeof(IMAGE_FILE_HEADER) + pPEHead->SizeOfOptionalHeader);
            for(int i=0; i<pPEHead->NumberOfSections; i++)
            {
                if(strcmp((const char*)section[i].Name,".text")==0)
                {
                    long res = memstr(key,length,(BYTE*)((DWORD)m_hModule + section[i].PointerToRawData),section[i].SizeOfRawData);
                    if(res!=-1)
                    {
                        //OutputDebugStringA("memstr");
                        //DbgPrint(L"%s %X",path,res+(DWORD)m_hModule + section[i].PointerToRawData);
                        return res+(DWORD)m_hModule + section[i].PointerToRawData;
                    }
                    break;
                }
            }
        }
    }
    return -1;
}