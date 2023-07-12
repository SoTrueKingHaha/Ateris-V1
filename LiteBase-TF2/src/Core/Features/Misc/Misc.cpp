#include "Misc.h"

#include "../Vars.h"
#include "../../Utils/Timer/Timer.h"
#include "../../SDK/Include/Steam/ISteamUserStats011.h"
#include "../../SDK/Include/Steam/ISteamFriends015.h"
#include "../../SDK/Include/EntityCache/EntityCache.h"
#include "../Visuals/Render/GUI/Menu/InputHelper/InputHelper.h"
#include "../Events/BadActors/BadActors.h"

void CMisc::Run( CUserCmd *pCmd )
{
	if ( const auto& pLocal = g_EntityCache.GetLocal( ) )
	{
		AccurateMovement(pCmd, pLocal);
		AutoJump( pCmd, pLocal );
		AutoScoutJump( pCmd, pLocal );
		AutoStrafe( pCmd, pLocal );
		AutoPeek( pCmd, pLocal );
		ServerHitbox( pLocal ); // super secret deathpole feature!!!!
		AntiBackstab(pCmd, pLocal);
	}

	CheatsBypass( );
	FixOutlines( );
	Viewmodel( );
	Console( );
	WeaponSway( );
	DetectChoke( );
}

void CMisc::RunLate( CUserCmd *pCmd )
{
	if ( const auto& pLocal = g_EntityCache.GetLocal( ) )
	{
		LegJitter( pCmd, pLocal );
		FastStop(pCmd, pLocal);
		//AutoRocketJump(pCmd, pLocal);
		//AutoScoutJump(pCmd, pLocal);
	}
}

/*void CMisc::Teleport( const CUserCmd *pCmd )
{
	// Stupid
	if ( GetAsyncKeyState( Vars::Misc::CL_Move::TeleportKey.var ) & 0x8000 )
	{
		if ( Vars::Misc::CL_Move::TeleportMode.var == 0 && g_GlobalInfo.tickShiftQueue == 0 && g_GlobalInfo.m_nShifted > 0 )
		{
			// Plain teleport
			g_GlobalInfo.tickShiftQueue = g_GlobalInfo.m_nShifted;
		} // Why do it like this wtf?
	}
}*/

void CMisc::LegJitter( CUserCmd *pCmd, CTFPlayer *pLocal )
{
	static bool pos = true;
	const float scale = pLocal->IsDucking( ) ? 14.f : 2.f;
	if ( g_GlobalInfo.m_bAttacking || g_GlobalInfo.m_bShouldShift || g_GlobalInfo.AntiAim.second ) { return; }
	if ( pCmd->forwardmove == 0.f && pCmd->sidemove == 0.f && pLocal->m_vecVelocity( ).Length2D( ) < 10.f && Vars::AntiHack::AntiAimBeta::Active.var/* && I::GlobalVars->tickcount % 2*/ )
	{
		pos ? pCmd->forwardmove = scale : pCmd->forwardmove = -scale;
		pos ? pCmd->sidemove = scale : pCmd->sidemove = -scale;
		pos = !pos;
	}
}

void CMisc::AntiBackstab( CUserCmd *pCmd, CTFPlayer *pLocal )
{
	g_GlobalInfo.m_bAvoidingBackstab = false;
	Vec3 vTargetPos;

	if ( pLocal->deadflag( ) || pLocal->InCond( TF_COND_STUNNED ) || pLocal->InCond( TF_COND_HALLOWEEN_KART ) || pLocal->InCond( TF_COND_HALLOWEEN_GHOST_MODE ) || !Vars::AntiHack::AntiAim::AntiBackstab.var || !Vars::AntiHack::AntiAim::Active.var )
	{
		return;
	}

	if ( g_GlobalInfo.m_bAttacking ) { return; }

	const Vec3 vLocalPos = pLocal->GetWorldSpaceCenter( );
	CBaseEntity *target = nullptr;

	for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::PLAYERS_ENEMIES ) )
	{
		const auto &pPlayer = pEntity->As<CTFPlayer>( );

		if ( pPlayer->m_iTeamNum( ) != pLocal->m_iTeamNum( ) )
		{
			const auto &pEnemy = pPlayer;

			if ( !pEnemy || pEnemy->deadflag( ) || pEnemy->m_iClass( ) != SPY || pEnemy->InCond( TF_COND_STEALTHED ) || pEnemy->InCond( TF_COND_HALLOWEEN_GHOST_MODE ) )
			{
				continue;
			}

			Vec3 vEnemyPos = pEnemy->GetWorldSpaceCenter( );
			if ( !Utils::VisPos( pLocal, pEnemy, vLocalPos, vEnemyPos ) ) { continue; }
			if ( !target && vLocalPos.DistTo( vEnemyPos ) < 150.f )
			{
				target = pEnemy;
				vTargetPos = target->GetWorldSpaceCenter( );
			}
			else if ( vLocalPos.DistTo( vEnemyPos ) < vLocalPos.DistTo( vTargetPos ) && vLocalPos.DistTo( vEnemyPos ) < 150.f )
			{
				target = pEnemy;
				vTargetPos = target->GetWorldSpaceCenter( );
			}
		}
	}

	if ( target )
	{
		vTargetPos = target->GetWorldSpaceCenter( );
		const Vec3 vAngleToSpy = Utils::CalcAngle( vLocalPos, vTargetPos );
		g_GlobalInfo.m_bAvoidingBackstab = true;
		Utils::FixMovement( pCmd, vAngleToSpy );
		pCmd->viewangles = vAngleToSpy;
	}
}

