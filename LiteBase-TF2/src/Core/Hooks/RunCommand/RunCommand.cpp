#include "RunCommand.h"

float GetCurTime( CUserCmd *pCmd, CTFPlayer* pLocal ) {
	static int g_tick = 0;
	static CUserCmd *g_pLastCmd = nullptr;
	if ( !g_pLastCmd || g_pLastCmd->hasbeenpredicted ) {
		g_tick = pLocal->m_nTickBase( );
	}
	else {
		// Required because prediction only runs on frames, not ticks
		// So if your framerate goes below tickrate, m_nTickBase won't update every tick
		++g_tick;
	}
	g_pLastCmd = pCmd;
	float curtime = g_tick * I::GlobalVars->interval_per_tick;
	return curtime;
}

int CalculateTick( CUserCmd* pCmd, int simTicks, CTFPlayer *player )
{
	float flCorrectionSeconds = std::clamp( 60.f / 1000.f, 0.0f, 1.0f );
	const int clockcorrect = TIME_TO_TICKS( flCorrectionSeconds ); //sv_clockcorrectmsecs

	const int nIdealFinalTick = TIME_TO_TICKS( GetCurTime( pCmd, player ) + I::EngineClient->GetNetChannelInfo( )->GetLatency( FLOW_OUTGOING ) ) + clockcorrect;

	const int estimatedFinal = player->m_nTickBase( ) + simTicks;

	const int fast = nIdealFinalTick + clockcorrect;
	const int slow = nIdealFinalTick - clockcorrect;

	if ( estimatedFinal > fast || estimatedFinal < slow ) {
		return nIdealFinalTick - simTicks;
	}

	// this is useless
	return estimatedFinal;
}

void __stdcall Hooks::RunCommand::Func( CTFPlayer *pEntity, CUserCmd *pCmd, IMoveHelper *pMoveHelper )
{
	g_GlobalInfo.tickCount = pCmd->tick_count;

	Hook.CallOriginal<fn>( )( I::Prediction, pEntity, pCmd, pMoveHelper );

	if ( pMoveHelper && !I::MoveHelper )
		I::MoveHelper = pMoveHelper;

	/*const auto &pLocal = g_EntityCache.GetLocal( );

	if ( pEntity && !pEntity->deadflag( ) && !pLocal->deadflag( ) && pLocal == pEntity && pCmd && pCmd->command_number )
	{
		int BackupTick = pEntity->m_nTickBase( );
		int BackupFinalTick = BackupTick;
		float CurtimeBackup = I::GlobalVars->curtime;

		if ( Vars::Misc::CL_Move::Doubletap.var && g_GlobalInfo.m_bShouldShift && !g_GlobalInfo.m_nWaitForShift )
		{
			if ( pCmd->command_number == g_GlobalInfo.lateUserCmd->command_number )
			{
				pEntity->m_nTickBase( ) = CalculateTick( pCmd, I::ClientState->chokedcommands + ( 24 - g_GlobalInfo.m_nShifted ) + 1, pEntity );//BackupTick - ( g_GlobalInfo.m_nTicksToShift - g_GlobalInfo.m_nShifted );
				pEntity->m_nFinalTick( ) = CalculateTick( pCmd, I::ClientState->chokedcommands + ( 24 - g_GlobalInfo.m_nShifted ) + 1, pEntity );
				I::GlobalVars->curtime = GetCurTime( pCmd, pEntity );
			}
		}

		Hook.CallOriginal<fn>( )( I::Prediction, pEntity, pCmd, pMoveHelper );

		if ( Vars::Misc::CL_Move::Doubletap.var && g_GlobalInfo.m_bShouldShift && !g_GlobalInfo.m_nWaitForShift )
		{
			if ( pCmd->command_number == g_GlobalInfo.lateUserCmd->command_number )
			{
				pEntity->m_nTickBase( ) = BackupTick;
				pEntity->m_nFinalTick( ) = BackupFinalTick;
				I::GlobalVars->curtime = CurtimeBackup;
			}
		}
		g_GlobalInfo.forwardmove = pCmd->forwardmove;
		g_GlobalInfo.sidemove = pCmd->sidemove;
		g_GlobalInfo.tickCount = pCmd->tick_count;
	}
	else
	{
		Hook.CallOriginal<fn>( )( I::Prediction, pEntity, pCmd, pMoveHelper );
	}*/
}

void Hooks::RunCommand::Init( )
{
	fn FN = reinterpret_cast< fn >( Utils::GetVFuncPtr( I::Prediction, 17 ) );
	Hook.Create( FN, Func );
}