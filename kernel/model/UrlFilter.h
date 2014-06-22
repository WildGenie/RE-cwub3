typedef int (__stdcall * SEND) (SOCKET s, char FAR *buf, int len, int flags);
SEND SendOldProc = NULL;
int WINAPI MySend(SOCKET s, char FAR *buf, int len, int flags)
{
    if(len>10 && ( (buf[0]=='G' && buf[1]=='E' && buf[2]=='T' && buf[3]==' ') || (buf[0]=='P' && buf[1]=='O' && buf[2]=='S' && buf[3]=='T' && buf[4]==' ') ) )
    {
        wchar_t* wszString = Ansi2Unicode(buf,len);
        wchar_t* end = wcsstr(wszString,L"\r\n\r\n");
        if(end)
        {
            end[0] = '\n';
            end[1] = 0;
        }
        
		if( wcsstr(wszString,L"mini.group.qq.com")!=0 || 
			wcsstr(wszString,L"mini2.qun.qq.com")!=0 
			|| wcsstr(wszString,L"fodder.qq.com")!=0 
			|| wcsstr(wszString,L"adv")!=0 
         || wcsstr(wszString,L"qqdoctor")!=0 
         || wcsstr(wszString,L"Update")!=0 
        )
        {
            free(wszString);
			//WSASetLastError(WSAENETRESET);
            return SOCKET_ERROR;
        }
        
        free(wszString);
    }
    if(SendOldProc) return SendOldProc(s,buf,len,flags);
    
    return 0;
}