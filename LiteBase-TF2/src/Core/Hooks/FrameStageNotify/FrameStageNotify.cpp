#include "FrameStageNotify.h"
#include "../../Features/Misc/Misc.h"
#include "../../Features/Visuals/AttributeChanger/AttributeChanger.h"
#include "../../Features/Visuals/Outlines/Outlines.h"
#include "../../Features/Assistance/Aimbot/MoveSim/MoveSim.h"
#include "../../Features/Exploits/Backtrack/Backtrack.h"
#include "../../Features/Visuals/Render/GUI/Menu/PlayerList/PlayerList.h"

void __fastcall Hooks::FrameStageNotify::Func( void *ecx, void *edx, ClientFrameStage_t curStage )
{
	switch ( curStage )
	{
		case ClientFrameStage_t::FRAME_RENDER_START:
		{
			g_GlobalInfo.m_vPunchAngles = Vec3( );

			if ( const auto& pLocal = g_EntityCache.GetLocal( ) )
			{
				g_GlobalInfo.m_vPunchAngles = pLocal->GetPunchAngles( );
				pLocal->ClearPunchAngle( );
			}
			break;
		}
		default: break;
	}

	Hook.CallOriginal<fn>( )( ecx, edx, curStage );

	switch ( curStage )
	{
		case ClientFrameStage_t::FRAME_NET_UPDATE_START:
		{
			g_EntityCache.Clear();
			break;
		}

		case ClientFrameStage_t::FRAME_NET_UPDATE_POSTDATAUPDATE_START:
		{
			//g_AttributeChanger.Run( );
			break;
		}

		case ClientFrameStage_t::FRAME_NET_UPDATE_END:
		{
			g_EntityCache.Fill();
			//g_PlayerList.Fill( );
			g_Backtrack.FrameStageNotify( );
			g_MoveSim.FillVelocities( );
			/*for ( int i = 0; i < I::EngineClient->GetMaxClients( ); i++ )
			{
				if ( const auto &player = I::ClientEntityList->GetClientEntity( i )->As<CTFPlayer>( ) )
				{
					const VelFixRecord record = { player->m_vecOrigin( ), player->m_fFlags( ), player->m_flSimulationTime( ) };
					g_GlobalInfo.velFixRecord[ player ] = record;
				}
			}
			*/

			/*outgoing_command = I::ClientState->lastoutgoingcommand + I::ClientState->chokedcommands;
			const auto &pLocal = g_EntityCache.GetLocal( );
			for ( int i{ }; ; i++ )
			{
				current_command = I::ClientState->last_command_ack + i;

				if ( current_command > outgoing_command || i >= 150 )
					break;

				cmd = I::Input->GetUserCmd( current_command );
				if ( !cmd )
					break;

				if ( !cmd->hasbeenpredicted )
					++pLocal->m_nTickBase( );
			}*/
			break;
		}

		case ClientFrameStage_t::FRAME_RENDER_START: {
			if ( I::EngineClient->IsConnected( ) || I::EngineClient->IsInGame( ) )
				g_Misc.UpdateWorldModulation( );

			if ( strstr( I::EngineClient->GetLevelName( ), "tr_walkway" ) )
			{
				const auto &pLocal = g_EntityCache.GetLocal( );

				for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::PLAYERS_ALL ) )
				{
					const auto &pPlayer = pEntity->As<CTFPlayer>( );

					if ( pPlayer->m_iTeamNum( ) != 3 || pPlayer == pLocal )
						continue;

					const auto &pAnimState = pPlayer->GetAnimState( );

					if ( pAnimState )
					{
						if ( pAnimState->m_PoseParameterData.m_iMoveX )
						{
							pAnimState->m_PoseParameterData.m_iMoveX = 0;
						}
						if ( pAnimState->m_PoseParameterData.m_iMoveY )
						{
							pAnimState->m_PoseParameterData.m_iMoveY = 0;
						}
					}
				}
			}
			break;
		}

		default: break;
	}
}

void Hooks::FrameStageNotify::Init()
{
	fn FN = reinterpret_cast<fn>(Utils::GetVFuncPtr(I::BaseClientDLL, 35));
	Hook.Create(FN, Func);
}