#include "ScoreboardHook.h"

void __fastcall Hooks::KeyValues::Func( void *ecx, void *edx, const char *keyName, int value )
{
	Hook.CallOriginal<fn>( )( ecx, edx, keyName, value );

	static DWORD dwDesired = g_Pattern.Find( L"client.dll", L"8B 03 8B CB 56 57 FF 90 ? ? ? ? 6A" );
	static DWORD dwJump = g_Pattern.Find( L"client.dll", L"89 45 BC E8 ? ? ? ? 3B C7 75 1D 80 7D F8 00 75 17 8B 4D C0" );

	if ( reinterpret_cast< DWORD >( _ReturnAddress( ) ) == dwDesired && std::string_view( keyName ).find( "nemesis" ) != std::string_view::npos )
		*reinterpret_cast< uintptr_t * >( _AddressOfReturnAddress( ) ) = dwJump;
}

void Hooks::KeyValues::Init( )
{
	fn FN = reinterpret_cast< fn >( g_Pattern.Find(
		L"client.dll",
		L"55 8B EC 6A 01 FF 75 08 E8 ? ? ? ? 85 C0 74 0A 8B 4D 0C 89 48 0C C6 40 10 02" ) );
	Hook.Create( FN, Func );
}

bool __fastcall Hooks::IsPlayerDominated::Func( void *ecx, void *edx, int index )
{
	bool bResult = Hook.CallOriginal<fn>( )( ecx, edx, index );

	if ( !bResult )
	{
		static DWORD dwDesired = g_Pattern.Find( L"client.dll", L"84 C0 74 ? 80 7D ? ? 74 ? 8B 83" );
		static DWORD dwJump = g_Pattern.Find( L"client.dll", L"89 45 BC E8 ? ? ? ? 3B C7 75 1D 80 7D F8 00 75 17 8B 4D C0" );

		if ( reinterpret_cast< DWORD >( _ReturnAddress( ) ) == dwDesired )
			*reinterpret_cast< uintptr_t * >( _AddressOfReturnAddress( ) ) = dwJump;
	}

	return bResult;
}

void Hooks::IsPlayerDominated::Init( )
{
	fn FN = reinterpret_cast< fn >( g_Pattern.Find(
		L"client.dll",
		L"55 8B EC 56 57 8B F1 E8 ? ? ? ? 8B F8 85 FF 75 08 5F 32 C0 5E 5D C2 04 00" ) );
	Hook.Create( FN, Func );
}