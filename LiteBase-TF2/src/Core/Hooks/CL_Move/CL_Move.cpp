#include "CL_Move.h"

void __cdecl Hooks::CL_Move::Func( float flAccumulateExtraSamples, bool bFinalTick )
{
	if ( Vars::Misc::StacBypass.var )
	{
		if ( g_GlobalInfo.m_bNullCmd == true )
			return;

		Vars::Misc::CL_Move::Enabled.var = false;
	}

	if ( !Vars::Misc::CL_Move::Enabled.var )
	{
		g_GlobalInfo.m_nShifted = 24;
		return Hook.CallOriginal<fn>( )( flAccumulateExtraSamples, bFinalTick );
	}

	if ( Vars::Misc::CL_Move::TeleportKey.var && ( GetAsyncKeyState( Vars::Misc::CL_Move::TeleportKey.var ) ) && !g_GlobalInfo.m_nShifted && !g_GlobalInfo.m_bShouldShift )
	{
		while ( g_GlobalInfo.m_nShifted < 24 )
		{
			Hook.CallOriginal<fn>( )( flAccumulateExtraSamples, ( g_GlobalInfo.m_nShifted == ( 23 ) ) );
			g_GlobalInfo.m_nShifted++;
		}

		return;
	}

	if ( g_GlobalInfo.m_bRechargeQueued && !g_GlobalInfo.m_bChoking )
	{
		g_GlobalInfo.m_bRechargeQueued = false;
		g_GlobalInfo.m_bRecharging = true;
	}

	else if ( g_GlobalInfo.m_bRecharging && g_GlobalInfo.m_nShifted )
	{
		//g_GlobalInfo.m_bForceSendPacket = true;
		g_GlobalInfo.m_nShifted--;
		g_GlobalInfo.m_nWaitForShift = g_GlobalInfo.m_nTicksToShift;
		return;
	}

	else if ( GetAsyncKeyState( Vars::Misc::CL_Move::RechargeKey.var ) && !g_GlobalInfo.m_bRechargeQueued && g_GlobalInfo.m_nShifted )
	{
		//g_GlobalInfo.m_bForceSendPacket = true;
		g_GlobalInfo.m_bRechargeQueued = true;
	}

	else
	{
		g_GlobalInfo.m_bRecharging = false;
	}

	if ( Vars::Misc::CL_Move::PassiveRecharge.var )
	{
		if ( g_GlobalInfo.m_nShifted && !g_GlobalInfo.m_bShouldShift && !g_GlobalInfo.m_bAttacking )
		{
			if ( I::GlobalVars->tickcount % Vars::Misc::CL_Move::TickFrequency.var == 0 )
			{
				g_GlobalInfo.m_nShifted--;
				//g_GlobalInfo.m_nWaitForShift = DT_WAIT_CALLS;
				return;
			}
		}
	}

	Hook.CallOriginal<fn>( )( flAccumulateExtraSamples, ( g_GlobalInfo.m_bShouldShift && !g_GlobalInfo.m_nWaitForShift ) ? true : bFinalTick );

	if ( g_GlobalInfo.m_nWaitForShift )
	{
		g_GlobalInfo.m_nWaitForShift--;
		return;
	}

	if ( g_GlobalInfo.m_bShouldShift && !g_GlobalInfo.m_nWaitForShift )
	{
		while ( g_GlobalInfo.m_nShifted < g_GlobalInfo.m_nTicksToShift )
		{
			Hook.CallOriginal<fn>( )( flAccumulateExtraSamples, ( g_GlobalInfo.m_nShifted == ( g_GlobalInfo.m_nTicksToShift - 1 ) ) );
			g_GlobalInfo.m_nShifted++;
			I::EngineClient->FireEvents( );
		}

		g_GlobalInfo.m_bShouldShift = false;
	}
}

void Hooks::CL_Move::Init( )
{
	fn CLMove = reinterpret_cast< fn >( g_Pattern.Find( L"engine.dll", L"55 8B EC 83 EC ? 83 3D ? ? ? ? 02 0F 8C ? ? 00 00 E8 ? ? ? 00 84 C0" ) );
	Hook.Create( reinterpret_cast< void * >( CLMove ), reinterpret_cast< void * >( Func ) );
}