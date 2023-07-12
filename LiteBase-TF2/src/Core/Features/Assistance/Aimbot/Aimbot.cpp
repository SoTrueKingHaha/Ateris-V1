#include "Aimbot.h"
#include "../../Vars.h"

#include "AimbotHitscan/AimbotHitscan.h"
#include "AimbotProjectile/AimbotProjectile.h"
#include "AimbotMelee/AimbotMelee.h"
#include "../../../SDK/Include/EntityCache/EntityCache.h"

bool CAimbot::ShouldRun(CTFPlayer *pLocal, CTFWeaponBase *pWeapon)
{
	if (!Vars::Aimbot::Global::Active.var)
		return false;

	if (I::EngineVGui->IsGameUIVisible() || I::Surface->IsCursorVisible())
		return false;

	if (pLocal->deadflag()
		|| pLocal->InCond(TF_COND_TAUNTING)
		|| pLocal->InCond(TF_COND_PHASE )
		|| pLocal->m_bFeignDeathReady()
		|| pLocal->InCond(TF_COND_STEALTHED )
		|| pLocal->InCond(TF_COND_HALLOWEEN_KART )
		|| pLocal->InCond( TF_COND_HALLOWEEN_GHOST_MODE))
		return false;

	switch (g_GlobalInfo.m_nCurItemDefIndex) {
		case Soldier_m_RocketJumper:
		case Demoman_s_StickyJumper: return false;
		default: break;
	}

	switch (pWeapon->GetWeaponID())
	{
		case TF_WEAPON_PDA:
		case TF_WEAPON_PDA_ENGINEER_BUILD:
		case TF_WEAPON_PDA_ENGINEER_DESTROY:
		case TF_WEAPON_PDA_SPY:
		case TF_WEAPON_PDA_SPY_BUILD:
		case TF_WEAPON_BUILDER:
		case TF_WEAPON_INVIS:
		case TF_WEAPON_LUNCHBOX:
		case TF_WEAPON_BUFF_ITEM:
		case TF_WEAPON_GRAPPLINGHOOK:
		{
			return false;
		}

		default: break;
	}

	return true;
}

void CAimbot::Run(CUserCmd* pCmd)
{
	g_GlobalInfo.m_nCurrentTargetIdx = 0;
	g_GlobalInfo.m_flCurAimFOV = 0.0f;
	//g_GlobalInfo.PredictedPos = Vec3();
	g_GlobalInfo.m_bHitscanRunning = false;
	g_GlobalInfo.m_bHitscanSilentActive = false;
	g_GlobalInfo.m_bProjectileSilentActive = false;
	g_GlobalInfo.m_vAimPos = Vec3();

	const auto &pLocal = g_EntityCache.GetLocal( );
	
	const auto& pWeapon = g_EntityCache.GetWeapon( );

	if (pLocal && pWeapon)
	{
		if (!ShouldRun(pLocal, pWeapon))
			return;

		switch (g_GlobalInfo.m_WeaponType)
		{
			case EWeaponType::HITSCAN: {
				g_AimbotHitscan.Run(pLocal, pWeapon, pCmd);
				break;
			}

			case EWeaponType::PROJECTILE: {
				g_AimbotProjectile.Run(pLocal, pWeapon, pCmd);
				break;
			}

			case EWeaponType::MELEE: {
				//g_AimbotMelee.Run(pLocal, pWeapon, pCmd);
				break;
			}

			default: break;
		}
	}
}