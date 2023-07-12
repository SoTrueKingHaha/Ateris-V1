#include "Pattern.h"

#define INRANGE(x,a,b)	(x >= a && x <= b) 
#define GetBits(x)		(INRANGE((x & (~0x20)),'A','F') ? ((x & (~0x20)) - 'A' + 0xA) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GetBytes(x)		(GetBits(x[0]) << 4 | GetBits(x[1]))

DWORD CPattern::FindPattern(const DWORD& dwAddress, const DWORD& dwLength, LPCWSTR szPattern)
{
	auto szPat = szPattern;
	DWORD dwFirstMatch = 0x0;

	for (auto pCur = dwAddress; pCur < dwLength; pCur++)
	{
		if (!*szPat)
			return dwFirstMatch;

		const auto pCurByte = *(BYTE *)pCur;
		const auto pBytePatt = *(BYTE *)szPat;

		if (pBytePatt == '\?' || pCurByte == GetBytes(szPat))
		{
			if (!dwFirstMatch)
				dwFirstMatch = pCur;

			if (!szPat[2])
				return dwFirstMatch;

			szPat += (pBytePatt == '\?\?' || pBytePatt != '\?') ? 3 : 2;
		}

		else
		{
			szPat = szPattern;
			dwFirstMatch = 0x0;
		}
	}

	return 0x0;
}

HMODULE CPattern::GetModuleHandleSafe(LPCWSTR szModuleName)
{
	HMODULE hModule = 0x0;

	while (!hModule) {
		hModule = GetModuleHandleW(szModuleName);
		Sleep(1);
	}

	return hModule;
}

DWORD CPattern::Find(LPCWSTR szModuleName, LPCWSTR szPattern)
{
	if (const auto hMod = reinterpret_cast<DWORD>(GetModuleHandleSafe(szModuleName))) {
		if (const auto pNT = reinterpret_cast<PIMAGE_NT_HEADERS>(hMod + reinterpret_cast<PIMAGE_DOS_HEADER>(hMod)->e_lfanew))
			return FindPattern(hMod + pNT->OptionalHeader.BaseOfCode, hMod + pNT->OptionalHeader.SizeOfCode, szPattern);
	}

	return 0x0;
}

DWORD CPattern::E8( LPCWSTR szModuleName, LPCWSTR szPattern )
{
	DWORD dwAddress = Find( szModuleName, szPattern ) + 0x1;
	return *reinterpret_cast< PDWORD >( dwAddress ) + dwAddress + 4;
}