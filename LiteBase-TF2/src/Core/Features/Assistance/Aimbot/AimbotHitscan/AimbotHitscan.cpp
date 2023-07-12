#include "AimbotHitscan.h"
#include "../../../Vars.h"
#include "../../../../SDK/Include/EntityCache/EntityCache.h"
#include "../../../Exploits/Backtrack/Backtrack.h"

float LerpTime( )
{
	float updaterate = g_ConVars.FindVar( "cl_updaterate" )->GetFloat( );
	ConVar *minupdate = g_ConVars.FindVar( "sv_minupdaterate" );
	ConVar *maxupdate = g_ConVars.FindVar( "sv_maxupdaterate" );

	if ( minupdate && maxupdate )
		updaterate = maxupdate->GetFloat( );

	float ratio = g_ConVars.FindVar( "cl_interp_ratio" )->GetFloat( );

	if ( ratio == 0 )
		ratio = 1.0f;

	float lerp = g_ConVars.FindVar( "cl_interp" )->GetFloat( );
	ConVar *cmin = g_ConVars.FindVar( "sv_client_min_interp_ratio" );
	ConVar *cmax = g_ConVars.FindVar( "sv_client_max_interp_ratio" );

	if ( cmin && cmax && cmin->GetFloat( ) != 1 )
		ratio = std::clamp( ratio, cmin->GetFloat( ), cmax->GetFloat( ) );

	return std::max( lerp, ratio / updaterate );
}

int CAimbotHitscan::GetHitbox(CTFPlayer *pLocal, CTFWeaponBase *pWeapon)
{
	switch (Vars::Aimbot::Hitscan::AimHitbox.var)
	{
		case 0: { return HITBOX_HEAD; }
		case 1: { return HITBOX_PELVIS; }
		case 2:
		{
			int nClassNum = pLocal->m_iClass();

			if (nClassNum == SNIPER)
			{
				if (g_GlobalInfo.m_nCurItemDefIndex != Sniper_m_TheSydneySleeper)
					return (pLocal->InCond(TF_COND_ZOOMED) ? HITBOX_HEAD : HITBOX_PELVIS);

				return HITBOX_PELVIS;
			}

			else if (nClassNum == SPY)
			{
				bool bIsAmbassador = (g_GlobalInfo.m_nCurItemDefIndex == Spy_m_TheAmbassador || g_GlobalInfo.m_nCurItemDefIndex == Spy_m_FestiveAmbassador);
				return (bIsAmbassador ? HITBOX_HEAD : HITBOX_PELVIS);
			}

			else return HITBOX_PELVIS;
		}
		default: return HITBOX_PELVIS;
	}

	return HITBOX_HEAD;
}

ESortMethod CAimbotHitscan::GetSortMethod()
{
	switch (Vars::Aimbot::Hitscan::SortMethod.var) 
	{
		case 0: return ESortMethod::FOV;
		case 1: return ESortMethod::DISTANCE;
		default: return ESortMethod::UNKNOWN;
	}
}

