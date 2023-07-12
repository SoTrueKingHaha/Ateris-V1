#include "IsPlayingTimeDemo.h"

bool __fastcall Hooks::IsPlayingTimeDemo::Func( void *ecx, void *edx )
{
	static DWORD dwInterpolateServerEntities = g_Pattern.Find( L"client.dll", L"55 8B EC 83 EC 30 8B 0D ? ? ? ? 53 33 DB 89 5D DC 89 5D E0" );

	if ( Vars::Misc::DisableInterpolation.var || g_GlobalInfo.m_bRecharging )
	{
		if ( reinterpret_cast< DWORD >( _ReturnAddress( ) ) == ( dwInterpolateServerEntities + 0xB8 ) )
		{
			return true;
		}
	}

	return Hook.CallOriginal<fn>( )( ecx, edx );
}

void Hooks::IsPlayingTimeDemo::Init( )
{
	fn FN = reinterpret_cast< fn >( Utils::GetVFuncPtr( I::EngineClient, 78 ) );
	Hook.Create( FN, Func );
}