void CMisc::AccurateMovement( CUserCmd *pCmd, CTFPlayer *pLocal )
{
	if ( !Vars::Misc::AccurateMovement.var || bMovementStopped )
	{
		return;
	}

	if ( pLocal->deadflag( )
		 || pLocal->IsSwimming( )
		 || pLocal->InCond( TF_COND_HALLOWEEN_KART )
		 || pLocal->InCond( TF_COND_HALLOWEEN_GHOST_MODE )
		 || pLocal->InCond( TF_COND_SHIELD_CHARGE )
		 || !pLocal->OnSolid( ) )
	{
		return;
	}

	if ( pLocal->GetMoveType( ) == MOVETYPE_NOCLIP
		 || pLocal->GetMoveType( ) == MOVETYPE_LADDER
		 || pLocal->GetMoveType( ) == MOVETYPE_OBSERVER )
	{
		return;
	}

	if ( pCmd->buttons & ( IN_JUMP | IN_MOVELEFT | IN_MOVERIGHT | IN_FORWARD | IN_BACK ) )
	{
		return;
	}

	const float speed = pLocal->m_vecVelocity( ).Length2D( );
	const float speedLimit = 10.f;

	if ( speed > speedLimit )
	{
		Vec3 direction = pLocal->m_vecVelocity( ).toAngle( );
		direction.y = pCmd->viewangles.y - direction.y;
		const Vec3 negatedDirection = direction.fromAngle( ) * -speed;
		pCmd->forwardmove = negatedDirection.x;
		pCmd->sidemove = negatedDirection.y;
	}
	else
	{
		pCmd->forwardmove = 0.0f;
		pCmd->sidemove = 0.0f;
	}
}

bool CanAttack( CTFPlayer *pLocal, const Vec3 &pPos )
{
	if ( const auto& pWeapon = g_EntityCache.GetWeapon( ) )
	{
		if ( !g_GlobalInfo.m_bWeaponCanHeadShot && pLocal->InCond( TF_COND_ZOOMED ) ) { return false; }
		if ( !pWeapon->CanShoot( pLocal ) ) { return false; }

		for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::PLAYERS_ENEMIES ) )
		{
			const auto &target = pEntity->As<CTFPlayer>( );

			if ( target->deadflag( ) || target->m_iTeamNum( ) == pLocal->m_iTeamNum( ) ) { continue; }
			//if ( F::AimbotGlobal.ShouldIgnore( target ) ) { continue; }

			// Get the hitbox position (Backtrack if possible)
			Vec3 targetPos = target->GetHitboxPos( HITBOX_HEAD );
			/*if ( Vars::Backtrack::Enabled.Value )
			{
				const auto &btRecord = F::Backtrack.GetRecord( target->GetIndex( ), BacktrackMode::Last );
				if ( btRecord ) { targetPos = btRecord->HeadPosition; }
			}*/

			// Is the player visible?
			if ( Utils::VisPos( pLocal, target, pPos, targetPos ) )
			{
				return true;
			}
		}
	}

	return false;
}

void CMisc::AutoPeek( CUserCmd *pCmd, CTFPlayer *pLocal )
{
	if ( !Vars::Misc::CL_Move::AutoPeek.var )
		return;

	static bool posPlaced = false;
	static bool isReturning = false;
	static bool hasDirection = false;
	static Vec3 peekStart;
	static Vec3 peekVector;

	if ( !pLocal->deadflag( ) && Vars::Misc::CL_Move::AutoPeekKey.var &&( GetAsyncKeyState( Vars::Misc::CL_Move::AutoPeekKey.var ) & 0x8000 ) )
	{
		const Vec3 localPos = pLocal->GetAbsOrigin( );

		// We just started peeking. Save the return position!
		if ( !posPlaced )
		{
			if ( pLocal->OnSolid( ) )
			{
				PeekReturnPos = localPos;
				posPlaced = true;
			}
		}
		/*else
		{
			static Timer particleTimer{};
			if ( particleTimer.Run( 700 ) )
			{
				Particles::DispatchParticleEffect( "ping_circle", PeekReturnPos, {} );
			}
		}*/

		// We need a peek direction (A / D)
		if ( !Vars::Misc::CL_Move::AutoPeekFree.var && !hasDirection && pLocal->OnSolid( ) )
		{
			const Vec3 viewAngles = I::EngineClient->GetViewAngles( );
			Vec3 vForward, vRight, vUp, vDirection;
			Utils::AngleVectors( viewAngles, &vForward, &vRight, &vUp );

			if ( GetAsyncKeyState( 0x41 ) & 0x8000 || GetAsyncKeyState( 0x57 ) & 0x8000 || GetAsyncKeyState( 0x44 ) & 0x8000 || GetAsyncKeyState( 0x53 ) & 0x8000 )
			{
				CGameTrace trace;
				CTraceFilterWorldAndPropsOnly traceFilter;
				Ray_t traceRay;

				if ( GetAsyncKeyState( 0x41 ) & 0x8000 || GetAsyncKeyState( 0x57 ) & 0x8000 )
				{
					vDirection = pLocal->GetEyePosition( ) - vRight * 200.f;//Vars::Misc::CL_Move::AutoPeekDistance.Value; // Left
				}
				else if ( GetAsyncKeyState( 0x44 ) & 0x8000 || GetAsyncKeyState( 0x53 ) & 0x8000 )
				{
					vDirection = pLocal->GetEyePosition( ) + vRight * 200.f;//Vars::Misc::CL_Move::AutoPeekDistance.Value; // Right
				}

				traceRay.Init( pLocal->GetEyePosition( ), vDirection );
				I::EngineTrace->TraceRay( traceRay, MASK_SHOT, &traceFilter, &trace );
				peekStart = trace.startpos;
				peekVector = trace.endpos - trace.startpos;
				hasDirection = true;
			}
		}

		// Should we peek?
		if ( !Vars::Misc::CL_Move::AutoPeekFree.var && hasDirection && !g_GlobalInfo.m_nWaitForShift )
		{
			bool targetFound = false;
			for ( int i = 10; i < 100; i += 10 )
			{
				const float step = i / 100.f;
				Vec3 currentPos = peekStart + ( peekVector * step );
				if ( CanAttack( pLocal, currentPos ) )
				{
					Utils::WalkTo( pCmd, pLocal, currentPos );
					targetFound = true;
				}

				if ( targetFound )
				{
					//I::DebugOverlay->AddLineOverlayAlpha( PeekReturnPos, currentPos, 68, 189, 50, 100, false, 0.04f );
					break;
				}

				//I::DebugOverlay->AddLineOverlayAlpha( PeekReturnPos, currentPos, 235, 59, 90, 100, false, 0.04f );
			}

			if ( !targetFound ) { isReturning = true; }
		}

		// We've just attacked. Let's return!
		if ( (g_GlobalInfo.lateUserCmd->buttons & IN_ATTACK ) || g_GlobalInfo.m_bAttacking )
		{
			if ( !g_GlobalInfo.m_bShouldShift )
				isReturning = true;
		}

		if ( isReturning )
		{
			if ( localPos.DistTo( PeekReturnPos ) < 7.f )
			{
				// We reached our destination. Recharge DT if wanted
				/*if ( Vars::Misc::CL_Move::AutoRecharge.Value && isReturning && !G::ShouldShift && !G::ShiftedTicks )
				{
					G::RechargeQueued = true;
				}*/
				isReturning = false;
				return;
			}

			Utils::WalkTo( pCmd, pLocal, PeekReturnPos );
		}
	}
	else
	{
		posPlaced = isReturning = hasDirection = false;
		PeekReturnPos = Vec3( );
	}
}