bool CAimbotHitscan::GetTargets(CTFPlayer *pLocal, CTFWeaponBase *pWeapon)
{
	ESortMethod SortMethod = GetSortMethod();

	if (SortMethod == ESortMethod::FOV)
		g_GlobalInfo.m_flCurAimFOV = Vars::Aimbot::Hitscan::AimFOV.var;

	g_AimbotGlobal.m_vecTargets.clear();
	
	Vec3 vLocalPos = pLocal->GetShootPos();
	Vec3 vLocalAngles = I::EngineClient->GetViewAngles();

	if (Vars::Aimbot::Global::AimPlayers.var)
	{
		int nHitbox = GetHitbox(pLocal, pWeapon);
		int nWeaponID = pWeapon->GetWeaponID( );
		bool bIsMedigun = nWeaponID == TF_WEAPON_MEDIGUN;

		for ( const auto& pEntity : g_EntityCache.GetGroup( bIsMedigun ? EEntGroup::PLAYERS_TEAMMATES : EEntGroup::PLAYERS_ENEMIES ) )
		{
			if ( pEntity->GetClassId( ) == EClassIds::CTFPlayer )
			{
				const auto &pPlayer = pEntity->As<CTFPlayer>( );

				if ( !pPlayer || pPlayer->deadflag( ) || pPlayer->IsDormant( ) || pPlayer->InCond( TF_COND_HALLOWEEN_GHOST_MODE ) || pPlayer == pLocal )
					continue;

				if ( Vars::Aimbot::Global::IgnoreInvulnerable.var && ( pPlayer->InCond( TF_COND_INVULNERABLE ) || pPlayer->InCond( TF_COND_PHASE ) ) )
					continue;

				if ( Vars::Aimbot::Global::IgnoreCloaked.var && pPlayer->InCond( TF_COND_STEALTHED ) )
					continue;

				if ( Vars::Aimbot::Global::IgnoreTaunting.var && pPlayer->InCond( TF_COND_TAUNTING ) )
					continue;

				if ( Vars::Aimbot::Global::IgnoreFriends.var && pPlayer->PlayerOnFriendsList( ) && pPlayer->m_iTeamNum( ) != pLocal->m_iTeamNum( ) )
					continue;

				player_info_t pi;
				if ( I::EngineClient->GetPlayerInfo( pPlayer->entindex( ), &pi ) )
				{
					if ( g_GlobalInfo.PlayerPriority[ pi.friendsID ].Mode == 1 )
					{
						continue;
					}
				}

				Vec3 vPos = pPlayer->GetHitboxPos( nHitbox );
				Vec3 vAngleTo = Utils::CalcAngle( vLocalPos, vPos );
				float flFOVTo = SortMethod == ESortMethod::FOV ? Utils::CalcFov( vLocalAngles, vAngleTo ) : 0.0f;
				float flDistTo = SortMethod == ESortMethod::DISTANCE ? vLocalPos.DistTo( vPos ) : 0.0f;

				if ( SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Hitscan::AimFOV.var )
					continue;

				const auto &priority = g_AimbotGlobal.GetPriority( pPlayer->entindex( ) );

				g_AimbotGlobal.m_vecTargets.push_back( { pPlayer, ETargetType::PLAYER, vPos, vAngleTo, flFOVTo, flDistTo, nHitbox, false, priority } );
			}
		}
	}

	if (Vars::Aimbot::Global::AimBuildings.var)
	{
		for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::SENTRY_ENEMIES ) )
		{
			const auto &pObject = pEntity->As<CBaseObject>( );

			if ( !pObject || pObject->m_iTeamNum( ) == pLocal->m_iTeamNum( ) || pObject->IsDormant( ) )
				continue;

			Vec3 vPos = pObject->GetWorldSpaceCenter( );
			Vec3 vAngleTo = Utils::CalcAngle( vLocalPos, vPos );
			float flFOVTo = SortMethod == ESortMethod::FOV ? Utils::CalcFov( vLocalAngles, vAngleTo ) : 0.0f;

			if ( SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Hitscan::AimFOV.var )
				continue;

			float flDistTo = SortMethod == ESortMethod::DISTANCE ? vLocalPos.DistTo( vPos ) : 0.0f;

			g_AimbotGlobal.m_vecTargets.push_back( { pObject, ETargetType::BUILDING, vPos, vAngleTo, flFOVTo, flDistTo } );
		}
	}

	if ( Vars::Aimbot::Global::AimNPCs.var )
	{
		for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::WORLD_NPC ) )
		{
			const auto &pNpc = pEntity->As<CBaseEntity>( );

			if ( !pNpc || pNpc->IsDormant( ) )
				continue;

			Vec3 vPos = pNpc->GetWorldSpaceCenter( );
			Vec3 vAngleTo = Utils::CalcAngle( vLocalPos, vPos );
			float flFOVTo = SortMethod == ESortMethod::FOV ? Utils::CalcFov( vLocalAngles, vAngleTo ) : 0.0f;

			if ( SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Hitscan::AimFOV.var )
				continue;

			float flDistTo = SortMethod == ESortMethod::DISTANCE ? vLocalPos.DistTo( vPos ) : 0.0f;

			g_AimbotGlobal.m_vecTargets.push_back( { pNpc, ETargetType::BUILDING, vPos, vAngleTo, flFOVTo, flDistTo } );
		}
	}

	if ( Vars::Aimbot::Global::AimStickies.var )
	{
		for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::WORLD_PROJECTILES ) )
		{
			const auto &pProjectile = pEntity->As<CBaseEntity>( );
			if ( pProjectile->GetPipebombType( ) != TYPE_STICKY )
			{
				continue;
			}

			const auto &pOwner = GET_ENT_H( pProjectile->As<CBaseGrenade>( )->m_hThrower( ) )->As<CTFPlayer>( );
			if ( !pOwner )
			{
				continue;
			}

			if ( !pProjectile->As<CTFGrenadePipebombProjectile>( )->m_bTouched( ) || ( pOwner->m_iTeamNum( ) == pLocal->m_iTeamNum( ) ) )
			{
				continue;
			}

			Vec3 vPos = pProjectile->GetWorldSpaceCenter( );
			Vec3 vAngleTo = Utils::CalcAngle( vLocalPos, vPos );
			float flFOVTo = SortMethod == ESortMethod::FOV ? Utils::CalcFov( vLocalAngles, vAngleTo ) : 0.0f;

			if ( SortMethod == ESortMethod::FOV && flFOVTo > Vars::Aimbot::Hitscan::AimFOV.var )
				continue;

			float flDistTo = SortMethod == ESortMethod::DISTANCE ? vLocalPos.DistTo( vPos ) : 0.0f;

			g_AimbotGlobal.m_vecTargets.push_back( { pProjectile, ETargetType::STICKY, vPos, vAngleTo, flFOVTo, flDistTo } );
		}
	}

	return !g_AimbotGlobal.m_vecTargets.empty();
}

