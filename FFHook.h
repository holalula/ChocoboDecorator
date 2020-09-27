#pragma once

#include <windows.h>

typedef void (*SETMODE)(int);
typedef int (*GETMODE)();
typedef void (*SETINIT)(int);
typedef int (*GETINIT)();

typedef	void (*SETR)(float);
typedef float (*GETR)();

typedef void (*SETX)(float);
typedef void (*SETY)(float);
typedef void (*SETZ)(float);


typedef float (*GETX)();
typedef float (*GETY)();
typedef float (*GETZ)();

typedef char* (*GETSENDBUF)();
typedef int (*GETSENDLEN)();

typedef int(*GET__C)();
typedef int(*GET__COUNT)();
class FFHook
{
public:
	~FFHook() {};
	FFHook(const FFHook&) = delete;
	FFHook& operator=(const FFHook&) = delete;
	static FFHook& get_instance() {
		static FFHook instance;
		return instance;

	}
	wchar_t dllPath[256];
	wchar_t logPath[256];
	int _In(const wchar_t* Path, DWORD pid);
	int _Ein(LPCTSTR szDllName, DWORD dwPID);
	DWORD ProcessFind(LPCTSTR Exename);
	BOOL CheckDllInProcess(DWORD dwPID, LPCTSTR szDllPath);
	int _LoadDLLGetFP();
	int _FreeDLL();
	int _loadDLL = 0;
	HMODULE hModule;
	SETINIT setInit;
	GETINIT getInit;
	SETMODE setMode;
	GETMODE getMode;
	SETX setX;
	SETY setY;
	SETZ setZ;
	SETR setR;
	GETX getX;
	GETY getY;
	GETZ getZ;
	GETR getR;
	GETSENDBUF get_send_buf;
	GETSENDLEN get_send_len;
	GET__C get__c;
	GET__COUNT get__count;
private:
	FFHook() {

	}
};