void CMisc::FastStop( CUserCmd *pCmd, CTFPlayer *pLocal )
{
	// 1<<17 = TFCond_Charging

	if ( pLocal && !pLocal->deadflag( ) && !pLocal->InCond( TF_COND_SHIELD_CHARGE ) && !pLocal->InCond( TF_COND_TAUNTING ) && !pLocal->InCond( TF_COND_STUNNED ) && pLocal->GetVelocity( ).Length2D( ) > 10.f ) {
		const int stopType = (
			g_GlobalInfo.m_bShouldShift && g_GlobalInfo.m_nShifted && Vars::Misc::CL_Move::AntiWarp.var ?
			pLocal->OnSolid( ) ? 1 : 2 : 0
			); // 0.none, 1.ground, 2.not ground
		static Vec3 predEndPoint = {};
		static Vec3 currentPos{};
		static int nShiftTickG = 0;
		static int nShiftTickA = 0;

		switch ( stopType ) {
		case 0: {
			nShiftTickG = 0;
			nShiftTickA = 0;
			return;
		}
		case 1: {
			/*if ( pLocal->m_iClass( ) == SCOUT )
			{
				predEndPoint = pLocal->m_vecOrigin( ) + pLocal->m_vecVelocity( );
				currentPos = pLocal->m_vecOrigin( );
				Utils::WalkTo( pCmd, pLocal, predEndPoint, currentPos, 0.275f );
				return;
			}
			else
			{*/
				switch ( nShiftTickG ) {
					case 0: {
						predEndPoint = pLocal->m_vecOrigin( ) + pLocal->m_vecVelocity( );
						nShiftTickG++;
						break;
					}
					case 1: {
						g_GlobalInfo.m_bShouldStop = true;
						nShiftTickG++;
						break;
					}
					default: {
						nShiftTickG++;
						break;
					}
				}//

				currentPos = pLocal->m_vecOrigin( );
				Utils::WalkTo( pCmd, pLocal, predEndPoint, currentPos, ( 1.f / currentPos.Dist2D( predEndPoint ) ) );
				//Utils::WalkTo( pCmd, pLocal, predEndPoint, currentPos, ( ( ( ( pLocal->m_vecVelocity( ).x + pLocal->m_vecVelocity( ).y ) / 2 ) / pLocal->CalculateMaxSpeed( ) ) / currentPos.Dist2D( predEndPoint ) ) );
				//	the "slight stop" that u can see when we do this is due to (i believe) the player reaching the desired point, and then constantly accelerating backwards, meaning their velocity-
				//	when they finish shifting ticks, is lower than when they started.
				//	alot of things worked better than (1/dist) as the scale, but caused issues on different classes, for now this is the best I can get it to.
				return;
			//}
		}
		case 2: {
			switch ( nShiftTickA ) {
			case 0: {
				predEndPoint = pLocal->m_vecOrigin( );
				nShiftTickA++;
				break;
			}
			default: {
				nShiftTickA++;
				break;
			}
			}

			currentPos = pLocal->m_vecOrigin( );
			Utils::WalkTo( pCmd, pLocal, predEndPoint, currentPos, 500 );
			return;
		}
		default: {
			return;
		}
		}
	}
}

bool CMisc::RemoveScope(int nPanel)
{
	const auto& pLocal = g_EntityCache.GetLocal( );

	if (!m_nHudZoom && Hash::IsHudScope(I::Panel->GetName(nPanel)))
		m_nHudZoom = nPanel;

	if ( pLocal && !pLocal->IsDormant( ) && !pLocal->deadflag( ) && Vars::Visuals::RemoveScope.var && pLocal->m_iClass( ) == SNIPER && nPanel == m_nHudZoom )
		return true;

	return false;
}

void CMisc::AutoScoutJump( CUserCmd *pCmd, CTFPlayer *pLocal )
{
	if ( !Vars::Misc::ScoutJump.var ) { return; }

	static bool shot = false;
	static CInputHelper scoutjump;
	scoutjump.Update( );
	if ( Utils::IsGameWindowInFocus( ) && scoutjump.IsPressed( Vars::Misc::ScoutJumpKey.var ) && !I::Surface->IsCursorVisible( ) )
	{
		if ( pLocal && !pLocal->deadflag( ) && !pLocal->IsDormant( ) && pLocal->m_iClass( ) == SCOUT && pLocal->IsOnGround( ) )
		{
			const auto &pWeapon = g_EntityCache.GetWeapon( );
			if ( pWeapon->m_iItemDefinitionIndex( ) == Scout_m_ForceANature || pWeapon->m_iItemDefinitionIndex( ) == Scout_m_FestiveForceANature )
			{
				Vec3 vAngle = { 42.f, pCmd->viewangles.y, 0.0f };
				g_GlobalInfo.m_bScoutJumping = true;
				if ( !shot )
				{
					pCmd->buttons |= IN_JUMP;
					pCmd->buttons |= IN_ATTACK;
					shot = true;
				}
				shot = false;

				Utils::FixMovement( pCmd, vAngle );
				pCmd->viewangles = vAngle;
			}
		}
	}
}

