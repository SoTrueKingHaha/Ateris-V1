#include "CreateMove.h"
#include "../../Features/Assistance/Triggerbot/Triggerbot.h"
//#include "../../Features/Exploits/CritHack/CritHack.h"
#include "../../Features/Exploits/CritHack/CritHack.h"
#include "../../Features/Exploits/Backtrack/Backtrack.h"
#include "../../Features/Exploits/Fakelag/Fakelag.h"
#include "../../Features/Assistance/Aimbot/Aimbot.h"
#include "../../Features/Exploits/AntiAim/AntiAim.h"
#include "../../Features/Exploits/AntiAim/AntiAimBeta.h"
#include "../../Features/Exploits/AntiAim/FakeAng.h"
#include "../../Utils/Timer/Timer.h"
#include "../../Features/Visuals/Models/Models.h"
#include "../../Features/Events/BadActors/BadActors.h"
#include "../../Features/Visuals/Outlines/Outlines.h"

void AttackingUpdate( ) {
	if ( !g_GlobalInfo.m_bAttacking ) { return; }

	if ( const auto& pLocal = g_EntityCache.GetLocal( ) ) {
		if ( const auto& pWeapon = g_EntityCache.GetWeapon( ) )
		{
			const float flFireDelay = pWeapon->GetWeaponData( ).m_flTimeFireDelay;
			pWeapon->m_flNextPrimaryAttack( ) = static_cast< float >( pLocal->m_nTickBase( ) ) * I::GlobalVars->interval_per_tick + flFireDelay;
		}
	}
}