bool CAimbotHitscan::ScanHitboxes(CTFPlayer *pLocal, Target_t &Target)
{
	if (Target.m_TargetType == ETargetType::PLAYER)
	{
		if (!Vars::Aimbot::Hitscan::ScanHitboxes.var)
			return false;
	}

	else if (Target.m_TargetType == ETargetType::BUILDING)
	{
		if (!Vars::Aimbot::Hitscan::ScanBuildings.var)
			return false;
	}

	Vec3 vLocalPos = pLocal->GetShootPos();

	for (int nHitbox = Target.m_TargetType == ETargetType::PLAYER ? 1 : 0; nHitbox < Target.m_pEntity->As<CTFPlayer>( )->GetNumOfHitboxes(); nHitbox++)
	{
		if (Target.m_TargetType == ETargetType::PLAYER && nHitbox == HITBOX_PELVIS)
			continue;

		Vec3 vHitbox = Target.m_pEntity->As<CTFPlayer>()->GetHitboxPos(nHitbox);
		//I::DebugOverlay->AddTextOverlay( vHitbox, 0.09f, "x" );
		if (Utils::VisPos(pLocal, Target.m_pEntity, vLocalPos, vHitbox)) {
			Target.m_vPos = vHitbox;
			Target.m_vAngleTo = Utils::CalcAngle(vLocalPos, vHitbox);
			return true;
		}
	}

	return false;
}

