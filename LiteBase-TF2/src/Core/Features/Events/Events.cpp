#include "Events.h"
#include "../Exploits/AntiAim/AntiAim.h"
#include "../../Utils/Timer/Timer.h"
#include "../Misc/Misc.h"
#include "BadActors/BadActors.h"

//static std::string yellow( { '\x7', 'C', '8', 'A', '9', '0', '0' } ); //C8A900

void CEvents::Setup( const std::deque<const char *> &events )
{
	if ( events.empty( ) )
		return;

	for ( auto pEvent : events )
	{
		I::GameEvent->AddListener( this, pEvent, false );

		if ( !I::GameEvent->FindListener( this, pEvent ) )
		{
			std::string what = ("Listener failed: %s", pEvent);
			throw std::runtime_error( what );
		}
	}
}

void CEvents::Destroy( )
{
	I::GameEvent->RemoveListener( this );
}

void CEvents::GameEvents( IGameEvent *pEvent, const FNV1A_t uHashName )
{
	if ( !I::EngineClient->IsConnected( ) || !I::EngineClient->IsInGame( ) )
		return;

	if ( const auto& pLocal = g_EntityCache.GetLocal( ) )
	{
		if ( uHashName == FNV1A::HashConst( "player_connect" ) )
		{
			if ( const auto &pEntity = GET_ENT_I( I::EngineClient->GetPlayerForUserID( pEvent->GetInt( "userid" ) ) ) )
			{
				if ( !pEntity ) { return; }

				int nIndex = pEntity->entindex( );
				player_info_t pi = {};
				I::EngineClient->GetPlayerInfo( nIndex, &pi );
				const auto& pPlayer = pEntity->As<CTFPlayer>( );
				std::string szTeam = pPlayer->m_iTeamNum( ) != pLocal->m_iTeamNum( ) ? "ENEMY" : "TEAM";
				std::string szName = yellow + pi.m_sName + white;
				std::string szGuid = yellow + pi.guid + white;

				std::string connect = white + "[" + blue + "ateris" + white + "]  [" + szTeam + "] " + szName + " has joined. [" + szGuid + "]";
				I::ClientModeShared->m_pChatElement->ChatPrintf( 0, connect.c_str( ) );
			}
		}

		if ( uHashName == FNV1A::HashConst( "player_disconnect" ) )
		{
			if ( const auto &pEntity = GET_ENT_I( I::EngineClient->GetPlayerForUserID( pEvent->GetInt( "userid" ) ) ) )
			{
				int nIndex = pEntity->entindex( );
				player_info_t pi = {};
				I::EngineClient->GetPlayerInfo( nIndex, &pi );
				const auto& pPlayer = pEntity->As<CTFPlayer>( );
				std::string szName = yellow + pEvent->GetString( "name" ) + white;

				std::string disconnect = white + "[" + blue + "ateris" + white + "] " + szName + " has left.";
				I::ClientModeShared->m_pChatElement->ChatPrintf( 0, disconnect.c_str( ) );
			}
		}

		if ( uHashName == FNV1A::HashConst( "player_changeclass" ) )
		{
			if ( const auto &pEntity = GET_ENT_I( I::EngineClient->GetPlayerForUserID( pEvent->GetInt( "userid" ) ) ) )
			{
				if ( !pEntity || pEntity == pLocal ) { return; }

				int nIndex = pEntity->entindex( );

				player_info_t pi = {};

				I::EngineClient->GetPlayerInfo( nIndex, &pi );
				const auto& pPlayer = pEntity->As<CTFPlayer>( );
				std::string szTeam = pPlayer->m_iTeamNum( ) != pLocal->m_iTeamNum( ) ? "ENEMY" : "TEAM";
				std::string szName = yellow + pi.m_sName + white;
				std::string szClass = Utils::GetClassByIndex( pEvent->GetInt( "class" ) );

				std::string classchange = white + "[" + blue + "ateris" + white + "]  [" + szTeam + "] " + szName + " changed class to " + szClass + ".";
				I::ClientModeShared->m_pChatElement->ChatPrintf( 0, classchange.c_str( ) );
			}
		}

		if ( uHashName == FNV1A::HashConst( "player_changename" ) )
		{
			if ( const auto &pEntity = GET_ENT_I( I::EngineClient->GetPlayerForUserID( pEvent->GetInt( "userid" ) ) ) )
			{
				if ( !pEntity ) { return; }

				std::string szNewName = pEvent->GetString( "newname" );
				std::string szOldName = pEvent->GetString( "oldname" );

				const auto &pPlayer = pEntity->As<CTFPlayer>( );
				std::string szTeam = pPlayer->m_iTeamNum( ) != pLocal->m_iTeamNum( ) ? "ENEMY" : "TEAM";

				std::string namechange = white + "[" + blue + "ateris" + white + "]  [" + szTeam + "] " + yellow + szOldName + white + " changed name to " + yellow + szNewName + white + ".";
				I::ClientModeShared->m_pChatElement->ChatPrintf( 0, namechange.c_str( ) );
			}
		}

		if ( uHashName == FNV1A::HashConst( "vote_cast" ) )
		{
			const auto &pEntity = GET_ENT_I( pEvent->GetInt( "entityid" ) );
			if ( pEntity && pEntity->GetClassId( ) == EClassIds::CTFPlayer )
			{
				if ( !pEntity ) { return; }

				const bool bVotedYes = pEvent->GetInt( "vote_option" ) == 0;
				player_info_t pi = {};

				I::EngineClient->GetPlayerInfo( pEntity->entindex( ), &pi );
				const auto& pPlayer = pEntity->As<CTFPlayer>( );
				std::string szTeam = pPlayer->m_iTeamNum( ) != pLocal->m_iTeamNum( ) ? "ENEMY" : "TEAM";
				std::string szName = yellow + pi.m_sName + white;
				std::string szVote = bVotedYes ? red + "Yes" + white : green + "No" + white;

				std::string vote = white + "[" + blue + "ateris" + white + "]  [" + szTeam + "] " + szName + " voted " + szVote + ".";
				I::ClientModeShared->m_pChatElement->ChatPrintf( 0, vote.c_str( ) );
			}
		}

		if ( uHashName == FNV1A::HashConst( "achievement_earned" ) )
		{
			float time = 5.0f;
			const int player = pEvent->GetInt( "player", 0xDEAD );
			const int achievement = pEvent->GetInt( "achievement", 0xDEAD );

			player_info_t pi = {};

			//0xCA7 = identify + mark
			//0xCA8 = mark

			if ( I::EngineClient->GetPlayerInfo( player, &pi ) )
			{
				bool bIsIdentify = achievement == 0xCA7;
				bool bIsMark = achievement == 0xCA8;
				if ( ( bIsIdentify || bIsMark ) && pLocal->entindex( ) != player )
				{
					const auto& pEntity = GET_ENT_I( player );
					if ( !pEntity ) { return; }

					const auto& pPlayer = pEntity->As<CTFPlayer>( );
					std::string szTeam = pPlayer->m_iTeamNum( ) != pLocal->m_iTeamNum( ) ? "ENEMY" : "TEAM";
					std::string szMark = bIsMark ? "0xCA8" : "0xCA7";
					std::string szName = yellow + pi.m_sName + white;

					std::string catidentify = white + "[" + blue + "ateris" + white + "]  [" + szTeam + "] " + szName + " is marked as a catbot [" + szMark + "]";
					I::ClientModeShared->m_pChatElement->ChatPrintf( 0, catidentify.c_str( ) );
				}
			}
		}
	}
}

