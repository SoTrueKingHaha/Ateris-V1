#include "SetAbsVelocity.h"

void __fastcall Hooks::SetAbsVelocity::Func( void *ecx, void *edx, const Vec3 &vecAbsVelocity )
{
	static DWORD dwC_BasePlayer_PostDataUpdate_SetAbsVelocityCall = g_Pattern.Find( L"client.dll", L"E8 ? ? ? ? 53 8B CF E8 ? ? ? ? 8D 47 F8 39 05" ) + 0x5;

	if ( reinterpret_cast< DWORD >( _ReturnAddress( ) ) == dwC_BasePlayer_PostDataUpdate_SetAbsVelocityCall )
	{
		if ( auto pPlayer = static_cast< CTFPlayer * >( ecx ) )
		{
			if ( g_GlobalInfo.velFixRecord.find( pPlayer ) != g_GlobalInfo.velFixRecord.end( ) )
			{
				const auto &Record = g_GlobalInfo.velFixRecord[ pPlayer ];

				float flSimTimeDelta = pPlayer->m_flSimulationTime( ) - Record.m_flSimulationTime;

				if ( flSimTimeDelta > 0.0f )
				{
					Vec3 vOldOrigin = Record.m_vecOrigin;

					int nCurFlags = pPlayer->m_fFlags( );
					int nOldFlags = Record.m_nFlags;

					if ( !( nCurFlags & FL_ONGROUND ) && !( nOldFlags & FL_ONGROUND ) )
					{
						bool bCorrected = false;

						if ( ( nCurFlags & FL_DUCKING ) && !( nOldFlags & FL_DUCKING ) )
						{
							vOldOrigin.z += 20.0f;
							bCorrected = true;
						}

						if ( !( nCurFlags & FL_DUCKING ) && ( nOldFlags & FL_DUCKING ) )
						{
							vOldOrigin.z -= 20.0f;
							bCorrected = true;
						}

						if ( bCorrected )
						{
							Vec3 vNewVelocity = vecAbsVelocity;
							vNewVelocity.z = ( pPlayer->m_vecOrigin( ).z - vOldOrigin.z ) / flSimTimeDelta;
							Hook.CallOriginal<fn>( )( ecx, edx, vNewVelocity );
							return;
						}
					}
				}
			}
		}
	}

	Hook.CallOriginal<fn>( )( ecx, edx, vecAbsVelocity );
}

void Hooks::SetAbsVelocity::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 83 EC ? 56 57 8B 7D ? 8B F1 F3 0F" ) );
	Hook.Create( pSrc, Func );
}