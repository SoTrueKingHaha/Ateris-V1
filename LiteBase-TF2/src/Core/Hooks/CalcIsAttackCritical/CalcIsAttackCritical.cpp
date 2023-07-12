#include "CalcIsAttackCritical.h"

bool CritHandler( )
{
    if ( !I::Prediction->m_bFirstTimePredicted )
    {
        return false;
    }

    const auto& pLocal = g_EntityCache.GetLocal( );
    const auto& pWeapon = g_EntityCache.GetWeapon( );

    if ( !pWeapon || pLocal )
    {
        return false;
    }

    {
        static int s_nPreviousTickcount = 0;

        if ( s_nPreviousTickcount == I::GlobalVars->tickcount )
        {
            return false;
        }

        s_nPreviousTickcount = I::GlobalVars->tickcount;
    }

    {
        if ( pWeapon->GetWeaponID( ) == TF_WEAPON_MINIGUN ||
            pWeapon->GetWeaponID( ) == TF_WEAPON_FLAMETHROWER )
        {
            auto nPreviousAmmoCount = pLocal->GetAmmoCount( pWeapon->m_iPrimaryAmmoType( ) );
            static auto nNewAmmoCount = nPreviousAmmoCount;

            const auto bHasFiredBullet = nNewAmmoCount != nPreviousAmmoCount;

            if ( !bHasFiredBullet )
            {
                return false;
            }
        }
    }

    return true;
}

void __fastcall Hooks::CalcIsAttackCritical::Func( void *ecx, void *edx )
{
    const auto& pLocal = g_EntityCache.GetLocal( );
    const auto& pWeapon = g_EntityCache.GetWeapon( );

    if ( !pLocal || !pWeapon )
    {
        return Hook.CallOriginal<fn>( )( ecx, edx );
    }
    if ( CritHandler( ) )
    {
        return;
    }

    const auto nPreviousWeaponMode = pWeapon->m_iWeaponMode( );
    pWeapon->m_iWeaponMode( ) = 0;

    Hook.CallOriginal<fn>( )( ecx, edx );

    pWeapon->m_iWeaponMode( ) = nPreviousWeaponMode;
}

void Hooks::CalcIsAttackCritical::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"53 57 6A 00 68 ? ? ? ? 68 ? ? ? ? 6A 00 8B F9 E8 ? ? ? ? 50 E8 ? ? ? ? 8B D8 83 C4 14 85 DB 0F 84" ) );
	Hook.Create( pSrc, Func );
}