#pragma once
#include <Windows.h>

class CPattern
{
private:
	DWORD FindPattern(const DWORD& dwAddress, const DWORD& dwLength, LPCWSTR szPattern);
	HMODULE GetModuleHandleSafe(LPCWSTR szModuleName);

public:
	DWORD Find(LPCWSTR szModuleName, LPCWSTR szPattern);
	DWORD E8(LPCWSTR szModuleName, LPCWSTR szPattern);
};

inline CPattern g_Pattern;