bool CMisc::TauntControl( CUserCmd *pCmd )
{
	bool bReturn = true;
	// Handle Taunt Slide
	const auto &pLocal = g_EntityCache.GetLocal( );
	if ( pLocal && !pLocal->deadflag( ) && !pLocal->IsDormant( ) )
	{
		static CInputHelper spinKey;
		spinKey.Update( );
		if ( pLocal->InCond( TF_COND_TAUNTING ) )
		{
			if ( Vars::Misc::TauntSpin.var )
			{
				if ( Utils::IsGameWindowInFocus( ) && ( spinKey.IsHeld( Vars::Misc::TauntSpinKey.var ) || spinKey.IsPressedAndHeld( Vars::Misc::TauntSpinKey.var ) && !I::Surface->IsCursorVisible( ) ) )
				{
					if ( m_flSpinYaw > 180 )
					{
						m_flSpinYaw = -180;
					}
					m_flSpinYaw += Vars::Misc::TauntSpinSpeed.var;

					pCmd->viewangles.y += m_flSpinYaw;
					pCmd->viewangles.x = 90.f;

					bReturn = false;
				}
			}

			if ( Vars::Misc::TauntSlide.var )
			{
				if ( Vars::Misc::TauntControl.var )
				{
					if ( pCmd->buttons & IN_BACK )
					{
						pCmd->viewangles.x = 91.0f;
					}
					else if ( pCmd->buttons & IN_FORWARD )
					{
						pCmd->viewangles.x = 0.0f;
					}
					else
					{
						pCmd->viewangles.x = 90.0f;
					}
					bReturn = false;
				}

				if ( Vars::Misc::TauntFollowsCamera.var && ( !spinKey.IsHeld( Vars::Misc::TauntSpinKey.var ) && !spinKey.IsPressedAndHeld( Vars::Misc::TauntSpinKey.var ) ) )
				{
					Vec3 vAngle = I::EngineClient->GetViewAngles( );
					pCmd->viewangles.y = vAngle.y;

					bReturn = false;
				}

				bReturn = false;
			}
		}
	}

	return bReturn;
}

void CMisc::ThirdPerson(CViewSetup* pView)
{
	const auto& pLocal = g_EntityCache.GetLocal( );
	
	if ( pLocal && !pLocal->deadflag( ) && !pLocal->IsDormant( ) && pView)
	{
		static bool bToggle = false;
		const bool bIsInTaunt = pLocal->InCond( TF_COND_TAUNTING );
		const bool bIsInThirdPerson = I::Input->CAM_IsThirdPerson( );

		if (!I::EngineVGui->IsGameUIVisible() && !I::Surface->IsCursorVisible())
		{
			if (Utils::IsGameWindowInFocus() && GetAsyncKeyState(Vars::Visuals::ThirdPersonKey.var) & 1)
			{
				if ( !bIsInTaunt )
					bToggle = !bToggle;
			}
		}

		if ( bIsInTaunt )
		{
			bToggle = true;
		}

		if (!bToggle)
		{
			if (bIsInThirdPerson)
			{
				I::Input->CAM_ToFirstPerson( );
				//pLocal->m_nForceTauntCam( ) = 0;
			}
			return;
		}

		if (!bIsInThirdPerson)
		{
			I::Input->CAM_ToThirdPerson( );
			//pLocal->m_nForceTauntCam( ) = 1;
			if ( bIsInTaunt )
			{
				pLocal->m_nForceTauntCam( ) = bToggle;
			}
		}

		if ( bIsInThirdPerson )
		{
			I::Prediction->SetLocalViewAngles( g_GlobalInfo.m_vRealViewAngles );
			if ( const auto &pAnimState = pLocal->GetAnimState( ) )
			{
				pAnimState->m_flCurrentFeetYaw = g_GlobalInfo.m_vRealViewAngles.y;
			}

			/*if ( Vars::Visuals::ThirdpersonOffset.var )
			{
				const Vec3 viewangles = I::EngineClient->GetViewAngles( ); // Use engine view angles so anti aim doesn't make your camera go crazy mode
				Vec3 vForward, vRight, vUp;
				Utils::AngleVectors( viewangles, &vForward, &vRight, &vUp );
				pView->origin += vRight * Vars::Visuals::ThirdpersonRight.var;
				pView->origin += vUp * Vars::Visuals::ThirdpersonUp.var;
				pView->origin += vForward * Vars::Visuals::ThirdpersonDist.var;
			}*/
		}

		Vector forward{ }, right{ }, up{ };
		Utils::AngleVectors( pView->angles, &forward, &right, &up );

		float distance = Vars::Visuals::ThirdpersonOffset.var ? Vars::Visuals::ThirdpersonDist.var : bIsInTaunt ? 0.f : 150.f;
		Vector offset = ( forward * distance ) - ( right * Vars::Visuals::ThirdpersonRight.var ) - ( up * Vars::Visuals::ThirdpersonUp.var );
		Vector delta = pView->origin - offset;
		Ray_t ray;
		ray.Init( pView->origin, delta, { -10.f, -10.f, -10.f }, { 10.f, 10.f, 10.f } );
		CTraceFilterWorldAndPropsOnly Filter = { };
		trace_t Trace = {};
		I::EngineTrace->TraceRay( ray, MASK_SOLID, &Filter, &Trace );

		if ( bIsInThirdPerson )
		{
			pView->origin -= offset * Trace.fraction;
		}
	}
}

void CMisc::FOV(CViewSetup* pView)
{
	const auto& pLocal = g_EntityCache.GetLocal( );
	
	if (pLocal && pView)
	{
		if (pLocal->m_iObserverMode() == OBS_MODE_FIRSTPERSON || ( pLocal->InCond( TF_COND_ZOOMED ) && !Vars::Visuals::RemoveScope.var ) )
			return;

		pView->fov = Vars::Visuals::fov.var;

		if (!pLocal->deadflag()) {
			pLocal->m_iFOV() = Vars::Visuals::fov.var;
			pLocal->m_iDefaultFOV() = Vars::Visuals::fov.var;
		}
	}
}

