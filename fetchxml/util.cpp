#include "util.h"
#include "stdafx.h"
#include "tlhelp32.h"

HANDLE dcreateprocess(char* cmd)
{
	STARTUPINFO si = { sizeof(STARTUPINFO) };
	PROCESS_INFORMATION pi = { 0 };
	int nProcess = 1;
	DWORD dwRet = 1;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.lpTitle = NULL;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.lpReserved = NULL;
	si.cbReserved2 = 0;
	si.lpReserved2 = NULL;
	BOOL bRet = CreateProcess( NULL, cmd, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL,  NULL, &si, &pi );
	if(bRet){
		//::CloseHandle(pi.hProcess);
		//return (pi.dwProcessId);
		::CloseHandle(pi.hThread);
		return pi.hProcess;
	}
	return 0;
}

void GetExePath(char* process)
{
#ifdef _WIN32

	GetModuleFileName(NULL,process,MAX_PATH);

    for(int i=(int)_tcslen(process);i>0;i--)
	{
		if( process[i] =='\\')
		{
			process[i+1]='\0';
			break;
		}
	}
#else
	getcwd(process, 256);
#endif
}

#if !defined(_WIN32) || _MSC_VER<=1200

int strcpy_s(char *_DEST, size_t _SIZE, const char *_SRC)
{
	if (_DEST == NULL || _SRC == NULL || _SIZE == 0)
		return -1;
	char *p;
    size_t available;

    /* validation section */
    //_VALIDATE_STRING(_DEST, _SIZE);
    //_VALIDATE_POINTER_RESET_STRING(_SRC, _DEST, _SIZE);

    p = _DEST;
    available = _SIZE;
    while ((*p++ = *_SRC++) != 0 && --available > 0)
    {
    }

    if (available == 0)
    {
        //_RESET_STRING(_DEST, _SIZE);
        //_RETURN_BUFFER_TOO_SMALL(_DEST, _SIZE);
        _DEST[_SIZE-1] = 0;
        return 2;
    }
    //_FILL_STRING(_DEST, _SIZE, _SIZE - available + 1);
    //_RETURN_NO_ERROR;
    return 0;
}
int strcat_s(char *_DEST, size_t _SIZE, const char *_SRC)
{
	if (_DEST == NULL || _SRC == NULL || _SIZE == 0)
		return -1;
    char *p;
    size_t available;

    /* validation section */
    //_VALIDATE_STRING(_DEST, _SIZE);
    //_VALIDATE_POINTER_RESET_STRING(_SRC, _DEST, _SIZE);

    p = _DEST;
    available = _SIZE;
    while (available > 0 && *p != 0)
    {
        p++;
        available--;
    }

    if (available == 0)
    {
        //_RESET_STRING(_DEST, _SIZE);
        //_RETURN_DEST_NOT_NULL_TERMINATED(_DEST, _SIZE);
        return 1;
    }

    while ((*p++ = *_SRC++) != 0 && --available > 0)
    {
    }

    if (available == 0)
    {
        //_RESET_STRING(_DEST, _SIZE);
        //_RETURN_BUFFER_TOO_SMALL(_DEST, _SIZE);
        _DEST[_SIZE-1] = 0;
        return 2;
    }
    //_FILL_STRING(_DEST, _SIZE, _SIZE - available + 1);
    //_RETURN_NO_ERROR;
    return 0;
}


int memcpy_s(char *Dst, int nDstSize, const char *Src, int nCpyLen)
{
	if (nCpyLen >= nDstSize)
		memcpy(Dst, Src, nDstSize);
	else
		memcpy(Dst, Src, nCpyLen);
    return 0;
}
#endif

//函数功能：判断进程是否存在，如果存在则返回进程id，否则返回0
//参数 name: 进程名
DWORD GetProcessidFromName(LPCTSTR name)
{
	PROCESSENTRY32 pe;
	DWORD id=0;
	HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	pe.dwSize=sizeof(PROCESSENTRY32);
	if(!Process32First(hSnapshot,&pe))
		return 0;
	while(1)
	{
		pe.dwSize=sizeof(PROCESSENTRY32);
		if(Process32Next(hSnapshot,&pe)==FALSE)
			break;
		if(strcmp(pe.szExeFile,name)==0)
		{
			id=pe.th32ProcessID;
			break;
		}
	}
	CloseHandle(hSnapshot);
	return id;
}