void CEvents::OnHurt( IGameEvent *pEvent, const FNV1A_t uNameHash )
{
	if ( uNameHash == FNV1A::HashConst( "player_hurt" ) )
	{
		g_BadActors.ReportDamage( pEvent );
		//I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "player hurt\n" );
		if ( const auto& pEntity = I::ClientEntityList->GetClientEntity(
			I::EngineClient->GetPlayerForUserID( pEvent->GetInt( "userid" ) ) ) )
		{
			//I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "player found\n" );
			const auto nAttacker = I::EngineClient->GetPlayerForUserID( pEvent->GetInt( "attacker" ) );
			const auto &pLocal = g_EntityCache.GetLocal( );
			if ( !pLocal || pLocal->deadflag( ) || pLocal->IsDormant( ) ) { return; }
			const auto &pWeapon = g_EntityCache.GetWeapon( );
			if ( !pWeapon || pWeapon->IsDormant( ) ) { return; }

			if ( GET_ENT_I( nAttacker ) == pLocal )
			{
				//I::CVar->ConsoleColorPrintf( { 0, 255, 255, 255 }, "local is attacker\n" );
				if ( Vars::Aimbot::Projectile::TargetBBox.var )
				{
					if ( g_GlobalInfo.m_WeaponType == EWeaponType::PROJECTILE && pWeapon->GetWeaponID( ) != TF_WEAPON_FLAMETHROWER )
					{
						if ( !g_GlobalInfo.m_vPredictedPos.IsZero( ) )
						{
							I::DebugOverlay->AddBoxOverlay( pEntity->As<CBaseEntity>( )->GetAbsOrigin( ), pEntity->As<CBaseEntity>( )->m_vecMins( ), 
															pEntity->As<CBaseEntity>( )->m_vecMaxs( ), { 0, 0, 0 }, 
															Vars::Colors::TargetBBox.r, Vars::Colors::TargetBBox.g, Vars::Colors::TargetBBox.b, Vars::Colors::TargetBBox.a,
															Vars::Aimbot::Projectile::BBoxDuration.var );

							static Timer EraseTimer{};
							if ( EraseTimer.Run( Vars::Aimbot::Projectile::BBoxDuration.var * 1000.f ) ) {
								g_GlobalInfo.m_vPredictedPos.Clear( );
							}
						}
					}
				}

				if ( Vars::Aimbot::Hitscan::ShowHitboxes.var )
				{
					if ( g_GlobalInfo.m_WeaponType == EWeaponType::HITSCAN )
					{
						if ( pEvent->GetInt( "userid" ) != I::EngineClient->GetLocalPlayer( ) )
							g_Misc.DrawHitboxes( pEntity->As<CTFPlayer>( ), Vars::Colors::ShowHitboxes,
												{ Vars::Colors::ShowHitboxes.r, Vars::Colors::ShowHitboxes.g, Vars::Colors::ShowHitboxes.b, 255 },
												 Vars::Aimbot::Hitscan::HitboxDuration.var );
					}
				}
			}
		}
	}
	g_GlobalInfo.m_vPredictedPos = Vec3( 0.0f, 0.0f, 0.0f );
}

void CEvents::FireGameEvent( IGameEvent *pEvent )
{
	if ( !pEvent )
		return;

	const FNV1A_t uNameHash = FNV1A::Hash( pEvent->GetName( ) );

	GameEvents( pEvent, uNameHash );
	g_AntiAim.Event( pEvent, uNameHash );
	OnHurt( pEvent, uNameHash );
}

void CEvents::PrintOnAttach( )
{
	std::string inject = white + "[" + blue + "ateris" + white + "] Successfully" + green + " attached" + white + ".";
	I::ClientModeShared->m_pChatElement->ChatPrintf( 0, inject.c_str( ) );
}

void CEvents::PrintOnDetach( )
{
	std::string uninject = white + "[" + blue + "ateris" + white + "] Successfully" + red + " detached" + white + ".";
	I::ClientModeShared->m_pChatElement->ChatPrintf( 0, uninject.c_str( ) );
}