bool CAimbotHitscan::ScanHead(CTFPlayer *pLocal, Target_t &Target)
{
	if (!Vars::Aimbot::Hitscan::ScanHead.var)
		return false;

	auto pModel = Target.m_pEntity->GetModel();
	if (!pModel)
		return false;

	studiohdr_t *pHDR = reinterpret_cast<studiohdr_t*>(I::ModelInfo->GetStudioModel(pModel));
	if (!pHDR)
		return false;

	matrix3x4_t BoneMatrix[128];
	if (!Target.m_pEntity->SetupBones(BoneMatrix, 128, 0x100, I::GlobalVars->curtime))
		return false;

	mstudiohitboxset_t *pSet = pHDR->GetHitboxSet(Target.m_pEntity->As<CTFPlayer>( )->m_nHitboxSet());
	if (!pSet)
		return false;

	mstudiobbox_t *pBox = pSet->pHitbox(HITBOX_HEAD);
	if (!pBox)
		return false;

	Vec3 vLocalPos = pLocal->GetShootPos();
	Vec3 vMins = pBox->bbmin;
	Vec3 vMaxs = pBox->bbmax;

	const float fScale = 0.8f;
	const std::vector<Vec3> vecPoints = {
		Vec3(((vMins.x + vMaxs.x) * 0.5f), (vMins.y * fScale), ((vMins.z + vMaxs.z) * 0.5f)),
		Vec3((vMins.x * fScale), ((vMins.y + vMaxs.y) * 0.5f), ((vMins.z + vMaxs.z) * 0.5f)),
		Vec3((vMaxs.x * fScale), ((vMins.y + vMaxs.y) * 0.5f), ((vMins.z + vMaxs.z) * 0.5f))
	};

	for (const auto &Point : vecPoints)
	{
		Vec3 vTransformed = {};
		Utils::VectorTransform(Point, BoneMatrix[pBox->bone], vTransformed);

		if (Utils::VisPosHitboxId(pLocal, Target.m_pEntity, vLocalPos, vTransformed, HITBOX_HEAD)) {
			Target.m_vPos = vTransformed;
			Target.m_vAngleTo = Utils::CalcAngle(vLocalPos, vTransformed);
			Target.m_bHasMultiPointed = true;
			return true;
		}
	}

	return false;
}

bool CAimbotHitscan::ScanBuildings(CTFPlayer *pLocal, Target_t &Target)
{
	if (!Vars::Aimbot::Hitscan::ScanBuildings.var)
		return false;

	Vec3 vLocalPos = pLocal->GetShootPos();

	Vec3 vMins = Target.m_pEntity->m_vecMins();
	Vec3 vMaxs = Target.m_pEntity->m_vecMaxs();

	const std::vector<Vec3> vecPoints = {	
		Vec3(vMins.x * 0.9f, ((vMins.y + vMaxs.y) * 0.5f), ((vMins.z + vMaxs.z) * 0.5f)),
		Vec3(vMaxs.x * 0.9f, ((vMins.y + vMaxs.y) * 0.5f), ((vMins.z + vMaxs.z) * 0.5f)),
		Vec3(((vMins.x + vMaxs.x) * 0.5f), vMins.y * 0.9f, ((vMins.z + vMaxs.z) * 0.5f)),
		Vec3(((vMins.x + vMaxs.x) * 0.5f), vMaxs.y * 0.9f, ((vMins.z + vMaxs.z) * 0.5f)),
		Vec3(((vMins.x + vMaxs.x) * 0.5f), ((vMins.y + vMaxs.y) * 0.5f), vMins.z * 0.9f),
		Vec3(((vMins.x + vMaxs.x) * 0.5f), ((vMins.y + vMaxs.y) * 0.5f), vMaxs.z * 0.9f)
	};

	const matrix3x4_t &Transform = Target.m_pEntity->RenderableToWorldTransform( );

	for (const auto &Point : vecPoints)
	{
		Vec3 vTransformed = {};
		Utils::VectorTransform(Point, Transform, vTransformed);
		//I::DebugOverlay->AddTextOverlay( vTransformed, 0.09f, "x" );
		if (Utils::VisPos(pLocal, Target.m_pEntity, vLocalPos, vTransformed)) {
			Target.m_vPos = vTransformed;
			Target.m_vAngleTo = Utils::CalcAngle(vLocalPos, vTransformed);
			return true;
		}
	}

	return false;
}