void CMisc::WeaponSway()
{
	static ConVar* cl_wpn_sway_interp = g_ConVars.FindVar("cl_wpn_sway_interp");
	if (cl_wpn_sway_interp)
	{
		if (Vars::Visuals::ViewmodelSway.var)
		{
			cl_wpn_sway_interp->SetValue(0.05f);
		}
		else
		{
			if (cl_wpn_sway_interp->GetFloat())
			{
				cl_wpn_sway_interp->SetValue(0.0f);
			}
		}
	}
}

void CMisc::ServerHitbox( CTFPlayer* pLocal)
{
	// draw our serverside hitbox on local servers, used to test fakelag & antiaim
	if (I::Input->CAM_IsThirdPerson() && Vars::Visuals::ThirdPersonServerHitbox.var)
	{
		//	i have no idea what this is
		using GetServerAnimating_t = void* (*)(int);
		static auto GetServerAnimating = reinterpret_cast<GetServerAnimating_t>(g_Pattern.Find(L"server.dll", L"55 8B EC 8B 55 ? 85 D2 7E ? A1"));

		using DrawServerHitboxes_t = void(__thiscall*)(void*, float, bool);	// C_BaseAnimating, Duration, MonoColour
		static auto DrawServerHitboxes = reinterpret_cast<DrawServerHitboxes_t>(g_Pattern.Find(L"server.dll", L"55 8B EC 83 EC ? 57 8B F9 80 BF ? ? ? ? ? 0F 85 ? ? ? ? 83 BF ? ? ? ? ? 75 ? E8 ? ? ? ? 85 C0 74 ? 8B CF E8 ? ? ? ? 8B 97"));

		
		if (pLocal && !pLocal->deadflag())
		{
			void* server_animating = GetServerAnimating(pLocal->entindex());
			if (server_animating)
			{
				DrawServerHitboxes(server_animating, I::GlobalVars->interval_per_tick * 2.f, true);
			}
		}
	}
}

void CMisc::CheatsBypass()
{
	static bool cheatset = false;
	if (ConVar* sv_cheats = g_ConVars.FindVar("sv_cheats")) {
		if (Vars::Misc::CheatsBypass.var && sv_cheats)
		{
			sv_cheats->SetValue(1);
			cheatset = true;
		}
		else
		{
			if (cheatset)
			{
				sv_cheats->SetValue(0);
				cheatset = false;
			}
		}
	}
}

void CMisc::FixOutlines( )
{
	static bool glowset = false;
	if ( ConVar *tf_enable_glows_after_respawn = g_ConVars.FindVar( "tf_enable_glows_after_respawn" ) )
	{
		if ( Vars::Outline::Main::Enabled.var && tf_enable_glows_after_respawn )
		{
			tf_enable_glows_after_respawn->SetValue( 0 );
			glowset = true;
		}
		else
		{
			if ( glowset )
			{
				tf_enable_glows_after_respawn->SetValue( 1 );
				glowset = true;
			}
		}
	}
}

void CMisc::Console( )
{
	if ( ConVar *net_graph = g_ConVars.FindVar( "net_graph" ) )
	{
		if ( ConVar *net_graphproportionalfont = g_ConVars.FindVar( "net_graphproportionalfont" ) )
		{
			if ( net_graph->GetInt( ) > 0 && net_graphproportionalfont->GetInt( ) < 1 && !I::EngineVGui->IsGameUIVisible( ) )
			{
				g_GlobalInfo.console = true;
			}
			else
			{
				g_GlobalInfo.console = false;
			}
		}
	}
}

void CMisc::Viewmodel( )
{
	static bool flip = false;
	ConVar *cl_flipviewmodels = g_ConVars.FindVar( "cl_flipviewmodels" );
	if ( cl_flipviewmodels )
	{
		if ( Vars::Visuals::ViewmodelFlip.var )
		{
			if ( !cl_flipviewmodels->GetInt( ) )
			{
				cl_flipviewmodels->SetValue( 1 );
				flip = true;
			}
		}
		else
		{
			if ( flip || cl_flipviewmodels->GetInt( ) )
			{
				cl_flipviewmodels->SetValue( 0 );
				flip = false;
			}
		}
	}

	static bool bSet = false;
	ConVar *viewmodel_fov = g_ConVars.FindVar( "viewmodel_fov" );
	if ( viewmodel_fov )
	{
		if ( !bSet )
		{
			Vars::Visuals::ViewmodelFOV.var = viewmodel_fov->GetInt( );
			ViewFOV = viewmodel_fov->GetInt( );
		}

		viewmodel_fov->SetValue( Vars::Visuals::ViewmodelFOV.var );
		bSet = true;
	}

	static bool bToggle = false;
	ConVar *r_aspectratio = g_ConVars.FindVar( "r_aspectratio" );
	if ( r_aspectratio )
	{
		if ( !bToggle )
		{
			Vars::Visuals::AspectRatio.var = r_aspectratio->GetFloat( );
			AspectRatio = r_aspectratio->GetFloat( );
		}

		r_aspectratio->SetValue( Vars::Visuals::AspectRatio.var );
		bToggle = true;
	}
}

