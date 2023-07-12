#pragma once
#include "../../../../../SDK/SDK.h"
#include <string>
#include <fstream>
#include <filesystem>

class CUser
{
	bool Find( const wchar_t *name, std::wstring &output );
	std::wifstream m_Read = {};
	std::wofstream m_Write = {};
	std::wstring m_sPath = {};
	std::wstring ext = {};
	std::wstring file = {};

public:
	std::wstring output = {};
	std::wstring username = {};
	void Find( );
};

inline CUser g_User;