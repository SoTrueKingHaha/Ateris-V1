#include "FrameAdvance.h"
std::unordered_map<void *, std::pair<int, float>> pAnimatingInfo;

float __fastcall Hooks::FrameAdvance::Func( void *ecx, void *edx, float flInterval )
{
	const auto& pLocal = g_EntityCache.GetLocal( );
	if ( !pLocal || pLocal->IsDormant( ) || pLocal->deadflag( ) ) { return Hook.CallOriginal<fn>( )( ecx, edx, flInterval ); }

	if ( const auto pEntity = static_cast< CBaseEntity * >( ecx ) ) {
		if ( pEntity->GetClassId( ) == EClassIds::CTFPlayer ) {
			if ( pEntity->As<CTFPlayer>( )->m_flSimulationTime( ) == pEntity->As<CTFPlayer>( )->GetOldSimulationTime( ) || I::GlobalVars->tickcount == pAnimatingInfo[ ecx ].first ) {
				pAnimatingInfo[ ecx ].second += flInterval;
				return 0.0f;
			}
		}
	}

	flInterval = pAnimatingInfo[ ecx ].second; pAnimatingInfo[ ecx ].second = 0.0f; pAnimatingInfo[ ecx ].first = I::GlobalVars->tickcount;
	return Hook.CallOriginal<fn>( )( ecx, edx, flInterval );
}

void Hooks::FrameAdvance::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 83 EC 14 56 8B F1 57 80 BE ? ? ? ? ? 0F 85 ? ? ? ? 83 BE ? ? ? ? ? 75 05 E8 ? ? ? ? 8B BE ? ? ? ? 85 FF 0F 84 ? ? ? ? F3 0F 10 45 ? 0F 57 D2 A1 ? ? ? ? 0F 2E C2 F3 0F 10 48 ? F3 0F 11 4D" ) );
	Hook.Create( pSrc, Func );
}