void CMisc::SteamRPC( )
{
	if ( !Vars::Misc::Steam::EnableRPC.var )
	{
		if ( SteamCleared == false ) //stupid way to return back to normal rpc
		{
			I::Friends015->SetRichPresence( "steam_display", "" );
			//this will only make it say "Team Fortress 2" until the player leaves/joins some server. its bad but its better than making 1000 checks to recreate the original
			SteamCleared = true;
		}
		return;
	}

	SteamCleared = false;
	I::Friends015->SetRichPresence( "steam_display", "#TF_RichPresence_Display" );

	/*
	"TF_RichPresence_State_MainMenu"              "Main Menu"
	"TF_RichPresence_State_SearchingGeneric"      "Searching for a Match"
	"TF_RichPresence_State_SearchingMatchGroup"   "Searching - %matchgrouploc_token%"
	"TF_RichPresence_State_PlayingGeneric"        "In Match - %currentmap%"
	"TF_RichPresence_State_LoadingGeneric"        "Joining Match"
	"TF_RichPresence_State_PlayingMatchGroup"     "%matchgrouploc_token% - %currentmap%" <--!!!! used
	"TF_RichPresence_State_LoadingMatchGroup"     "Joining %matchgrouploc_token%"
	"TF_RichPresence_State_PlayingCommunity"      "Community - %currentmap%"
	"TF_RichPresence_State_LoadingCommunity"      "Joining Community Server"
	*/
	if ( !I::EngineClient->IsInGame( ) && Vars::Misc::Steam::OverrideMenu.var )
	{
		I::Friends015->SetRichPresence( "state", "MainMenu" );
	}
	else
	{
		I::Friends015->SetRichPresence( "state", "PlayingMatchGroup" );

		switch ( Vars::Misc::Steam::MatchGroup.var )
		{
			case 0:
				I::Friends015->SetRichPresence( "matchgrouploc", "SpecialEvent" );
				break;
			case 1:
				I::Friends015->SetRichPresence( "matchgrouploc", "MannUp" );
				break;
			case 2:
				I::Friends015->SetRichPresence( "matchgrouploc", "Competitive6v6" );
				break;
			case 3:
				I::Friends015->SetRichPresence( "matchgrouploc", "Casual" );
				break;
			case 4:
				I::Friends015->SetRichPresence( "matchgrouploc", "BootCamp" );
				break;
			default:
				I::Friends015->SetRichPresence( "matchgrouploc", "SpecialEvent" );
				break;
		}
	}

	/*
	"TF_RichPresence_MatchGroup_Competitive6v6"   "Competitive"
	"TF_RichPresence_MatchGroup_Casual"           "Casual"
	"TF_RichPresence_MatchGroup_SpecialEvent"     "Special Event"
	"TF_RichPresence_MatchGroup_MannUp"           "MvM Mann Up"
	"TF_RichPresence_MatchGroup_BootCamp"         "MvM Boot Camp"
	*/
	switch ( Vars::Misc::Steam::MapText.var )
	{
		case 0:
			if ( Vars::Misc::Steam::CustomText.var.empty( ) )
			{
				I::Friends015->SetRichPresence( "currentmap", "ateris.technology" );
			}
			else
			{
				I::Friends015->SetRichPresence( "currentmap", Vars::Misc::Steam::CustomText.var.c_str( ) );
			}
			break;
		case 1:
			I::Friends015->SetRichPresence( "currentmap", "ateris.technology" );
			break;
		case 2:
			I::Friends015->SetRichPresence( "currentmap", "ateris.tech" );
			break;
		case 3:
			I::Friends015->SetRichPresence( "currentmap", "Ateris V2" );
			break;
		case 4:
			I::Friends015->SetRichPresence( "currentmap", "rijin.solutions" );
			break;
		case 5:
			I::Friends015->SetRichPresence( "currentmap", "(Visuals Only)" );
			break;
		case 6:
			I::Friends015->SetRichPresence( "currentmap", "ateris.technology (Visuals Only)" );
			break;
		default:
			I::Friends015->SetRichPresence( "currentmap", "ateris.technology" );
			break;
	}

	I::Friends015->SetRichPresence( "steam_player_group_size",
									std::to_string( Vars::Misc::Steam::GroupSize.var ).c_str( ) );
}

void CMisc::UnlockAchievements( )
{
	using FN = IAchievementMgr * ( * )( void );
	const auto achievementmgr = I::EngineClient->GetAchievementMgr( );
	if ( achievementmgr )
	{
		I::UserStats->RequestCurrentStats( );
		for ( int i = 0; i < achievementmgr->GetAchievementCount( ); i++ )
		{
			achievementmgr->AwardAchievement( achievementmgr->GetAchievementByIndex( i )->GetAchievementID( ) );
		}
		I::UserStats->StoreStats( );
		I::UserStats->RequestCurrentStats( );
	}
}

void CMisc::LockAchievements( )
{
	using FN = IAchievementMgr * ( * )( void );
	const auto achievementmgr = GetVFunc<FN>( I::EngineClient, 114 )( );
	if ( achievementmgr )
	{
		I::UserStats->RequestCurrentStats( );
		for ( int i = 0; i < achievementmgr->GetAchievementCount( ); i++ )
		{
			I::UserStats->ClearAchievement( achievementmgr->GetAchievementByIndex( i )->GetName( ) );
		}
		I::UserStats->StoreStats( );
		I::UserStats->RequestCurrentStats( );
	}
}

void CMisc::ManualNetwork( StartSoundParams_t &params )
{
	if ( Vars::ESP::Main::esp_dormant.var ) {
		// Credits go to reestart entirely
		Vec3 vOrigin;
		if ( params.soundsource > 0 ) //Make sure we even have a valid entity to begin with
		{
			Utils::VectorCopy( params.origin, vOrigin );
			int iEntIndex = params.soundsource;
			CTFPlayer *pEntity = I::ClientEntityList->GetClientEntity( iEntIndex )->As<CTFPlayer>( );
			CTFPlayerResource *cResource = g_EntityCache.GetPR( );
			if ( pEntity && iEntIndex != I::EngineClient->GetLocalPlayer( ) && cResource->IsAlive( pEntity->entindex( ) ) &&
				pEntity->IsDormant( ) && pEntity->GetClassId( ) == EClassIds::CTFPlayer )
			{
				pEntity->SetAbsOrigin( vOrigin );
				pEntity->m_iHealth( ) = cResource->GetHealth( pEntity->entindex( ) );
				g_GlobalInfo.PartyPlayerESP[ pEntity->entindex( ) ].LastUpdate = I::EngineClient->Time( );
			}
		}
	}
}

void RenderLine( const Vector &v1, const Vector &v2, Color_t c, bool bZBuffer )
{
	static auto RenderLineFn = reinterpret_cast< void( __cdecl * )( const Vector &, const Vector &, Color_t, bool ) >( g_Pattern.Find( L"engine.dll", L"55 8B EC 81 EC ? ? ? ? 56 E8 ? ? ? ? 8B 0D ? ? ? ? 8B 01 FF 90 ? ? ? ? 8B F0 85 F6" ) );
	RenderLineFn( v1, v2, c, bZBuffer );
}

