// Deamon.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <Windows.h>
#include "tlhelp32.h"

//�������ܣ��жϽ����Ƿ���ڣ���������򷵻ؽ���id�����򷵻�0
//���� name: ������
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
	si.wShowWindow = SW_SHOW;
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

int _tmain(int argc, _TCHAR* argv[])
{
	while (true)
	{
		if(GetProcessidFromName("ToolManage.exe")){
			Sleep(1000);
		}
		else{
			dcreateprocess("ToolManage.exe");
			printf("��������ToolManage\n");
			Sleep(10000);
		}
	}
	return 0;
}

