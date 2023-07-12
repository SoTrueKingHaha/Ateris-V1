#pragma once
#include "../../SDK/SDK.h"
#include "../../Features/Exploits/Backtrack/Backtrack.h"
#include "intrin.h"

using LookupAttachment_t = int( * )( CBaseEntity *, const char * );

namespace Hooks
{
	namespace FireBullets
	{
		inline CHook Hook;
		using fn = void( __fastcall * )( void *, void *, CBaseCombatWeapon *, const FireBulletsInfo_t &, bool, int, int );
		void __fastcall Func( void *ecx, void *edx, CBaseCombatWeapon *pWeapon, const FireBulletsInfo_t &info, bool bDoEffects, int nDamageType, int nCustomDamageType );
		void Init( );
	}

	namespace FX_FireBullets
	{
		inline CHook Hook;
		using fn = void( __cdecl * )( void *, int, const Vec3 *, const Vec3 *, int, int, int, float, float, bool );
		void __cdecl Func( void *pWpn, int iPlayer, const Vec3 *vecOrigin, const Vec3 *vecAngles, int iWeapon, int iMode, int iSeed, float flSpread, float flDamage, bool bCritical );
		void Init( );
	}
}