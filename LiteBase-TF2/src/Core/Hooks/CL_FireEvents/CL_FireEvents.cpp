#include "CL_FireEvents.h"

float __fastcall Hooks::CL_FireEvents::Func( void *ecx, void *edx )
{
	static DWORD dwGetTime = g_Pattern.Find( ( L"engine.dll" ), ( L"D9 43 ? DF F1" ) );

	if ( reinterpret_cast< DWORD >( _ReturnAddress( ) ) == ( dwGetTime ) )
		return FLT_MAX;

	return Hook.CallOriginal<fn>( )( ecx, edx );
}

void Hooks::CL_FireEvents::Init( )
{
	DWORD dwCLFireEvents = g_Pattern.Find( L"engine.dll", L"E8 ? ? ? ? 83 EC 10" ) + 0x1;
	DWORD dwEstimate = *( PDWORD )dwCLFireEvents + dwCLFireEvents + 4;
	fn CLFireEvents = reinterpret_cast< fn >( dwEstimate );
	Hook.Create( CLFireEvents, Func );
}