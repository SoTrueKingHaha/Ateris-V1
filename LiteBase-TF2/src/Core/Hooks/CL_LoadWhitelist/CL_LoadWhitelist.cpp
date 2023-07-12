#include "CL_LoadWhitelist.h"

void *__cdecl Hooks::CL_LoadWhitelist::Func( void *table, const char *name )
{
	if ( Vars::Misc::PureBypass.var )
	{
		return nullptr;
	}

	return Hook.CallOriginal<fn>( )( table, name );
}

void Hooks::CL_LoadWhitelist::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"engine.dll", L"55 8B EC 83 EC ? 56 8B 75 ? 8B CE FF 75" ) );
	Hook.Create( pSrc, Func );
}