void CMisc::DrawPrediction( )
{
	if ( !Vars::Aimbot::Projectile::PredictedPath.var )
		return;

	if ( !g_GlobalInfo.PredLinesBackup.empty( ) )
	{
		for ( size_t i = 1; i < g_GlobalInfo.PredLinesBackup.size( ); i++ )
		{
			RenderLine( g_GlobalInfo.PredLinesBackup.at( i - 1 ),
						g_GlobalInfo.PredLinesBackup.at( i ),
						Vars::Colors::PredictedPath, false );

			RenderLine( g_GlobalInfo.PredLinesBackup.at( i ),
						{ g_GlobalInfo.PredLinesBackup.at( i ).x + 10, g_GlobalInfo.PredLinesBackup.at( i ).y, g_GlobalInfo.PredLinesBackup.at( i ).z },
						Vars::Colors::PredictedPath, false );

			RenderLine( { g_GlobalInfo.PredLinesBackup.at( i - 1 ).x + 10, g_GlobalInfo.PredLinesBackup.at( i - 1 ).y, g_GlobalInfo.PredLinesBackup.at( i - 1 ).z },
						{ g_GlobalInfo.PredLinesBackup.at( i ).x + 10, g_GlobalInfo.PredLinesBackup.at( i ).y, g_GlobalInfo.PredLinesBackup.at( i ).z },
						Vars::Colors::PredictedPath, false );

			if ( i == 1 )
			{
				RenderLine( g_GlobalInfo.PredLinesBackup.at( i - 1 ),
							{ g_GlobalInfo.PredLinesBackup.at( i - 1 ).x + 10, g_GlobalInfo.PredLinesBackup.at( i - 1 ).y, g_GlobalInfo.PredLinesBackup.at( i - 1 ).z },
							Vars::Colors::PredictedPath, false );
			}
		}

		static Timer EraseTimer{};
		int timer = Vars::Aimbot::Projectile::PathDuration.var * 1000;
		if ( EraseTimer.Run( timer ) ) {
			g_GlobalInfo.PredLinesBackup.clear( );
		}
	}
}

void CMisc::AutoJump( CUserCmd *pCmd, CTFPlayer *pLocal )
{
	if ( !Vars::Misc::AutoJump.var
		 || pLocal->deadflag( )
		 || pLocal->IsSwimming( )
		 || pLocal->InCond( TF_COND_HALLOWEEN_KART )
		 || pLocal->InCond( TF_COND_HALLOWEEN_GHOST_MODE ) )
	{
		return;
	}

	if ( pLocal->GetMoveType( ) == MOVETYPE_NOCLIP
		 || pLocal->GetMoveType( ) == MOVETYPE_LADDER
		 || pLocal->GetMoveType( ) == MOVETYPE_OBSERVER )
	{
		return;
	}

	static bool s_bState = false;

	if ( fabsf( pLocal->m_vecVelocity( ).x ) < ( pLocal->CalculateMaxSpeed( ) ) && fabsf( pLocal->m_vecVelocity( ).y ) < ( pLocal->CalculateMaxSpeed( ) ) && fabsf( pLocal->m_vecVelocity( ).x ) + fabsf( pLocal->m_vecVelocity( ).y ) < ( pLocal->CalculateMaxSpeed( ) * 0.8f ) && pLocal->IsOnGround( ) && Vars::Misc::BreakJump.var )
	{
		pCmd->buttons &= ~IN_JUMP;
		if ( ( GetAsyncKeyState( VK_SPACE ) && Utils::IsGameWindowInFocus( ) && !I::EngineVGui->IsGameUIVisible( ) && !I::Surface->IsCursorVisible( ) ) && ( pCmd->buttons != IN_FORWARD && pCmd->buttons != IN_BACK && pCmd->buttons != IN_LEFT && pCmd->buttons != IN_RIGHT ) )
		{
			pCmd->forwardmove = 450.f;
		}
	}
	else
	{
		if ( pCmd->buttons & IN_JUMP )
		{
			if ( !s_bState && !pLocal->OnSolid( ) )
			{
				pCmd->buttons &= ~IN_JUMP;
			}
			else if ( s_bState )
			{
				s_bState = false;
			}
			const auto &pWeapon = g_EntityCache.GetWeapon( );

			if ( Vars::Misc::TomislavRev.var && pWeapon && pWeapon->m_iItemDefinitionIndex( ) == Heavy_m_Tomislav )
			{
				if ( pLocal->IsOnGround( ) )
					pCmd->buttons |= IN_ATTACK2;
				else
					pCmd->buttons &= ~IN_ATTACK2;
			}
		}
		else if ( !s_bState )
		{
			s_bState = true;
		}

		if ( Vars::Misc::BreakJump.var )
		{
			static Timer cmdTimer{};
			if ( !pLocal->OnSolid( ) ) {
				pCmd->buttons |= IN_DUCK;

			}
			if ( !pLocal->OnSolid( ) && cmdTimer.Run( 20 ) )
			{
				pCmd->buttons &= ~IN_DUCK;
			}
		}
	}
}