bool CAimbotHitscan::VerifyTarget(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, Target_t &Target)
{
	switch (Target.m_TargetType)
	{
		case ETargetType::PLAYER:
		{
			if ( Vars::Backtrack::Active.var )
			{
				if ( std::optional<TickRecord> ValidRecord = g_Backtrack.Aimbot( Target.m_pEntity->As<CTFPlayer>( ), BacktrackMode::LAST, Target.m_nAimedHitbox ) )
				{
					Target.SimTime = ValidRecord->flSimTime;
					Target.m_vAngleTo = Utils::CalcAngle( pLocal->GetShootPos( ), Target.m_pEntity->As<CTFPlayer>( )->GetHitboxPosMatrix( Target.m_nAimedHitbox, ( matrix3x4_t * )( &ValidRecord->BoneMatrix.BoneMatrix ) ) );
					Target.ShouldBacktrack = true;
					return true;
				}
			}

			if (Target.m_nAimedHitbox == HITBOX_HEAD)
			{
				int nHit = -1;

				if (!Utils::VisPosHitboxIdOut(pLocal, Target.m_pEntity, pLocal->GetShootPos(), Target.m_vPos, nHit))
					return false;

				if (nHit != HITBOX_HEAD && !ScanHead(pLocal, Target))
					return false;
			}

			else if (Target.m_nAimedHitbox == HITBOX_PELVIS)
			{
				if ( !Utils::VisPos( pLocal, Target.m_pEntity, pLocal->GetShootPos( ), Target.m_vPos ) && !ScanHitboxes( pLocal, Target ) )
				{
					return false;
				}
			}

			break;
		}

		case ETargetType::BUILDING:
		{
			if (!Utils::VisPos(pLocal, Target.m_pEntity, pLocal->GetShootPos(), Target.m_vPos))
			{
				//Sentryguns have hitboxes, it's better to use ScanHitboxes for them
				if (Target.m_pEntity->GetClassId() == EClassIds::CObjectSentrygun ? !ScanHitboxes(pLocal, Target) : !ScanBuildings(pLocal, Target))
					return false;
			}

			break;
		}

		case ETargetType::STICKY:
		{
			if ( !Utils::VisPos( pLocal, Target.m_pEntity, pLocal->GetShootPos( ), Target.m_vPos ) )
			{
				return false;
			}

			/*const auto &pInfo = pWeapon->GetWeaponInfo( );
			static int currentclip = pWeapon->m_iClip1( );
			if ( pWeapon->m_iClip2( ) > 0 )
			{
				currentclip += pWeapon->m_iClip2( );
			}

			static int maxclip = pInfo->iDefaultClip1;
			if ( pInfo->iDefaultClip2 > 0 )
			{
				maxclip += pInfo->iDefaultClip2;
			}
			I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "m_iClip1: %i,\nm_iClip2: %i,\nmax1: %i,\nmax2: %i,\ncurrentclip: %i,\n,maxclip: %i\n",
										 pWeapon->m_iClip1( ), pWeapon->m_iClip2( ), pInfo->iMaxClip1, pInfo->iMaxClip2, currentclip, maxclip );
			const float ratio = ( float )currentclip / maxclip;
			I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "ratio: %.2f\n", ratio );
			if ( ( ( ratio * 100.f ) < Vars::Aimbot::Global::AmmoCount.var ) && Vars::Aimbot::Global::AmmoCheck.var )
			{
				return false;
			}*/

			break;
		}

		default:
		{
			if (!Utils::VisPos(pLocal, Target.m_pEntity, pLocal->GetShootPos(), Target.m_vPos))
				return false;

			break;
		}
	}

	return true;
}

bool CAimbotHitscan::GetTarget(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, Target_t &Out)
{
	if ( !GetTargets( pLocal, pWeapon ) )
	{
		return false;
	}

	g_AimbotGlobal.SortTargets(GetSortMethod());

	for (auto &Target : g_AimbotGlobal.m_vecTargets)
	{
		if (!VerifyTarget(pLocal, pWeapon, Target))
			continue;

		Out = Target;
		return true;
	}
	return false;
}

