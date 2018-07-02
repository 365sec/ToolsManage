#ifndef _UTIL_H_
#define _UTIL_H_
#include <Windows.h>

HANDLE dcreateprocess(char* cmd);
void GetExePath(char* process);
DWORD GetProcessidFromName(LPCTSTR name);
#endif