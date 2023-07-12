#include "ShouldDrawViewmodel.h"

bool __stdcall Hooks::ShouldDrawViewmodel::Func( )
{
	if ( const auto& pLocal = g_EntityCache.GetLocal( ) )
	{
		if ( pLocal && !pLocal->deadflag( ) && pLocal->m_iClass( ) == SNIPER && pLocal->InCond( TF_COND_ZOOMED ) && Vars::Visuals::RemoveScope.var && !I::Input->CAM_IsThirdPerson( ) )
			return true;
	}

	return Hook.CallOriginal<fn>( )( I::ClientModeShared );
}

void Hooks::ShouldDrawViewmodel::Init( )
{
	fn FN = reinterpret_cast< fn >( Utils::GetVFuncPtr( I::ClientModeShared, 24 ) );
	Hook.Create( FN, Func );
}