void CAimbotHitscan::Aim(CUserCmd *pCmd, Vec3 &vAngle)
{
	vAngle -= g_GlobalInfo.m_vPunchAngles;
	Utils::ClampAngles(vAngle);

	int nAimMethod = (Vars::Aimbot::Hitscan::SpectatedSmooth.var && g_GlobalInfo.m_bLocalSpectated) ? 1 : Vars::Aimbot::Hitscan::AimMethod.var;

	switch (nAimMethod)
	{
		case 0: //Plain
		{
			pCmd->viewangles = vAngle;
			I::EngineClient->SetViewAngles(pCmd->viewangles);
			break;
		}

		case 1: //Smooth
		{
			//Calculate delta of current viewangles and wanted angles
			Vec3 vecDelta = vAngle - I::EngineClient->GetViewAngles();

			//Clamp, keep the angle in possible bounds
			Utils::ClampAngles(vecDelta);

			//Basic smooth by dividing the delta by wanted smooth amount
			pCmd->viewangles += vecDelta / Vars::Aimbot::Hitscan::SmoothingAmount.var;

			//Set the viewangles from engine
			I::EngineClient->SetViewAngles(pCmd->viewangles);
			break;
		}

		case 2: //Silent
		{
			/*if ( Vars::AntiHack::AntiAim::InvalidShootPitch.var && Vars::AntiHack::AntiAim::Active.var && ( ( Vars::AntiHack::AntiAim::YawReal.var && Vars::AntiHack::AntiAim::YawFake.var ) ||
				 Vars::AntiHack::AntiAim::Pitch.var ) )
			{
				g_GlobalInfo.m_bFakeShotPitch = true;

				if ( vAngle.x > 0.f )
				{
					vAngle.x = Utils::RemapValClamped( vAngle.x, 0.0f, 89.0f, 180.0f, 91.0f );
				}
				else
				{
					vAngle.x = Utils::RemapValClamped( vAngle.x, 0.0f, -89.0f, -180.0f, -91.0f );
				}

				vAngle.y -= 180.f;
			}
			else*/
			{
				Utils::FixMovement( pCmd, vAngle );
				pCmd->viewangles = vAngle;
			}
			break;
		}

		default: break;
	}
}

