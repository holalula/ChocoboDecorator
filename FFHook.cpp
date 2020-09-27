#include "FFHook.h"
#include <TlHelp32.h>
#include <tchar.h>
#include <iostream>
using namespace std;

BOOL FFHook::CheckDllInProcess(DWORD dwPID, LPCTSTR szDllPath)
{
	BOOL                    bMore = FALSE;
	HANDLE                  hSnapshot = INVALID_HANDLE_VALUE;
	MODULEENTRY32           me = { sizeof(me), };

	if (INVALID_HANDLE_VALUE ==
		(hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID)))//获得进程的快照
	{
		_tprintf(L"CheckDllInProcess() : CreateToolhelp32Snapshot(%d) failed!!! [%d]\n",
			dwPID, GetLastError());
		return FALSE;
	}
	bMore = Module32First(hSnapshot, &me);//遍历进程内得的所有模块
	for (; bMore; bMore = Module32Next(hSnapshot, &me))
	{
		if (!_tcsicmp(me.szModule, szDllPath) || !_tcsicmp(me.szExePath, szDllPath))//模块名或含路径的名相符
		{
			CloseHandle(hSnapshot);
			return TRUE;
		}
	}
	CloseHandle(hSnapshot);
	return FALSE;
}

DWORD FFHook::ProcessFind(LPCTSTR Exename) {
	HANDLE hp = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (!hp) {
		return false;
	}
	PROCESSENTRY32 info;
	info.dwSize = sizeof(info);
	if (!Process32First(hp, &info))
		return false;
	while (1) {
		if (_tcscmp(info.szExeFile, Exename) == 0) {
			return info.th32ProcessID;
		}
		if (!Process32Next(hp, &info)) {
			return false;
		}
	}
	return false;
}

int FFHook::_In(const wchar_t* Path, DWORD pid) {
	HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);
	if (!hp) {
		cout << "5fail" << endl;
		cout << GetLastError() << endl;
		return 0;
	}
	SIZE_T PathSize = (_tcslen(Path) + 1) * sizeof(TCHAR);
	LPVOID StartAddress = VirtualAllocEx(hp, NULL, PathSize, MEM_COMMIT, PAGE_READWRITE);
	if (!StartAddress) {
		cout << "4fail" << endl;
		return 0;
	}
	if (!WriteProcessMemory(hp, StartAddress, Path, PathSize, NULL)) {
		cout << "3fail" << endl;
		return 0;
	}
	PTHREAD_START_ROUTINE pfnStartAddress = (PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "LoadLibraryW");
	if (!pfnStartAddress) {
		cout << "2fail" << endl;
		return 0;
	}

	HANDLE hThread = CreateRemoteThreadEx(hp, NULL, NULL, pfnStartAddress, StartAddress, NULL, NULL, NULL);
	if (!hThread) {
		cout << "1fail" << endl;
		return 0;
	}
	WaitForSingleObject(hThread, INFINITE);
	//WaitForSingleObject(hThread, 5000);
	CloseHandle(hThread);
	CloseHandle(hp);

	cout << "inject suc" << endl;
}

int FFHook::_Ein(LPCTSTR szDllName, DWORD dwPID)
{
	BOOL bMore = FALSE, bFound = FALSE;
	HANDLE hSnapshot, hProcess, hThread;
	HMODULE hModule = NULL;
	MODULEENTRY32 me = { sizeof(me) };
	LPTHREAD_START_ROUTINE pThreadProc;
	//dwPID=notepad进程的PID
	//使用TH32CS_SNAPMODULE参数获取加载到notepad进程的dll名称
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID);
	bMore = Module32First(hSnapshot, &me);
	for (; bMore; bMore = Module32Next(hSnapshot, &me))
	{
		if (!_tcsicmp((LPCTSTR)me.szModule, szDllName) || !_tcsicmp((LPCTSTR)me.szExePath, szDllName))
		{
			bFound = TRUE;
			break;
		}
	}
	if (!bFound)
	{
		CloseHandle(hSnapshot);
		return FALSE;
	}
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))
	{
		_tprintf(L"OpenProcess(%d) failed!!! [%d]\n,", dwPID, GetLastError());
		return FALSE;
	}
	hModule = GetModuleHandle(L"Kernel32.dll");
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hModule, "FreeLibrary");
	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, me.modBaseAddr, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	CloseHandle(hSnapshot);
	return TRUE;
}

int FFHook::_LoadDLLGetFP() {
	if (dllPath == NULL) {
		return -1;
	}
	hModule = LoadLibraryW(dllPath);
	if (hModule != NULL) {
		setMode = (SETMODE)GetProcAddress(hModule, "setMode");
		getMode = (GETMODE)GetProcAddress(hModule, "getMode");

		setInit = (SETINIT)GetProcAddress(hModule, "setInit");
		getInit = (GETINIT)GetProcAddress(hModule, "getInit");

		setX = (SETX)GetProcAddress(hModule, "setX");
		setY = (SETY)GetProcAddress(hModule, "setY");
		setZ = (SETZ)GetProcAddress(hModule, "setZ");
		setR = (SETR)GetProcAddress(hModule, "setR");

		getX = (GETX)GetProcAddress(hModule, "getX");
		getY = (GETY)GetProcAddress(hModule, "getY");
		getZ = (GETZ)GetProcAddress(hModule, "getZ");
		getR = (GETR)GetProcAddress(hModule, "getR");

		get_send_buf = (GETSENDBUF)GetProcAddress(hModule, "get_send_buf");
		get_send_len = (GETSENDLEN)GetProcAddress(hModule, "getLen");

		//get__c = (GET__C)GetProcAddress(hModule,"")

		if (get_send_len !=NULL&&get_send_buf !=NULL&&setMode != NULL && getMode != NULL && setInit != NULL && getInit != NULL &&
			setX != NULL && getX != NULL && setY != NULL && getY != NULL && setZ != NULL && getZ != NULL && setR != NULL && getR != NULL) {
			_loadDLL = 1;
			return 1;
		}

	}
	return 0;
}
int FFHook::_FreeDLL() {
	if (hModule != NULL) {
		FreeLibrary(hModule);
		_loadDLL = 0;
		return 1;
	}
	return 0;
}