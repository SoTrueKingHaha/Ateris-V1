#include "IsTaunting.h"

bool __stdcall Hooks::IsTaunting::Func( )
{
	return false;
}

void Hooks::IsTaunting::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 83 EC 08 56 57 8B 7D 08 8B F1 83" ) );
	Hook.Create( pSrc, Func );
}