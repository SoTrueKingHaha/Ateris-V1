#include "FireBullets.h"
#include "../../Features/Events/BadActors/BadActors.h"

bool FireBulletsHandler( CBaseCombatWeapon *weapon )
{
	static int nLastTickCount = 0;
	static int nLastBulletAmount = 0;
	static int nShotCount = 0;
	const auto& pLocal = g_EntityCache.GetLocal( );
	if ( !pLocal || pLocal->deadflag( ) ) 
	{ 
		nShotCount = 0;
		return false; 
	}
	const auto& pWeapon = g_EntityCache.GetWeapon( );
	if ( !weapon || weapon != pWeapon || !I::Prediction->m_bFirstTimePredicted )
	{
		nShotCount = 0;
		return false;
	}

	const int nBulletAmount = weapon->GetBulletAmount( );

	if ( nBulletAmount != nLastBulletAmount )
	{
		nShotCount = 0;
		nLastBulletAmount = nBulletAmount;
		return true;
	}

	if ( nLastTickCount == I::GlobalVars->tickcount )
	{
		nShotCount++;
		if ( nShotCount > nBulletAmount )
		{
			return false;
		}
	}
	nShotCount = 0;
	nLastTickCount = I::GlobalVars->tickcount;

	return true;
}

void __cdecl Hooks::FX_FireBullets::Func( void *pWpn, int iPlayer, const Vec3 *vecOrigin, const Vec3 *vecAngles, int iWeapon, int iMode, int iSeed, float flSpread, float flDamage, bool bCritical )
{
	if ( iPlayer != I::EngineClient->GetLocalPlayer( ) ) {
		//Utils::ConLog("FX_FireBullets", tfm::format("%d {%.1f, %.1f, %.1f}", iPlayer, vecAngles.x, vecAngles.y, vecAngles.z).c_str(), {0, 222, 255, 255});
		g_Backtrack.ReportShot( iPlayer );
		g_BadActors.ReportShot( iPlayer );
	}

	static auto C_TFWeaponBaseGun__FireBullet_Call = g_Pattern.Find( L"client.dll", L"83 C4 ? 5F 5E 5B 8B E5 5D C2 ? ? CC CC CC CC 53" );
	if ( reinterpret_cast< DWORD >( _ReturnAddress( ) ) != C_TFWeaponBaseGun__FireBullet_Call )
	{
		return;
	}

	return Hook.CallOriginal<fn>( )( pWpn, iPlayer, vecOrigin, vecAngles, iWeapon, iMode, iSeed, flSpread, flDamage, bCritical );
}

void Hooks::FX_FireBullets::Init( )
{
	//void *pSrc = reinterpret_cast< void * >( g_Pattern.E8( L"client.dll", L"E8 ? ? ? ? 83 C4 28 C2 04 00" ) );
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 81 EC ? ? ? ? 53 8B 5D ? 56 53" ) );
	Hook.Create( pSrc, Func );
}

void __fastcall Hooks::FireBullets::Func( void *ecx, void *edx, CBaseCombatWeapon *pWeapon, const FireBulletsInfo_t &info, bool bDoEffects, int nDamageType, int nCustomDamageType )
{
	if ( !pWeapon || !Vars::Visuals::ParticleTracer.var )
	{
		return Hook.CallOriginal<fn>( )( ecx, edx, pWeapon, info, bDoEffects, nDamageType, nCustomDamageType );
	}
	if ( const auto& pLocal = g_EntityCache.GetLocal( ) )
	{
		const Vec3 vStart = info.m_vecSrc;
		const Vec3 vEnd = vStart + info.m_vecDirShooting * info.m_flDistance;

		CGameTrace trace = {};
		CTraceFilterHitscan filter = {};
		filter.pSkip = pLocal;

		/* if ur shooting thru stuff, change MASK_SHOT to MASK_SOLID - myzarfin */
		Utils::Trace( vStart, vEnd, ( MASK_SHOT /* | CONTENTS_GRATE | MASK_VISIBLE*/ ), &filter, &trace );

		const int iAttachment = pWeapon->LookupAttachment( "muzzle" );
		pWeapon->GetAttachment( iAttachment, trace.startpos );

		if ( !pLocal->IsInValidTeam( ) )
		{
			return;
		}

		const int team = pLocal->m_iTeamNum( );

		switch ( Vars::Visuals::ParticleTracer.var )
		{
			//Machina
			case 1:
			{
				Particles::ParticleTracer( team == 2 ? "dxhr_sniper_rail_red" : "dxhr_sniper_rail_blue", trace.startpos, trace.endpos, pLocal->entindex( ), iAttachment, true );
				break;
			}

				//C.A.P.P.E.R
			case 2:
			{
				pLocal->IsCritBoosted( )
					? ( Particles::ParticleTracer( team == 2 ? "bullet_tracer_raygun_red_crit" : "bullet_tracer_raygun_blue_crit",
						trace.startpos, trace.endpos, pLocal->entindex( ), iAttachment, true ) )
					: ( Particles::ParticleTracer( team == 2 ? "bullet_tracer_raygun_red" : "bullet_tracer_raygun_blue", trace.startpos,
						trace.endpos, pLocal->entindex( ), iAttachment, true ) );
				break;
			}

				//Short circuit
			case 3:
			{
				Particles::ParticleTracer( team == 2 ? "dxhr_lightningball_hit_zap_red" : "dxhr_lightningball_hit_zap_blue", trace.startpos, trace.endpos, pLocal->entindex( ), iAttachment, true );
				break;
			}

				//Merasmus ZAP
			case 4:
			{
				Particles::ParticleTracer( "merasmus_zap", trace.startpos, trace.endpos, pLocal->entindex( ), iAttachment, true );
				break;
			}

				//Merasmus ZAP Beam 2
			case 5:
			{
				Particles::ParticleTracer( "merasmus_zap_beam02", trace.startpos, trace.endpos, pLocal->entindex( ), iAttachment, true );
				break;
			}

			case 6:
			{
				pLocal->IsCritBoosted( )
					? ( Particles::ParticleTracer(
					team == 2 ? "bullet_bignasty_tracer01_red_crit" : "bullet_bignasty_tracer01_blue_crit",
					trace.startpos, trace.endpos, pLocal->entindex( ), iAttachment, true ) )
					: ( Particles::ParticleTracer( team == 2 ? "bullet_bignasty_tracer01_blue" : "bullet_bignasty_tracer01_red",
						trace.startpos, trace.endpos, pLocal->entindex( ), iAttachment, true ) );
				break;
			}

			default: break;
		}
	}
}

void Hooks::FireBullets::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 81 EC ? ? ? ? 53 56 57 8B F9 8B 5D" ) );
	Hook.Create( pSrc, Func );
}