bool CAimbotHitscan::ShouldFire(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd, const Target_t &Target)
{
	if (!Vars::Aimbot::Global::AutoShoot.var)
		return false;

	switch (g_GlobalInfo.m_nCurItemDefIndex)
	{
		case Sniper_m_TheMachina:
		case Sniper_m_ShootingStar:
		{
			if (!pLocal->InCond(TF_COND_ZOOMED))
				return false;

			break;
		}
		default: break;
	}

	switch (pLocal->m_iClass())
	{
		case SCOUT:
		{
			if ( Vars::Misc::CL_Move::Doubletap.var )
			{
				if ( g_GlobalInfo.m_nWaitForShift && !g_GlobalInfo.m_nShifted )
				{
					return false;
				}
			}

			break;
		}
		case SNIPER:
		{
			bool bIsScoped = pLocal->InCond(TF_COND_ZOOMED);

			if (Vars::Aimbot::Hitscan::WaitForHeadshot.var)
			{
				if (g_GlobalInfo.m_nCurItemDefIndex != Sniper_m_TheClassic
					&& g_GlobalInfo.m_nCurItemDefIndex != Sniper_m_TheSydneySleeper
					&& !g_GlobalInfo.m_bWeaponCanHeadShot && bIsScoped)
					return false;
			}

			if (Vars::Aimbot::Hitscan::WaitForCharge.var && bIsScoped)
			{
				int nHealth = Target.m_pEntity->As < CTFPlayer>( )->m_iHealth( );
				bool bIsCritBoosted = pLocal->IsCritBoostedNoMini();

				if (Target.m_nAimedHitbox == HITBOX_HEAD && g_GlobalInfo.m_nCurItemDefIndex != Sniper_m_TheSydneySleeper)
				{
					if (nHealth > 150)
					{
						float flDamage = Utils::RemapValClamped(pWeapon->As<CTFSniperRifle>( )->m_flChargedDamage( ), 0.0f, 150.0f, 0.0f, 450.0f );
						int nDamage = static_cast<int>(flDamage);

						if (nDamage < nHealth && nDamage != 450)
							return false;
					}

					else
					{
						if (!bIsCritBoosted && !g_GlobalInfo.m_bWeaponCanHeadShot)
							return false;
					}
				}

				else
				{
					if (nHealth > (bIsCritBoosted ? 150 : 50))
					{
						float flMult = Target.m_pEntity->As<CTFPlayer>()->InCond(TF_COND_URINE) ? 1.36f : 1.0f;

						if (bIsCritBoosted)
							flMult = 3.0f;

						float flMax = 150.0f * flMult;
						int nDamage = static_cast<int>(pWeapon->As<CTFSniperRifle>( )->m_flChargedDamage( ) * flMult);

						if (nDamage < Target.m_pEntity->As<CTFPlayer>( )->m_iHealth() && nDamage != static_cast<int>(flMax))
							return false;
					}
				}
			}

			break;
		}

		case SPY:
		{
			if (Vars::Aimbot::Hitscan::WaitForHeadshot.var && !g_GlobalInfo.m_bWeaponCanHeadShot)
			{
				if (g_GlobalInfo.m_nCurItemDefIndex == Spy_m_TheAmbassador || g_GlobalInfo.m_nCurItemDefIndex == Spy_m_FestiveAmbassador)
					return false;
			}

			break;
		}

		default: break;
	}

	int nAimMethod = (Vars::Aimbot::Hitscan::SpectatedSmooth.var && g_GlobalInfo.m_bLocalSpectated) ? 1 : Vars::Aimbot::Hitscan::AimMethod.var;

	if (nAimMethod == 1)
	{
		Vec3 vForward = { };
		Utils::AngleVectors(pCmd->viewangles, &vForward);
		Vec3 vTraceStart = pLocal->GetShootPos();
		Vec3 vTraceEnd = (vTraceStart + (vForward * 8192.0f));
		
		CGameTrace trace = { };
		CTraceFilterHitscan filter = { };
		filter.pSkip = pLocal;

		Utils::Trace(vTraceStart, vTraceEnd, (MASK_SHOT | CONTENTS_GRATE), &filter, &trace);

		if (trace.m_pEnt != Target.m_pEntity)
			return false;

		if (Target.m_nAimedHitbox == HITBOX_HEAD)
		{
			if (trace.hitbox != HITBOX_HEAD)
				return false;

			if (!Target.m_bHasMultiPointed)
			{
				Vec3 vMins = {}, vMaxs = {}, vCenter = {};
				matrix3x4_t Matrix = {};

				if (!Target.m_pEntity->As<CTFPlayer>( )->GetHitboxMinsAndMaxsAndMatrix(HITBOX_HEAD, vMins, vMaxs, Matrix, &vCenter))
					return false;

				vMins *= 0.5f;
				vMaxs *= 0.5f;

				if (!Utils::RayToOBB(vTraceStart, vForward, vCenter, vMins, vMaxs, Matrix))
					return false;
			}
		}
	}

	return true;
}

bool CAimbotHitscan::IsAttacking(CUserCmd *pCmd, CTFWeaponBase *pWeapon)
{
	return ((pCmd->buttons & IN_ATTACK) && g_GlobalInfo.m_bWeaponCanAttack);
}