void CMisc::AutoStrafe(CUserCmd* pCmd, CTFPlayer* pLocal)
{
	if (!Vars::Misc::AutoStrafe.var)
	{
		return;
	}

	if (pLocal->deadflag()
		|| pLocal->IsSwimming()
		|| pLocal->InCond(TF_COND_HALLOWEEN_KART)
		|| pLocal->InCond(TF_COND_HALLOWEEN_GHOST_MODE)
		|| pLocal->OnSolid())
	{
		return;
	}

	if (pLocal->GetMoveType() == MOVETYPE_NOCLIP
		|| pLocal->GetMoveType() == MOVETYPE_LADDER
		|| pLocal->GetMoveType() == MOVETYPE_OBSERVER)
	{
		return;
	}

	static auto cl_sidespeed = g_ConVars.FindVar("cl_sidespeed");
	if (!cl_sidespeed || !cl_sidespeed->GetFloat())
	{
		return;
	}

	static bool wasJumping = false;
	const bool isJumping = pCmd->buttons & IN_JUMP;

	switch (Vars::Misc::AutoStrafe.var)
	{
	case 1:
	{
		if (pCmd->mousedx && (!isJumping || wasJumping))
		{
			pCmd->sidemove = pCmd->mousedx > 1 ? cl_sidespeed->GetFloat() : -cl_sidespeed->GetFloat();
		}
		wasJumping = isJumping;
		break;
	}

	case 2:
	{
		const float speed = pLocal->GetVelocity().Length2D();

		if (speed < 2.0f)
		{
			return;
		}

		const auto vel = pLocal->GetVelocity();

		constexpr auto perfectDelta = [](float speed) noexcept {
			if (const auto& pLocal = g_EntityCache.GetLocal( ))
			{
				static auto speedVar = pLocal->CalculateMaxSpeed();
				static auto airVar = g_ConVars.FindVar("sv_airaccelerate");
				static auto wishSpeed = 30.0f;

				const auto term = wishSpeed / airVar->GetFloat() / speedVar * 100.f / speed;

				if (term < 1.0f && term > -1.0f)
				{
					return acosf(term);
				}
			}
			return 0.0f;
		};

		const float pDelta = perfectDelta(speed); //1.56892
		if ((!isJumping || wasJumping) && pDelta)
		{
			const float yaw = DEG2RAD(pCmd->viewangles.y);
			const float velDir = atan2f(vel.y, vel.x) - yaw;
			const float wishAng = atan2f(-pCmd->sidemove, pCmd->forwardmove);
			const float delta = Utils::AngleDiffRad(velDir, wishAng);

			const float moveDir = delta < 0.0f ? velDir + pDelta : velDir - pDelta;

			pCmd->forwardmove = cosf(moveDir) * cl_sidespeed->GetFloat();
			pCmd->sidemove = -sinf(moveDir) * cl_sidespeed->GetFloat();
		}
		wasJumping = isJumping;
		break;
	}

	default: break;
	}
}

void CMisc::DetectChoke( )
{
	static int iOldTick = I::GlobalVars->tickcount;
	if ( I::GlobalVars->tickcount == iOldTick ) { return; }
	iOldTick = I::GlobalVars->tickcount;
	for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::PLAYERS_ALL ) )
	{
		const auto &pPlayer = pEntity->As<CTFPlayer>( );
		if ( pPlayer->deadflag( ) || pPlayer->IsDormant( ) )
		{
			g_GlobalInfo.ChokeMap[ pPlayer->entindex( ) ] = 0;
			continue;
		}

		if ( pPlayer->m_flSimulationTime( ) == pPlayer->GetOldSimulationTime( ) )
		{
			g_GlobalInfo.ChokeMap[ pPlayer->entindex( ) ]++;
		}
		else
		{
			g_BadActors.ReportTickCount( { pPlayer, g_GlobalInfo.ChokeMap[ pPlayer->entindex( ) ] } );
			g_GlobalInfo.ChokeMap[ pPlayer->entindex( ) ] = 0;
		}
	}
}

void CMisc::DrawHitboxes( CTFPlayer *pPlayer, Color_t colorface, Color_t coloredge, float time )
{
	//I::DebugOverlay->ClearAllOverlays();

	const model_t *model = pPlayer->GetModel( );
	const studiohdr_t *hdr = I::ModelInfo->GetStudioModel( model );
	const mstudiohitboxset_t *set = hdr->GetHitboxSet( pPlayer->m_nHitboxSet( ) );

	for ( int i{}; i < set->numhitboxes; ++i )
	{
		const mstudiobbox_t *bbox = set->pHitbox( i );
		if ( !bbox )
		{
			continue;
		}

		/*if (bbox->m_radius <= 0.f) {*/
		matrix3x4_t rotMatrix;
		Utils::AngleMatrix( bbox->angle, rotMatrix );

		matrix3x4_t matrix;
		matrix3x4_t boneees[ 128 ];
		pPlayer->SetupBones( boneees, 128, 0x0007FF00, I::GlobalVars->curtime );
		Utils::ConcatTransforms( boneees[ bbox->bone ], rotMatrix, matrix );

		Vec3 bboxAngle;
		Utils::MatrixAngles( matrix, bboxAngle );

		Vec3 matrixOrigin;
		Utils::GetMatrixOrigin( matrix, matrixOrigin );

		I::DebugOverlay->AddBoxOverlay2( matrixOrigin, bbox->bbmin, bbox->bbmax, bboxAngle, colorface, coloredge, time );
	}
}

bool bWorldIsModulated = false;

void ApplyModulation( const Color_t &clr )
{
	for ( MaterialHandle_t h = I::MaterialSystem->FirstMaterial( ); h != I::MaterialSystem->InvalidMaterial( ); h = I::MaterialSystem->NextMaterial( h ) )
	{
		if ( IMaterial* pMaterial = I::MaterialSystem->GetMaterial( h ) )
		{
			if ( pMaterial->IsErrorMaterial( ) )
				continue;

			std::string_view group( pMaterial->GetTextureGroupName( ) );

			if ( group.find( "World textures" ) < group.npos || group.find( "SkyBox textures" ) < group.npos )
			{
				pMaterial->ColorModulate( Utils::CLR2FLT( clr.r ), Utils::CLR2FLT( clr.g ), Utils::CLR2FLT( clr.b ) );
				pMaterial->AlphaModulate( Utils::CLR2FLT( clr.a ) );
			}
		}
	}

	bWorldIsModulated = true;
}

void CMisc::ModulateWorld( )
{
	if ( !Vars::Visuals::WorldModulation.var )
		return;

	ApplyModulation( Vars::Colors::WorldModulation );
}

void CMisc::UpdateWorldModulation( )
{
	if ( !Vars::Visuals::WorldModulation.var ) {
		RestoreWorldModulation( );
		return;
	}

	auto ColorChanged = [ & ] ( ) -> bool
	{
		static Color_t old = Vars::Colors::WorldModulation;
		Color_t cur = Vars::Colors::WorldModulation;

		if ( cur.r != old.r || cur.g != old.g || cur.b != old.b ) {
			old = cur;
			return true;
		}

		return false;
	};

	if ( !bWorldIsModulated || ColorChanged( ) )
		ApplyModulation( Vars::Colors::WorldModulation );
}

void CMisc::RestoreWorldModulation( )
{
	if ( !bWorldIsModulated )
		return;

	ApplyModulation( { 255, 255, 255, 255 } );
	bWorldIsModulated = false;
	ApplyModulation( { 255, 255, 255, 255 } );
}