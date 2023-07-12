#include "PlayerList.h"
#include "../../../../../../Utils/Timer/Timer.h"
#include "../../../../Colors.h"

void CPlayerList::Fill( )
{
	if ( !I::EngineClient->IsInGame( ) || !I::EngineClient->IsConnected( ) ) { return; }
	const auto &pLocal = g_EntityCache.GetLocal( );
	if ( pLocal && !pLocal->IsDormant( ) )
	{
		static Timer playerlist{ };
		if ( playerlist.Run( 1000 ) )
		{
			std::multimap<int, Player_t> listBuffer{};
			for ( int i = 1; i <= I::EngineClient->GetMaxClients( ); i++ )
			{
				const auto &PR = g_EntityCache.GetPR( );
				if ( PR && PR->GetValid( i ) && PR->GetConnected( i ) )
				{
					player_info_t pi;
					if ( I::EngineClient->GetPlayerInfo( i, &pi ) )
					{
						Player_t player
						{
							//Colors::GetEntityColor( GET_ENT_I( i )->As<CBaseEntity>( ), pLocal ),
							PR->GetPlayerName( i ),
							PR->IsAlive( i ),
							//GET_ENT_I( i )->As<CTFPlayer>( )->PlayerOnFriendsList( ),
							PR->GetTeam( i ),
							i,
							PR->GetUserID( i ),
							pi.friendsID,
							PR->GetScore( i ),
							PR->GetKills( i ),
							PR->GetDeaths( i ),
							PR->GetPing( i )
						};

						listBuffer.emplace( PR->GetTeam( i ), player );
					}
				}
			}

			std::lock_guard lock( mutex );
			listBuffer.swap( PlayerCache );
		}
	}
}