#include "InCond.h"
#include "../../Features/Visuals/Outlines/Outlines.h"

bool __fastcall Hooks::InCond::Func( void *ecx, void *edx, ETFCond nCond )
{
	static const auto dwPlayerShouldDraw = g_Pattern.Find( L"client.dll", L"E8 ? ? ? ? 84 C0 75 C5" ) + 0x5;
	static const auto dwWearableShouldDraw = g_Pattern.Find( L"client.dll", L"E8 ? ? ? ? 84 C0 75 E1 6A 03" ) + 0x5;
	static const auto dwHudScopeShouldDraw = g_Pattern.Find( L"client.dll", L"84 C0 74 ? 8B CE E8 ? ? ? ? 85 C0 74 ? 8B CE E8 ? ? ? ? 8B C8 8B 10 FF 92 ? ? ? ? 83 F8 ? 0F 94" );

	const auto dwRetAddr = reinterpret_cast< DWORD >( _ReturnAddress( ) );

	if ( nCond == TF_COND_ZOOMED )
	{
		if ( dwRetAddr == dwPlayerShouldDraw ||
			 dwRetAddr == dwWearableShouldDraw ||
			 ( Vars::Visuals::RemoveScope.var && dwRetAddr == dwHudScopeShouldDraw ) )
		{
			return false;
		}
	}

	auto GetOuter = [ &ecx ] ( ) -> CBaseEntity *
	{
		static const auto dwShared = g_NetVars.get_offset( "DT_TFPlayer", "m_Shared" );
		static const auto dwBombHeadStage = g_NetVars.get_offset(
		 "DT_TFPlayer", "m_Shared", "m_nHalloweenBombHeadStage" );
		static const auto dwOff = ( dwBombHeadStage - dwShared ) + 0x4;
		return *reinterpret_cast< CBaseEntity ** >( reinterpret_cast< DWORD >( ecx ) + dwOff );
	};

	if ( Vars::Misc::DisableTaunts.var )
	{
		if ( nCond == TF_COND_TAUNTING )
		{
			const auto &pLocal = g_EntityCache.GetLocal( );
			if ( pLocal && !pLocal->IsDormant( ) && !pLocal->deadflag( ) )
			{
				if ( const auto &pEntity = GetOuter( ) )
				{
					if ( pEntity->m_iTeamNum( ) != pLocal->m_iTeamNum( ) )
					{
						return false;
					}
				}

				/*if ( pLocal == GetOuter( ) )
				{
					I::Input->SetGameActionSetFlags( GAME_ACTION_SET_FLAGS_NONE );
					return false;
				}*/
			}
		}
	}

	//Just compare entity ptr's, filtering out local is enough. Also prevents T pose.
	if ( nCond == TF_COND_DISGUISED && LOCAL_PLAYER != GetOuter( ) )
	{
		return false;
	}

	return Hook.CallOriginal<fn>( )( ecx, edx, nCond );
}

void Hooks::InCond::Init( )
{
	void *pSrc = reinterpret_cast< void * >( g_Pattern.Find( L"client.dll", L"55 8B EC 83 EC 08 56 57 8B 7D 08 8B F1 83 FF 20" ) );
	Hook.Create( pSrc, Func );
}