void CAimbotHitscan::Run(CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd)
{
	if (!Vars::Aimbot::Hitscan::Active.var || !Utils::IsGameWindowInFocus( ) )
		return;

	Target_t Target = { };

	const int nWeaponID = pWeapon->GetWeaponID();
	const bool bShouldAim = (Vars::Aimbot::Global::AimKey.var == VK_LBUTTON ? (pCmd->buttons & IN_ATTACK) : g_AimbotGlobal.IsKeyDown());

	//Rev minigun if enabled and aimbot active
	if (bShouldAim) {
		if (Vars::Aimbot::Hitscan::AutoRev.var && nWeaponID == TF_WEAPON_MINIGUN)
			pCmd->buttons |= IN_ATTACK2;
	}

	if (GetTarget(pLocal, pWeapon, Target) && bShouldAim)
	{
		if (nWeaponID != TF_WEAPON_COMPOUND_BOW
			&& pLocal->m_iClass() == SNIPER
			&& pWeapon->GetSlot() == EWeaponSlots::SLOT_PRIMARY)
		{
			bool bScoped = pLocal->InCond(TF_COND_ZOOMED);

			if (Vars::Aimbot::Hitscan::AutoScope.var && !bScoped) {
				pCmd->buttons |= IN_ATTACK2;
				return;
			}

			if (Vars::Aimbot::Hitscan::ScopedOnly.var && !bScoped)
				return;
		}

		g_GlobalInfo.m_nCurrentTargetIdx = Target.m_pEntity->entindex();
		g_GlobalInfo.m_bHitscanRunning = true;
		g_GlobalInfo.m_bHitscanSilentActive = Vars::Aimbot::Hitscan::AimMethod.var == 2;

		if (Vars::Aimbot::Hitscan::SpectatedSmooth.var && g_GlobalInfo.m_bLocalSpectated)
			g_GlobalInfo.m_bHitscanSilentActive = false;

		if (g_GlobalInfo.m_bHitscanSilentActive)
			g_GlobalInfo.m_vAimPos = Target.m_vPos;

		if (ShouldFire(pLocal, pWeapon, pCmd, Target))
		{
			if (nWeaponID == TF_WEAPON_MINIGUN)
				pCmd->buttons |= IN_ATTACK2;

			pCmd->buttons |= IN_ATTACK;
			
			if ( Vars::Misc::CL_Move::Enabled.var && Vars::Misc::CL_Move::Doubletap.var && (pCmd->buttons & IN_ATTACK) && !g_GlobalInfo.m_nShifted && !g_GlobalInfo.m_nWaitForShift && pLocal->IsOnGround( ) )
			{
				g_GlobalInfo.m_bShouldShift = true;
			}

			if (g_GlobalInfo.m_bAAActive && !g_GlobalInfo.m_bWeaponCanAttack)
				pCmd->buttons &= ~IN_ATTACK;

			if (Vars::Aimbot::Hitscan::TapFire.var && nWeaponID == TF_WEAPON_MINIGUN && !g_GlobalInfo.m_bShouldShift )
			{
				bool bDo = Vars::Aimbot::Hitscan::TapFire.var == 1 ? pLocal->GetAbsOrigin().DistTo(Target.m_pEntity->GetAbsOrigin()) > 1000.0f : true;

				if (bDo && pWeapon->GetWeaponSpread())
				{
					float flTimeSinceLastShot = (pLocal->m_nTickBase() * TICK_INTERVAL) - pWeapon->m_flLastFireTime();

					if (pWeapon->GetWeaponData().m_nBulletsPerShot > 1)
					{
						if (flTimeSinceLastShot <= 0.25f)
							pCmd->buttons &= ~IN_ATTACK;
					}

					else if (flTimeSinceLastShot <= 1.25f)
						pCmd->buttons &= ~IN_ATTACK;
				}
			}
		}

		bool bIsAttacking = IsAttacking(pCmd, pWeapon);

		if (bIsAttacking)
			g_GlobalInfo.m_bAttacking = true;

		const float flLerpTime = TICKS_TO_TIME( TIME_TO_TICKS( g_GlobalInfo.lerpTime ) );
		const float simTime = Target.ShouldBacktrack ? Target.SimTime : ( Target.m_pEntity->m_flSimulationTime( ) + LerpTime( ) );
		if ((Vars::Misc::DisableInterpolation.var && Target.m_TargetType == ETargetType::PLAYER && bIsAttacking) || Target.ShouldBacktrack ) {
			/*pCmd->tick_count = TIME_TO_TICKS( Target.m_pEntity->m_flSimulationTime( ) + std::max(g_ConVars.cl_interp->GetFloat(), g_ConVars.cl_interp_ratio->GetFloat() / g_ConVars.cl_updaterate->GetFloat()));*/
			pCmd->tick_count = TIME_TO_TICKS( flLerpTime + simTime );
		}

		if ( Vars::Misc::StacBypass.var )
		{
			g_GlobalInfo.m_bNullCmd = true;
		}

		Aim( pCmd, Target.m_vAngleTo );

		if ( Vars::Misc::StacBypass.var )
		{
			g_GlobalInfo.m_bNullCmd = false;
		}
	}
}