bool __fastcall Hooks::CreateMove::Func( void *ecx, void *edx, float flInputSampleTime, CUserCmd *pCmd )
{
	g_GlobalInfo.m_bSilentTime = false;
	g_GlobalInfo.m_bAttacking = false;

	const auto& pLocal = g_EntityCache.GetLocal( );

	if ( !pCmd || !pCmd->command_number || !pLocal || pLocal->deadflag( ) )
		return Hook.CallOriginal<fn>( )( ecx, edx, flInputSampleTime, pCmd );

	if ( Hook.CallOriginal<fn>( )( ecx, edx, flInputSampleTime, pCmd ) )
		I::Prediction->SetLocalViewAngles( pCmd->viewangles );

	uintptr_t _bp;
	__asm mov _bp, ebp;
	auto pSendPacket = reinterpret_cast< bool * >( ***reinterpret_cast< uintptr_t *** >( _bp ) - 0x1 );

	int nOldFlags = 0;
	int nOldGroundEnt = 0;
	Vec3 vOldAngles = pCmd->viewangles;
	float fOldSide = pCmd->sidemove;
	float fOldForward = pCmd->forwardmove;
	g_GlobalInfo.m_vRealViewAngles = pCmd->viewangles;

	g_GlobalInfo.currentUserCmd = pCmd;
	const auto &pWeapon = g_EntityCache.GetWeapon( );

	if ( pLocal )
	{
		nOldFlags = pLocal->m_fFlags( );
		static int nOldClass = pLocal->m_iClass( );
		if ( pWeapon )
		{
			const int nItemDefIndex = pWeapon->m_iItemDefinitionIndex( );

			if ( g_GlobalInfo.m_nCurItemDefIndex != nItemDefIndex || !pWeapon->m_iClip1( ) || ( pLocal->deadflag( ) || pLocal->InCond( TF_COND_TAUNTING ) || pLocal->InCond( TF_COND_PHASE ) || pLocal->InCond( TF_COND_HALLOWEEN_GHOST_MODE ) || pLocal->InCond( TF_COND_HALLOWEEN_KART ) ) )
			{
				g_GlobalInfo.m_nWaitForShift = DT_WAIT_CALLS + 1;
			}

			if ( pWeapon->GetWeaponID( ) == TF_WEAPON_MINIGUN )
			{
				const int nState = pWeapon->As<CTFMinigun>( )->m_iWeaponState( );
				if ( g_GlobalInfo.m_nState != nState && nState != AC_STATE_SPINNING && nState != AC_STATE_IDLE )
				{
					g_GlobalInfo.m_nWaitForShift = DT_WAIT_CALLS + 1;
				}
				g_GlobalInfo.m_nState = nState;
			}

			if ( nOldClass != pLocal->m_iClass( ) && Vars::CritHack::Active.var )
			{
				g_Crits.stats.flCritBucket[ 0 ] = pLocal->m_iClass( ) == HEAVY ? 0.f : 300.f;
				g_Crits.stats.flCritBucket[ 1 ] = 300.f;
				g_Crits.stats.flCritBucket[ 2 ] = 300.f;
				nOldClass = pLocal->m_iClass( );
			}

			g_GlobalInfo.m_nCurItemDefIndex = nItemDefIndex;
			if ( pLocal->m_iClass( ) == SPY )
			{
				if ( pWeapon->m_iItemDefinitionIndex( ) == Spy_m_TheAmbassador || pWeapon->m_iItemDefinitionIndex( ) == Spy_m_FestiveAmbassador )
					g_GlobalInfo.m_bWeaponCanHeadShot = pWeapon->AmbassadorCanHeadshot( );
				else
					g_GlobalInfo.m_bWeaponCanHeadShot = pWeapon->CanHeadShot( pLocal );
			}
			else
			{
				g_GlobalInfo.m_bWeaponCanHeadShot = pWeapon->CanHeadShot( pLocal );
			}
			g_GlobalInfo.m_bWeaponCanAttack = pWeapon->CanShoot( pLocal );
			g_GlobalInfo.m_bWeaponCanSecondaryAttack = pWeapon->CanSecondaryAttack( pLocal );
			g_GlobalInfo.m_WeaponType = Utils::GetWeaponType( pWeapon );
			g_GlobalInfo.m_bAttacking = Utils::IsAttacking( pCmd, pWeapon );

			if ( pWeapon->GetSlot( ) != SLOT_MELEE )
			{
				if ( !pWeapon->IsInReload( ) )
					g_GlobalInfo.m_bWeaponCanAttack = true;

				if ( pWeapon->m_iItemDefinitionIndex( ) != Soldier_m_TheBeggarsBazooka )
				{
					if ( pWeapon->m_iClip1( ) == 0 )
						g_GlobalInfo.m_bWeaponCanAttack = false;
				}
			}
		}

		/*if ( const auto &netChan = I::EngineClient->GetNetChannelInfo( ) )
		{
			static uint32_t oldMap = FNV1A::HashConst( I::EngineClient->GetLevelName( ) );
			static uint32_t oldAddress = FNV1A::HashConst( netChan->GetAddress( ) );
			const uint32_t curMap = FNV1A::HashConst( I::EngineClient->GetLevelName( ) );
			const uint32_t curAddress = FNV1A::HashConst( netChan->GetAddress( ) );

			if ( curMap != oldMap || curAddress != oldAddress )
			{
				Outlines.Init( );
				Models.Init( );
				oldMap = curMap; oldAddress = curAddress;
			}
		}*/

		if ( pLocal->m_vecVelocity( ).IsZero( ) )
			g_GlobalInfo.m_nTicksToShift = Vars::Misc::CL_Move::DTTicksStanding.var;
		else
			g_GlobalInfo.m_nTicksToShift = Vars::Misc::CL_Move::DTTicksMoving.var;
	}
	
	static Timer RichPresenceTimer{};
	if ( RichPresenceTimer.Run( 1000 ) ) {
		g_Misc.SteamRPC( );
	}

	/*static Timer ValidateTimer{ };
	if ( ValidateTimer.Run( 3000 ) )
	{
		for ( auto pMat : Models.matList )
		{
			if ( !pMat ) { continue; }
			Models.ValidateMaterial( pMat );
		}
	}
	*/

	/*bool choked = I::ClientState->chokedcommands >= 10;
	if ( choked )
	{
		pCmd->buttons |= IN_DUCK;
	}
	else
	{
		pCmd->buttons &= ~IN_DUCK;
	}*/

	/*if ( g_GlobalInfo.m_bShouldShift && !g_GlobalInfo.m_nWaitForShift )
	{
		*pSendPacket = g_GlobalInfo.m_nShifted == ( g_GlobalInfo.m_nTicksToShift );
	}*/

	g_Misc.Run( pCmd );
	g_BadActors.OnTick( );
	g_Backtrack.Run( pCmd );

	g_Prediction.Start( pCmd );
	{
		g_Aimbot.Run( pCmd );
		g_Triggerbot.Run( pLocal, pWeapon, pCmd );
		//g_AntiAim.Run( pCmd, pSendPacket );
		//g_AntiAimBeta.Run( pCmd, pSendPacket );

		static CInputHelper cyclerealkey;
		cyclerealkey.Update( );

		if ( cyclerealkey.IsPressed( Vars::AntiHack::AntiAimBeta::CycleRealKey.var ) )
		{
			g_AntiAimBeta.bRealFlip = !g_AntiAimBeta.bRealFlip;
		}

		static CInputHelper cyclefakekey;
		cyclefakekey.Update( );

		if ( cyclefakekey.IsPressed( Vars::AntiHack::AntiAimBeta::CycleFakeKey.var ) )
		{
			g_AntiAimBeta.bFakeFlip = !g_AntiAimBeta.bFakeFlip;
		}

		static Vec3 fakeangle;
		static Vec3 realangle;
		if ( Vars::Misc::CL_Move::Fakelag.var && Vars::AntiHack::AntiAimBeta::Active.var )
		{
			if ( g_GlobalInfo.m_nShifted > 23 )
			{
				if ( I::ClientState->chokedcommands == Vars::Misc::CL_Move::FakelagValue.var )
				{
					g_AntiAimBeta.DoFake( pCmd, pSendPacket );
					g_GlobalInfo.m_vFakeViewAngles = pCmd->viewangles;
				}
				else
				{
					g_AntiAimBeta.DoReal( pCmd, pSendPacket );
					g_GlobalInfo.m_vRealViewAngles = pCmd->viewangles;
				}
			}

			
			else if ( g_GlobalInfo.m_nShifted >= std::max( Vars::Misc::CL_Move::DTTicksStanding.var, Vars::Misc::CL_Move::DTTicksMoving.var ) - abs( Vars::Misc::CL_Move::DTTicksStanding.var - Vars::Misc::CL_Move::DTTicksMoving.var ) - 1 ) //no dt
			{
				int choked = std::clamp( Vars::Misc::CL_Move::FakelagValue.var < g_GlobalInfo.m_nTicksToShift ? Vars::Misc::CL_Move::FakelagValue.var : g_GlobalInfo.m_nShifted + 1, 1, 21 );
				if ( I::ClientState->chokedcommands == choked )
				{
					g_AntiAimBeta.DoFake( pCmd, pSendPacket );
					g_GlobalInfo.m_vFakeViewAngles = pCmd->viewangles;
				}
				else
				{
					g_AntiAimBeta.DoReal( pCmd, pSendPacket );
					g_GlobalInfo.m_vRealViewAngles = pCmd->viewangles;
				}
			}
			else //dt
			{
				if ( !g_GlobalInfo.m_bShouldShift )
				{
					if ( !I::ClientState->chokedcommands )
					{
						g_AntiAimBeta.DoReal( pCmd, pSendPacket );
						g_GlobalInfo.m_vRealViewAngles = pCmd->viewangles;
						*pSendPacket = false;
					}
					else
					{
						g_AntiAimBeta.DoFake( pCmd, pSendPacket );
						g_GlobalInfo.m_vFakeViewAngles = pCmd->viewangles;
						*pSendPacket = true;
					}
				}
			}
		}
		else
		{
			if ( Vars::AntiHack::AntiAimBeta::Active.var )
			{
				if ( pCmd->command_number % 2 == 0 )
				{
					g_AntiAimBeta.DoReal( pCmd, pSendPacket );
					g_GlobalInfo.m_vRealViewAngles = pCmd->viewangles;
					*pSendPacket = false;
				}
				else
				{
					g_AntiAimBeta.DoFake( pCmd, pSendPacket );
					g_GlobalInfo.m_vFakeViewAngles = pCmd->viewangles;
					*pSendPacket = true;
				}
				//g_GlobalInfo.m_vFakeViewAngles = fakeangle;
				//g_GlobalInfo.m_vRealViewAngles = realangle;
			}
		}
		g_FakeLag.OnTick( pCmd, pSendPacket );
	}
	g_Prediction.End( pCmd );

	g_Crits.Run( pCmd );
	g_Misc.RunLate( pCmd );

	/*if ( *pSendPacket )
	{
		g_FakeAng.Run( pCmd );
		g_FakeAng.DrawChams = Vars::AntiHack::AntiAim::Active.var || Vars::Misc::CL_Move::Fakelag.var;
	}*/

	g_GlobalInfo.m_vViewAngles = pCmd->viewangles;

	static int nChoked = 0;
	if ( g_GlobalInfo.m_bShouldShift )
	{
		AttackingUpdate( );
		*pSendPacket = g_GlobalInfo.m_nShifted == ( g_GlobalInfo.m_nTicksToShift - 1 );

		if ( !*pSendPacket ) { nChoked++; }
		else { nChoked = 0; }
		if ( nChoked > 24 ) { *pSendPacket = true; }
	}

	/*if ( GetAsyncKeyState( Vars::Aimbot::Global::AimKey.var ) && g_GlobalInfo.m_bAttacking && g_GlobalInfo.m_bWeaponCanAttack )
	{
		I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "unmodified: %i\n", g_GlobalInfo.tickCount );
		I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "modified: %i\n", pCmd->tick_count );
	}*/

	/*if ( pLocal )
	{
		if ( Vars::Misc::TauntSlide.var )
		{
			if ( pLocal->InCond( TF_COND_TAUNTING ) )
			{
				pCmd->viewangles.x = ( pCmd->buttons & IN_BACK )
					? 91.0f
					: ( pCmd->buttons & IN_FORWARD )
					? 0.0f
					: 90.0f;

				return false;
			}
		}
	}*/

	static bool bWasSet = false;

	if ( g_GlobalInfo.m_bSilentTime ) {
		*pSendPacket = false;
		bWasSet = true;
	}

	else
	{
		if ( bWasSet )
		{
			*pSendPacket = true;
			pCmd->viewangles = vOldAngles;
			pCmd->sidemove = fOldSide;
			pCmd->forwardmove = fOldForward;
			bWasSet = false;
		}
	}

	if ( g_GlobalInfo.m_bForceSendPacket )
	{
		*pSendPacket = true;
		g_GlobalInfo.m_bForceSendPacket = false;
	} // if we are trying to force update do this lol
	else if ( g_GlobalInfo.m_bForceChokePacket )
	{
		*pSendPacket = false;
		g_GlobalInfo.m_bForceChokePacket = false;
	}

	//failsafe
	{
		static int nChoked = 0;
		const int nAmount = Vars::Misc::CL_Move::Fakelag.var ? ( Vars::Misc::CL_Move::FakelagSafe.var ? 14 : 21 ) : ( g_GlobalInfo.m_nTicksToShift - 1 );

		if ( !*pSendPacket )
			nChoked++;

		else nChoked = 0;

		if ( nChoked > nAmount )
			*pSendPacket = true;
	}

	/*if ( pWeapon && g_GlobalInfo.m_nTicksToShift > 20 )
	{
		if ( pCmd->buttons & IN_ATTACK )
		{
			if ( g_GlobalInfo.nextSafeTick > I::GlobalVars->tickcount && g_GlobalInfo.m_bShouldShift && g_GlobalInfo.m_nShifted ) {
				pCmd->buttons &= ~IN_ATTACK;
			}
			else {
				g_GlobalInfo.nextSafeTick = I::GlobalVars->tickcount + g_ConVars.sv_maxusrcmdprocessticks_holdaim->GetInt( );
			}
		}
	}*/
	g_GlobalInfo.lateUserCmd = pCmd;

	if ( g_GlobalInfo.m_bShouldStop )
	{
		//G::ShouldStop = false;	//	we still need to stop if we didn't stop...
		Utils::StopMovement( pCmd, !g_GlobalInfo.m_bShouldShift );
		if ( !g_GlobalInfo.m_bAttacking && !g_GlobalInfo.m_bRecharging && !g_GlobalInfo.m_bShouldStop ) {	//	only do this code if we DID actually stop.
			*pSendPacket = false;	//	stop angle shit
		}
		return false;
	}

	if ( g_GlobalInfo.m_bSilentTime || g_GlobalInfo.m_bHitscanSilentActive || g_GlobalInfo.m_bProjectileSilentActive || g_GlobalInfo.m_bAAActive || g_GlobalInfo.m_bAvoidingBackstab || g_GlobalInfo.m_bScoutJumping /* || !g_Misc.TauntControl( pCmd )*/ || ( g_GlobalInfo.m_bShouldShift && !g_GlobalInfo.m_nWaitForShift ) )
	{
		return false;
	}

	return Hook.CallOriginal<fn>( )( ecx, edx, flInputSampleTime, pCmd );
}

void Hooks::CreateMove::Init()
{
	fn FN = reinterpret_cast<fn>( Utils::GetVFuncPtr( I::ClientModeShared, 21 ) );
	Hook.Create(FN, Func);
	/*void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC E8 ? ? ? ? 8B C8 85 C9 75 06" ) );
	Hook.Create( pSrc, Func );*/
}