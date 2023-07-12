#include "Triggerbot.h"

int GetHitbox( CTFPlayer *pLocal, CTFWeaponBase *pWeapon )
{
	if ( Vars::Triggerbot::HeadOnly.var )
	{
		if ( pLocal->m_iClass( ) == SNIPER && pWeapon->CanHeadShot( pLocal ) )
		{
			if ( pWeapon->m_iItemDefinitionIndex( ) == Sniper_m_TheSydneySleeper )
			{
				return HITBOX_PELVIS;
			}

			return HITBOX_HEAD;
		}

		if ( pLocal->m_iClass( ) == SPY )
		{
			if ( pWeapon->m_iItemDefinitionIndex( ) == Spy_m_TheAmbassador || pWeapon->m_iItemDefinitionIndex( ) == Spy_m_FestiveAmbassador && pWeapon->AmbassadorCanHeadshot( ) )
			{
				return HITBOX_HEAD;
			}
			return HITBOX_PELVIS;
		}
		return HITBOX_HEAD;
	}
	return HITBOX_PELVIS;
}

bool CTriggerbot::IsAimingAtValidTarget( CTFPlayer *pLocal, CTFWeaponBase* pWeapon, CUserCmd *pCmd, float *pSimTime )
{
	Vec3 vForward = {};
	Utils::AngleVectors( pCmd->viewangles, &vForward );
	Vec3 vFrom = pLocal->GetShootPos( );
	Vec3 vTo = ( vFrom + ( vForward * 8192.0f ) );

	CGameTrace Trace = {};
	CTraceFilterHitscan Filter = {};
	Filter.pSkip = pLocal;
	Utils::Trace( vFrom, vTo, ( MASK_SHOT | CONTENTS_GRATE ), &Filter, &Trace );

	if ( const auto &pEntity = Trace.m_pEnt )
	{
		switch ( pEntity->GetClassId( ) )
		{
			case EClassIds::CTFPlayer:
			{
				if ( pEntity->As<CTFPlayer>()->deadflag( ) )
					return false;

				if ( pEntity->IsDormant( ) )
					return false;

				if ( !Vars::Triggerbot::TriggerPlayers.var )
					return false;

				if ( pEntity->m_iTeamNum( ) == pLocal->m_iTeamNum( ) )
					return false;

				if ( Trace.hitbox == HITBOX_HEAD && Vars::Triggerbot::HeadScale.var < 1.0f )
				{
					Vec3 vMins = {}, vMaxs = {}, vCenter = {};
					matrix3x4_t Matrix = {};

					if ( !pEntity->As<CTFPlayer>( )->GetHitboxMinsAndMaxsAndMatrix( HITBOX_HEAD, vMins, vMaxs, Matrix, &vCenter ) )
						return false;

					vMins *= Vars::Triggerbot::HeadScale.var;
					vMaxs *= Vars::Triggerbot::HeadScale.var;

					if ( !Utils::RayToOBB( vFrom, vForward, vCenter, vMins, vMaxs, Matrix ) )
						return false;
				}

				break;
			}

			case EClassIds::CObjectSentrygun:
			case EClassIds::CObjectDispenser:
			case EClassIds::CObjectTeleporter:
			{
				if ( !pEntity )
					return false;

				if ( !Vars::Triggerbot::TriggerBuildings.var )
					return false;

				if ( pEntity->m_iTeamNum( ) == pLocal->m_iTeamNum( ) )
					return false;

				break;
			}

			default: return false;
		}

		return true;
	}

	return false;
}

bool CTriggerbot::ShouldFire( CTFPlayer *pLocal, CTFWeaponBase *pWeapon )
{
	switch ( pWeapon->m_iItemDefinitionIndex( ) )
	{
		case Sniper_m_TheMachina:
		case Sniper_m_ShootingStar:
		{
			if ( !pLocal->InCond( TF_COND_ZOOMED ) )
				return false;

			break;
		}
		default: break;
	}

	switch ( pLocal->m_iClass( ) )
	{
		case SNIPER:
		{
			if ( Vars::Triggerbot::WaitForHeadshot.var )
			{
				if ( !pWeapon->CanHeadShot( pLocal ) && pLocal->InCond( TF_COND_ZOOMED ) )
					return false;
			}

			if ( !pLocal->InCond( TF_COND_ZOOMED ) && Vars::Triggerbot::ScopeOnly.var )
				return false;

			break;
		}
		case SPY:
		{
			if ( pWeapon->m_iItemDefinitionIndex( ) == Spy_m_TheAmbassador || pWeapon->m_iItemDefinitionIndex( ) == Spy_m_FestiveAmbassador )
			{
				if ( !pWeapon->AmbassadorCanHeadshot( ) && Vars::Triggerbot::WaitForHeadshot.var )
				{
					return false;
				}
			}

			break;
		}
		default: break;
	}

	return true;
}

void CTriggerbot::Run( CTFPlayer *pLocal, CTFWeaponBase *pWeapon, CUserCmd *pCmd )
{
	if ( !Vars::Triggerbot::Active.var )
		return;

	if ( !pLocal
		|| !pLocal->IsAlive( )
		|| pLocal->InCond( TF_COND_TAUNTING )
		|| pLocal->InCond( TF_COND_PHASE )
		|| pLocal->InCond( TF_COND_HALLOWEEN_GHOST_MODE )
		|| pLocal->InCond( TF_COND_HALLOWEEN_KART ) )
		return;

	if ( Utils::GetWeaponType( pWeapon ) != EWeaponType::HITSCAN )
		return;

	float fSimTime = 0.0f;

	if ( IsAimingAtValidTarget( pLocal, pWeapon, pCmd, &fSimTime ) && ShouldFire( pLocal, pWeapon ) && (GetAsyncKeyState(Vars::Triggerbot::Key.var) & 0x8000) )
	{
		pCmd->buttons |= IN_ATTACK;

		if ( pWeapon->CanShoot( pLocal ) )
			g_GlobalInfo.m_bAttacking = true;
	}
}