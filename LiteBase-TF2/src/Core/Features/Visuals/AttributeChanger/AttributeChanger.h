#pragma once
#include "../../../SDK/SDK.h"

#include <fstream>
#include <filesystem>

enum AttributeID
{
	IsAustraliumItem = 2027,
	LootRarity = 2022,
	ItemStyleOverride = 542,
	AncientPowers = 150,
	IsFestive = 2053,
	Sheen = 2014,
	UnusualEffect = 134,
	ParticleEffect = 370
};

struct AttributeInfo_t
{
	int ItemDefIndex;
	int Effect;
	int Particle;
	int Sheen;
	bool Ancient;
	bool StyleOverride;
	bool Australium;
};

class CAttributeChanger
{
	std::unordered_map<int, AttributeInfo_t> AttributeMap;

	std::ifstream ReadStream;
	std::ofstream WriteStream;
	std::wstring AttributePath;

	__inline void SaveInt( const wchar_t *szSection, const wchar_t *szItem, int value )
	{
		const std::wstring szToSave = std::to_wstring( value );
		WritePrivateProfileStringW( szSection, szItem, szToSave.c_str( ), AttributePath.c_str( ) );
	}

	__inline void SaveBool( const wchar_t *szSection, const wchar_t *szItem, bool value )
	{
		const std::wstring szToSave = value ? L"true" : L"false";
		WritePrivateProfileStringW( szSection, szItem, szToSave.c_str( ), AttributePath.c_str( ) );
	}

	__inline int LoadInt( const wchar_t *szSection, const wchar_t *szItem )
	{
		wchar_t szReturn[ 69 ];
		GetPrivateProfileStringW( szSection, szItem, L"0", szReturn, 69, AttributePath.c_str( ) );
		return std::stoi( szReturn );
	}

	__inline bool LoadBool( const wchar_t *szSection, const wchar_t *szItem )
	{
		wchar_t szReturn[ 69 ];
		GetPrivateProfileStringW( szSection, szItem, L"false", szReturn, 69, AttributePath.c_str( ) );
		return ( wcscmp( szReturn, L"true" ) == 0 ) ? true : false;
	}

public:
	void Run( );
	void Init( );

	void SetAttribute( );
	void SaveConfig( );
	void LoadConfig( );
};

inline CAttributeChanger g_AttributeChanger;