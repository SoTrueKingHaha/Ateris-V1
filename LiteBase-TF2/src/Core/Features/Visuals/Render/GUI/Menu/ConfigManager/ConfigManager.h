#pragma once

#include "../../../../../../Utils/Color/Color.h"

#include <fstream>
#include <filesystem>

class CConfigManager
{
private:
	std::wofstream m_Write;
	std::wifstream m_Read;

private:
	bool Find(const wchar_t *name, std::wstring &output);

	void SaveVar(const wchar_t *name, bool val);
	void SaveVar(const wchar_t *name, int val);
	void SaveVar(const wchar_t *name, float val);
	void SaveVar(const wchar_t *name, Color_t val);

	void LoadVar(const wchar_t *name, bool &val);
	void LoadVar(const wchar_t *name, int &val);
	void LoadVar(const wchar_t *name, float &val);
	void LoadVar(const wchar_t *name, Color_t &val);

public:
	std::wstring m_sConfigPath;
	CConfigManager();
	void Save(const wchar_t *name);
	void Load(const wchar_t *name);
	void Remove(const wchar_t *name);
};

inline CConfigManager g_CFG;