#include "DataTableWarning.h"

void __cdecl Hooks::DataTableWarning::Func( const char *pInMessage )
{
	return;
}

void Hooks::DataTableWarning::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"engine.dll", L"55 8B EC B8 ? ? ? ? E8 ? ? ? ? 8D 45 0C 50 FF 75 08 8D 85 ? ? ? ? 68 ? ? ? ? 50 E8 ? ? ? ? 8D 85 ? ? ? ? 50 68 ? ? ? ? FF 15 ? ? ? ? 83 C4 18 8B E5 5D C3" ) );
	Hook.Create( pSrc, Func );
}