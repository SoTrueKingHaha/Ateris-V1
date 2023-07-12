#include "GetOuterAbsVelocity.h"

void __fastcall Hooks::GetOuterAbsVelocity::Func( void *ecx, void *edx, Vec3 &vec )
{
	
}

void Hooks::GetOuterAbsVelocity::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 83 EC 0C 56 8B F1 E8 ? ? ? ? 3B F0 75 2B 8B CE" ) );
	Hook.Create( pSrc, Func );
}