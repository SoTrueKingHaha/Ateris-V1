#include "ESP.h"
#include "../../../Exploits/AntiAim/AntiAimBeta.h"
#include "../../../Events/BadActors/BadActors.h"

void __ESP::RenderPlayers( CTFPlayer* pLocal )
{
	if ( !Vars::ESP::Players::esp_players.var )
		return;

	for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::PLAYERS_ALL ) )
	{
		const auto &pPlayer = pEntity->As<CTFPlayer>( );
		const auto &cResource = g_EntityCache.GetPR( );

		if ( pPlayer->deadflag( ) || !cResource->IsAlive( pPlayer->entindex( ) ) || pPlayer->InCond( TF_COND_HALLOWEEN_GHOST_MODE ) )
			continue;

		Color_t esp = { 255, 255, 255, 255 };
		if ( pPlayer->IsDormant( ) )
		{
			esp = { 170, 170, 170, 255 };
			if ( I::EngineClient->Time( ) - g_GlobalInfo.PartyPlayerESP[ pPlayer->entindex( ) ].LastUpdate >= 5.0f )
			{
				continue;
			}
		}

		player_info_t pi{};
		if ( !I::EngineClient->GetPlayerInfo( pPlayer->entindex( ), &pi ) )
			continue;

		if ( Vars::ESP::Main::esp_enemies.var && pPlayer != pLocal && pPlayer->m_iTeamNum( ) == pLocal->m_iTeamNum( ) && !pPlayer->PlayerOnFriendsList( ) )
			continue;

		int x = 0, y = 0, w = 0, h = 0;
		Vec3 vTrans[ 8 ];
		const auto &pWeapon = g_EntityCache.GetWeapon( );

		if ( Utils::GetDrawBounds( pPlayer, vTrans, x, y, w, h ) )
		{
			if ( pLocal && !pLocal->deadflag( ) && !pLocal->IsDormant( ) )
			{
				if ( pPlayer == pLocal && !I::Input->CAM_IsThirdPerson( ) )
					continue;
			}
			//name
			if ( Vars::ESP::Players::name.var )
			{
				int nTextOffset = 0;
				nTextOffset += g_Fonts[ EFonts::NAME ].m_nTall / 2;
				if ( Vars::ESP::Main::esp_cheaptext.var )
				{
					g_Draw.String( g_Fonts[ EFonts::CHEAPNAME ].m_dwFont, x + w / 2, y - nTextOffset - 2, esp, ALIGN_CENTER, Utils::ConvertUtf8ToWide( pi.m_sName ).c_str( ) );
				}
				else
				{
					g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, x + w / 2, y - nTextOffset - 2, esp, ALIGN_CENTER, Utils::ConvertUtf8ToWide( pi.m_sName ).c_str( ) );
				}
				/*g_Draw.String( g_Fonts[ EFonts::NAME ].m_dwFont, x + w + 10, y + nTextOffset - 1, { 255, 255, 255, 255 }, ALIGN_CENTER, "%.2f", g_GlobalInfo.PartyPlayerESP[ pEntity->entindex( ) ].LastUpdate );
				g_Draw.String( g_Fonts[ EFonts::NAME ].m_dwFont, x + w + 10, y + ( nTextOffset * 3 ) - 1, { 255, 255, 255, 255 }, ALIGN_CENTER, "%.2f", I::EngineClient->Time( ) );
				bool bDuration = I::EngineClient->Time( ) - g_GlobalInfo.PartyPlayerESP[ pEntity->entindex( ) ].LastUpdate <= 5.0f;
				g_Draw.String( g_Fonts[ EFonts::NAME ].m_dwFont, x + w + 10, y + ( nTextOffset * 5 ) - 1, { 255, 255, 255, 255 }, ALIGN_CENTER, "%i", bDuration );*/

				if ( pPlayer->PlayerOnFriendsList( ) )
				{
					if ( Vars::ESP::Main::esp_cheaptext.var )
					{
						g_Draw.String(
							g_Fonts[ EFonts::CHEAPNAME ].m_dwFont,
							x + ( w / 2 ), y - ( nTextOffset * 3 ),
							{ 0, 200, 255, 255 }, ALIGN_CENTER,
							L"FRIEND" );
					}
					else
					{
						g_Draw.OutlinedString(
							g_Fonts[ EFonts::NAME ].m_dwFont,
							x + ( w / 2 ), y - ( nTextOffset * 3 ),
							{ 0, 200, 255, 255 }, ALIGN_CENTER,
							L"FRIEND" );
					}

					nTextOffset += g_Fonts[ EFonts::WEAPON ].m_nTall;
				}
				else
				{
					switch ( g_GlobalInfo.PlayerPriority[ pi.friendsID ].Mode )
					{
						case 2:
						{
							if ( g_BadActors.mData[ pPlayer ].bSuspicious )
							{
								if ( Vars::ESP::Main::esp_cheaptext.var )
								{
									g_Draw.String(
										g_Fonts[ EFonts::CHEAPNAME ].m_dwFont,
										x + ( w / 2 ), y - ( nTextOffset * 3 ),
										{ 255, 150, 0, 255 }, ALIGN_CENTER,
										L"SUSPICIOUS" );
								}
								else
								{
									g_Draw.OutlinedString(
										g_Fonts[ EFonts::NAME ].m_dwFont,
										x + ( w / 2 ), y - ( nTextOffset * 3 ),
										{ 255, 150, 0, 255 }, ALIGN_CENTER,
										L"SUSPICIOUS" );
								}
							}
							break;
						}

						case 3:
						{
							if ( Vars::ESP::Main::esp_cheaptext.var )
							{
								g_Draw.String(
									g_Fonts[ EFonts::CHEAPNAME ].m_dwFont,
									x + ( w / 2 ), y - ( nTextOffset * 3 ),
									{ 255, 225, 0, 255 }, ALIGN_CENTER,
									L"RAGE" );
							}
							else
							{
								g_Draw.OutlinedString(
									g_Fonts[ EFonts::NAME ].m_dwFont,
									x + ( w / 2 ), y - ( nTextOffset * 3 ),
									{ 255, 225, 0, 255 }, ALIGN_CENTER,
									L"RAGE" );
							}
							break;
						}

						case 4:
						{
							if ( Vars::ESP::Main::esp_cheaptext.var )
							{
								g_Draw.String(
									g_Fonts[ EFonts::CHEAPNAME ].m_dwFont,
									x + ( w / 2 ), y - ( nTextOffset * 3 ),
									{ 255, 55, 0, 255 }, ALIGN_CENTER,
									L"CHEATER" );
							}
							else
							{
								g_Draw.OutlinedString(
									g_Fonts[ EFonts::NAME ].m_dwFont,
									x + ( w / 2 ), y - ( nTextOffset * 3 ),
									{ 255, 55, 0, 255 }, ALIGN_CENTER,
									L"CHEATER" );
							}
							break;
						}
						default: break;
					}
				}
			}

			//if ( pLocal->m_iClass( ) == SPY )

			//box
			if ( Vars::ESP::Players::box.var )
			{
				g_Draw.BlackOutlinedRect( x, y, w, h + 1, Colors::GetEntityColor( pPlayer, pLocal ) );
			}

			//health bar
			if ( Vars::ESP::Players::healthbar.var )
			{
				int nHealth = pPlayer->m_iHealth( );
				int nMaxHealth = pPlayer->GetMaxHealth( );
				static const int nWidth = 2;
				constexpr float duration = 0.005f;

				float flHealth = ( float )nHealth;
				float flMaxHealth = ( float )nMaxHealth;

				float &health = m_Health[ pPlayer->entindex( ) ];

				float ratio = std::clamp( health / flMaxHealth, 0.0f, 1.0f );
				int nHeight = h + ( flHealth < flMaxHealth ? 1 : 0 );

				if ( health > nHealth )
				{
					if ( nMaxHealth < 301 )
						health -= ( flMaxHealth * 2 / 1.0f ) * I::GlobalVars->frametime;
					else
						health -= ( flMaxHealth * 2 / 1.0f ) * I::GlobalVars->frametime * flMaxHealth / 300.f;
				}
				else
				{
					health = nHealth;
				}

				Color_t Health = Utils::GetHealthColor( nHealth, nMaxHealth );

				g_Draw.Rect( x - nWidth - 4, y - 1, nWidth * 2, h + 3, { 0, 0, 0, 255 } );
				if ( Vars::ESP::Players::gradient.var )
					//g_Draw.GradientRectWH( x - nWidth - 3, y + nHeight - ( nHeight * ratio ), nWidth, nHeight * ratio + 1, { 128, 200, 0, 255 }, { 240, 80, 60, 255 }, false );
				{
					Color_t top_clr = { 128, 200, 0, 255 };
					Color_t bottom_clr = { 240, 80, 60, 255 };
					I::Surface->DrawSetColor( top_clr.r, top_clr.g, top_clr.b, top_clr.a );
					I::Surface->DrawFilledRectFade( x - nWidth - 3, y + nHeight - ( nHeight * ratio ), x - nWidth, y + nHeight - ( nHeight * ratio ) + ( nHeight * ratio + 1 ), 255, 255, false );
					I::Surface->DrawSetColor( bottom_clr.r, bottom_clr.g, bottom_clr.b, bottom_clr.a );
					I::Surface->DrawFilledRectFade( x - nWidth - 3, y + nHeight - ( nHeight * ratio ), x - nWidth, y + nHeight - ( nHeight * ratio ) + ( nHeight * ratio + 1 ), Utils::GetAlpha( health, nMaxHealth ), 255, false );
					//g_Draw.String( g_Fonts[ EFonts::CONDS ].m_dwFont, x + w + 5, y + h + 3, { 255, 255, 255, 255 }, ALIGN_DEFAULT, L"%i", Utils::GetAlpha( health, nMaxHealth ) );
				}
				else
					g_Draw.Rect( x - nWidth - 3, y + nHeight - ( nHeight * ratio ), nWidth, nHeight * ratio + 1, Health );
				g_Draw.OutlinedRect( x - nWidth - 4, y - 1, nWidth * 2, h + 3, { 0, 0, 0, 255 } );

				if ( Vars::ESP::Players::lines.var > 0 )
				{
					for ( int i = 1; i <= Vars::ESP::Players::lines.var; i++ )
					{
						if ( i == 1 )
							yline = 1.f / ( Vars::ESP::Players::lines.var + 1 );

						g_Draw.Line( x - nWidth - 4, y + h * yline, x - nWidth - 1, y + h * yline, { 0, 0, 0, 255 } );
						yline += 1.f / ( Vars::ESP::Players::lines.var + 1 );
					}
				}

				if ( nHealth > nMaxHealth )
				{
					g_Draw.Rect( x - nWidth - 3, y, nWidth, nHeight + 1, { 65, 155, 220, 255 } );
					g_Draw.OutlinedRect( x - nWidth - 4, y - 1, nWidth * 2, h + 3, { 0, 0, 0, 255 } );

					if ( Vars::ESP::Players::lines.var > 0 )
					{
						for ( int i = 1; i <= Vars::ESP::Players::lines.var; i++ )
						{
							if ( i == 1 )
								yline = 1.f / ( Vars::ESP::Players::lines.var + 1 );

							g_Draw.Line( x - nWidth - 4, y + h * yline, x - nWidth - 1, y + h * yline, { 0, 0, 0, 255 } );
							yline += 1.f / ( Vars::ESP::Players::lines.var + 1 );
						}
					}
				}
			}

			//uber bar
			{
				if ( pPlayer->m_iClass( ) == MEDIC )
				{
					if ( const auto &pMedgun = pPlayer->GetWeaponFromSlot( 1 )->As<CWeaponMedigun>( ) )
					{
						float flMaxUber = ( pMedgun->m_iItemDefinitionIndex( ) == Medic_s_TheVaccinator ? 400.0f : 100.0f );
						float flUber = pMedgun->m_flChargeLevel( ) * flMaxUber;

						if ( flUber > flMaxUber )
						{
							flUber = flMaxUber;
						}

						static constexpr int RECT_WIDTH = 2;
						int nHeight = h + ( flUber < flMaxUber ? 1 : 0 );
						int nHeight2 = h + 1;

						float ratio = flUber / flMaxUber;
						g_Draw.Rect( x + RECT_WIDTH - 13, y - 1, RECT_WIDTH + 2, nHeight2 + 2, { 0, 0, 0, 255 } );
						g_Draw.Rect( x + RECT_WIDTH - 12, y + nHeight - ( nHeight * ratio ), RECT_WIDTH, nHeight * ratio + 1,
									 { 224, 86, 253, 255 } );
						g_Draw.OutlinedRect( x + RECT_WIDTH - 13, y - 1, RECT_WIDTH + 2, nHeight2 + 2, { 0, 0, 0, 255 } );

						g_Draw.Line( x - RECT_WIDTH - 9, y + h / 2, x - RECT_WIDTH - 6, y + h / 2, { 0, 0, 0, 255 } );//middle
						g_Draw.Line( x - RECT_WIDTH - 9, y + h / 4, x - RECT_WIDTH - 6, y + h / 4, { 0, 0, 0, 255 } );//top
						g_Draw.Line( x - RECT_WIDTH - 9, y + h * 0.75, x - RECT_WIDTH - 6, y + h * 0.75, { 0, 0, 0, 255 } );//bottom
					}
				}
			}

			//conds
			if ( Vars::ESP::Players::conds.var )
			{
				y -= 2;
				int nTextOffset = 0;
				/*if ( strstr( pi.guid, "[U:1:356068859]" ) )
				{
					g_Draw.String( g_Fonts[ EFonts::CONDS ].m_dwFont, x + w + 2, y + nTextOffset, { 255, 255, 255, 255 }, ALIGN_DEFAULT, L"DEV" );
					nTextOffset += g_Fonts[ EFonts::CONDS ].m_nTall;
				}*/

				if ( pPlayer == pLocal )
				{
					if ( Vars::AntiHack::AntiAimBeta::Active.var && g_AntiAimBeta.bRealFlip )
					{
						if ( Vars::ESP::Main::esp_cheaptext.var )
						{
							g_Draw.String( g_Fonts[ EFonts::CHEAPWEAPON ].m_dwFont,
										   x + w + 2, y + nTextOffset,
										   { 0, 255, 0, 255 }, ALIGN_DEFAULT,
										   L"REAL CYCLED" );
						}
						else
						{
							g_Draw.OutlinedString( g_Fonts[ EFonts::WEAPON ].m_dwFont,
												   x + w + 2, y + nTextOffset,
												   { 0, 255, 0, 255 }, ALIGN_DEFAULT,
												   L"REAL CYCLED" );
						}

						nTextOffset += g_Fonts[ EFonts::WEAPON ].m_nTall;
					}

					if ( Vars::AntiHack::AntiAimBeta::Active.var && Vars::AntiHack::AntiAimBeta::Rage.var && g_AntiAimBeta.bFakeFlip )
					{
						if ( Vars::ESP::Main::esp_cheaptext.var )
						{
							g_Draw.String( g_Fonts[ EFonts::CHEAPWEAPON ].m_dwFont,
										   x + w + 2, y + nTextOffset,
										   { 255, 0, 0, 255 }, ALIGN_DEFAULT,
										   L"FAKE CYCLED" );
						}
						else
						{
							g_Draw.OutlinedString( g_Fonts[ EFonts::WEAPON ].m_dwFont,
												   x + w + 2, y + nTextOffset,
												   { 255, 0, 0, 255 }, ALIGN_DEFAULT,
												   L"FAKE CYCLED" );
						}

						nTextOffset += g_Fonts[ EFonts::WEAPON ].m_nTall;
					}
				}

				if ( pPlayer->m_iHealth( ) > pPlayer->GetMaxHealth( ) )
				{
					if ( Vars::ESP::Main::esp_cheaptext.var )
					{
						g_Draw.String( g_Fonts[ EFonts::CHEAPWEAPON ].m_dwFont,
									   x + w + 2, y + nTextOffset,
									   { 150, 255, 150, 255 }, ALIGN_DEFAULT,
									   L"+%i HP", pPlayer->m_iHealth( ) - pPlayer->GetMaxHealth( ) );
					}
					else
					{
						g_Draw.OutlinedString( g_Fonts[ EFonts::WEAPON ].m_dwFont,
									   x + w + 2, y + nTextOffset,
									   { 150, 255, 150, 255 }, ALIGN_DEFAULT,
									   L"+%i HP", pPlayer->m_iHealth( ) - pPlayer->GetMaxHealth( ) );
					}

					nTextOffset += g_Fonts[ EFonts::WEAPON ].m_nTall;
				}

				if ( pPlayer->InCond( TF_COND_FEIGN_DEATH ) || pPlayer->InCond( TF_COND_STEALTHED_BLINK ) || pPlayer->InCond( TF_COND_STEALTHED ) )
				{
					if ( Vars::ESP::Main::esp_cheaptext.var )
					{
						g_Draw.String( g_Fonts[ EFonts::CHEAPWEAPON ].m_dwFont, x + w + 2, y + nTextOffset, { 200, 200, 200, 255 }, ALIGN_DEFAULT, L"CLOAKED" );
					}
					else
					{
						g_Draw.OutlinedString( g_Fonts[ EFonts::WEAPON ].m_dwFont, x + w + 2, y + nTextOffset, { 200, 200, 200, 255 }, ALIGN_DEFAULT, L"CLOAKED" );
					}
					
					nTextOffset += g_Fonts[ EFonts::WEAPON ].m_nTall;
				}

				if ( pPlayer->InCond( TF_COND_INVULNERABLE ) )
				{
					if ( Vars::ESP::Main::esp_cheaptext.var )
					{
						g_Draw.String( g_Fonts[ EFonts::CHEAPWEAPON ].m_dwFont, x + w + 2, y + nTextOffset, { 255, 110, 255, 255 }, ALIGN_DEFAULT, L"UBERCHARGE" );
					}
					else
					{
						g_Draw.OutlinedString( g_Fonts[ EFonts::WEAPON ].m_dwFont, x + w + 2, y + nTextOffset, { 255, 110, 255, 255 }, ALIGN_DEFAULT, L"UBERCHARGE" );
					}
					
					nTextOffset += g_Fonts[ EFonts::WEAPON ].m_nTall;
				}

				if ( pPlayer->IsCritBoostedNoMini( ) )
				{
					if ( Vars::ESP::Main::esp_cheaptext.var )
					{
						g_Draw.String( g_Fonts[ EFonts::CHEAPWEAPON ].m_dwFont, x + w + 2, y + nTextOffset, { 255, 150, 255, 255 }, ALIGN_DEFAULT, L"CRIT BOOST" );
					}
					else
					{
						g_Draw.OutlinedString( g_Fonts[ EFonts::WEAPON ].m_dwFont, x + w + 2, y + nTextOffset, { 255, 150, 255, 255 }, ALIGN_DEFAULT, L"CRIT BOOST" );
					}
					
					nTextOffset += g_Fonts[ EFonts::WEAPON ].m_nTall;
				}

				int kills = pPlayer->m_iKills( ) == 0 ? 1 : pPlayer->m_iKills( );
				int deaths = pPlayer->m_iDeaths( ) == 0 ? 1 : pPlayer->m_iDeaths( );
				float kd = ( float )kills / ( float )deaths;
				if ( kd > 10.f )
				{
					if ( Vars::ESP::Main::esp_cheaptext.var )
					{
						g_Draw.String( g_Fonts[ EFonts::CHEAPWEAPON ].m_dwFont, x + w + 2, y + nTextOffset, { 255, 255, 0, 255 }, ALIGN_DEFAULT, L"HIGH K/D" );
					}
					else
					{
						g_Draw.OutlinedString( g_Fonts[ EFonts::WEAPON ].m_dwFont, x + w + 2, y + nTextOffset, { 255, 255, 0, 255 }, ALIGN_DEFAULT, L"HIGH K/D" );
					}
					nTextOffset += g_Fonts[ EFonts::WEAPON ].m_nTall;
				}

				for ( int i = 0; i < 130; i++ )
				{
					if ( pPlayer->InCond( static_cast< ETFCond >( i ) ) )
					{
						Utils::Cond conds = Utils::GetCondByID( static_cast< ETFCond >( i ) );
						if ( Vars::ESP::Main::esp_cheaptext.var )
						{
							g_Draw.String( 
								g_Fonts[ EFonts::CHEAPWEAPON ].m_dwFont,
								x + w + 2, y + nTextOffset,
								conds.color, ALIGN_DEFAULT,
								conds.name
							);
						}
						else
						{
							g_Draw.OutlinedString(
								g_Fonts[ EFonts::WEAPON ].m_dwFont,
								x + w + 2, y + nTextOffset,
								conds.color, ALIGN_DEFAULT,
								conds.name
							);
						}

						if ( wcslen( conds.name ) != 0 )
							nTextOffset += g_Fonts[ EFonts::WEAPON ].m_nTall;
					}
				}

				nTextOffset += g_Fonts[ EFonts::WEAPON ].m_nTall;

				y += 2;
			}

			int esptext = 0;
			//weapontext
			if ( Vars::ESP::Players::weapons.var )
			{
				if ( const auto &pWeapon = GET_ENT_H( pPlayer->m_hActiveWeapon( ) )->As<CTFWeaponBase>( ) )
				{
					if ( Vars::ESP::Main::esp_cheaptext.var )
					{
						g_Draw.String( g_Fonts[ EFonts::CHEAPWEAPON ].m_dwFont, x + ( w / 2 ), y + h + 6, esp, ALIGN_CENTER, Utils::GetWeaponNameByType( pWeapon ) );
					}
					else
					{
						g_Draw.OutlinedString( g_Fonts[ EFonts::WEAPON ].m_dwFont, x + ( w / 2 ), y + h + 6, esp, ALIGN_CENTER, Utils::GetWeaponNameByType( pWeapon ) );
					}
				}
				esptext += g_Fonts[ EFonts::WEAPON ].m_nTall;
			}

			if ( Vars::ESP::Players::distance.var )
			{
				if ( pLocal && !pLocal->deadflag( ) && !pLocal->IsDormant( ) && pPlayer != pLocal )
				{
					const Vec3 localOrigin = pLocal->GetAbsOrigin( );
					const Vec3 playerOrigin = pPlayer->GetAbsOrigin( );

					float distance = sqrt( powf( localOrigin.x - playerOrigin.x, 2 ) + powf( localOrigin.y - playerOrigin.y, 2 ) + powf( localOrigin.z - playerOrigin.z, 2 ) ) * 0.01905f;
					int realdistance = round( distance / 5 ) * 5;

					if ( Vars::ESP::Main::esp_cheaptext.var )
					{
						g_Draw.String( g_Fonts[ EFonts::CHEAPCONDS ].m_dwFont, x + w / 2, y + h + 7 + esptext, esp, ALIGN_CENTER, L"[ %im ]", /*real*/( int )distance );
					}
					else
					{
						g_Draw.OutlinedString( g_Fonts[ EFonts::CONDS ].m_dwFont, x + w / 2, y + h + 7 + esptext, esp, ALIGN_CENTER, L"[ %im ]", /*real*/( int )distance );
					}
					esptext += g_Fonts[ EFonts::CONDS ].m_nTall;
				}
			}

			//bones
			if ( Vars::ESP::Players::skeleton.var )
			{
				DrawBones( pPlayer, { 8, 7, 6, 4 }, { 255, 255, 255, 255 } );
				DrawBones( pPlayer, { 11, 10, 9, 4 }, { 255, 255, 255, 255 } );
				DrawBones( pPlayer, { 0, 4, 1 }, { 255, 255, 255, 255 } );
				DrawBones( pPlayer, { 14, 13, 1 }, { 255, 255, 255, 255 } );
				DrawBones( pPlayer, { 17, 16, 1 }, { 255, 255, 255, 255 } );
			}
		}
	}
}

void __ESP::RenderBuildings( CTFPlayer *pLocal )
{
	if ( !Vars::ESP::Buildings::esp_buildings.var )
		return;

	for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::BUILDINGS_ALL ) )
	{
		const auto &pObject = pEntity->As<CBaseObject>( );

		int x = 0, y = 0, w = 0, h = 0;
		Vec3 vTrans[ 8 ];

		if ( Utils::GetDrawBounds( pObject, vTrans, x, y, w, h ) )
		{
			if ( Vars::ESP::Main::esp_enemies.var && pObject->m_iTeamNum( ) == pLocal->m_iTeamNum( ) && GET_ENT_H( pObject->m_hBuilder( ) ) != pLocal )
				continue;

			//name
			if ( Vars::ESP::Buildings::name.var )
			{
				int nTextOffset = 0;
				nTextOffset += g_Fonts[ EFonts::NAME ].m_nTall / 2;
				if ( Vars::ESP::Main::esp_cheaptext.var )
				{
					g_Draw.String( g_Fonts[ EFonts::CHEAPNAME ].m_dwFont, x + w / 2, y - nTextOffset, { 255, 255, 255, 255 }, ALIGN_CENTER, G::GetBuildingName( pObject ).c_str( ) );
				}
				else
				{
					g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, x + w / 2, y - nTextOffset, { 255, 255, 255, 255 }, ALIGN_CENTER, G::GetBuildingName( pObject ).c_str( ) );
				}
			}

			//box
			if ( Vars::ESP::Buildings::box.var )
				g_Draw.BlackOutlinedRect( x, y, w, h, Colors::GetBuildingColor( pObject, pLocal ) );

			//healthbar
			if ( Vars::ESP::Buildings::healthbar.var )
			{
				int nHealth = pObject->m_iHealth( );
				int nMaxHealth = pObject->m_iMaxHealth( );
				static const int nWidth = 2;

				float flHealth = ( float )nHealth;
				float flMaxHealth = ( float )nMaxHealth;
				float &health = m_HealthBuildings[ pObject->entindex( ) ];

				float ratio = std::clamp( health / flMaxHealth, 0.0f, 1.0f );
				int nHeight = h + ( flHealth < flMaxHealth ? 1 : 0 );

				if ( health > nHealth )
				{
					health -= ( flMaxHealth / 1.0f ) * I::GlobalVars->frametime;
				}
				else
				{
					health = nHealth;
				}

				Color_t Health = Utils::GetHealthColor( nHealth, nMaxHealth );

				g_Draw.Rect( x - nWidth - 4, y - 1, nWidth * 2, h + 3, { 0, 0, 0, 255 } );
				if ( Vars::ESP::Buildings::gradient.var )
					//g_Draw.GradientRectWH( x - nWidth - 3, y + nHeight - ( nHeight * ratio ), nWidth, nHeight * ratio + 1, { 128, 200, 0, 255 }, { 240, 80, 60, 255 }, false );
				{
					Color_t top_clr = { 128, 200, 0, 255 };
					Color_t bottom_clr = { 240, 80, 60, 255 };
					I::Surface->DrawSetColor( top_clr.r, top_clr.g, top_clr.b, top_clr.a );
					I::Surface->DrawFilledRectFade( x - nWidth - 3, y + nHeight - ( nHeight * ratio ), x - nWidth, y + nHeight - ( nHeight * ratio ) + ( nHeight * ratio + 1 ), 255, 255, false );
					I::Surface->DrawSetColor( bottom_clr.r, bottom_clr.g, bottom_clr.b, bottom_clr.a );
					I::Surface->DrawFilledRectFade( x - nWidth - 3, y + nHeight - ( nHeight * ratio ), x - nWidth, y + nHeight - ( nHeight * ratio ) + ( nHeight * ratio + 1 ), Utils::GetAlpha( nHealth, nMaxHealth ), 255, false );
					//g_Draw.String( g_Fonts[ EFonts::CONDS ].m_dwFont, x + w + 5, y + h + 3, { 255, 255, 255, 255 }, ALIGN_DEFAULT, L"%i", Utils::GetAlpha( nHealth, nMaxHealth ) );
				}
				else
					g_Draw.Rect( x - nWidth - 3, y + nHeight - ( nHeight * ratio ), nWidth, nHeight * ratio + 1, Health );
				g_Draw.OutlinedRect( x - nWidth - 4, y - 1, nWidth * 2, h + 3, { 0, 0, 0, 255 } );

				if ( Vars::ESP::Buildings::lines.var > 0 )
				{
					for ( int i = 1; i <= Vars::ESP::Buildings::lines.var; i++ )
					{
						if ( i == 1 )
							yline = 1.f / ( Vars::ESP::Buildings::lines.var + 1 );

						g_Draw.Line( x - nWidth - 4, y + h * yline, x - nWidth - 1, y + h * yline, { 0, 0, 0, 255 } );
						yline += 1.f / ( Vars::ESP::Buildings::lines.var + 1 );
					}
				}
			}

			if ( Vars::ESP::Buildings::conds.var )
			{
				int nTextOffset = 0;
				y -= 2;
				if ( pEntity->GetClassId( ) == EClassIds::CObjectTeleporter )
				{
					float recharge = pObject->As<CObjectTeleporter>( )->m_flRechargeTime( ) - I::GlobalVars->curtime + 0.05f;
					if ( pObject->As<CObjectTeleporter>( )->m_iState( ) == 6 && recharge > 0 )
					{
						if ( Vars::ESP::Main::esp_cheaptext.var )
						{
							g_Draw.String(
								g_Fonts[ EFonts::CHEAPWEAPON ].m_dwFont,
								x + w + 3, y + nTextOffset,
								{ 255, 255, 255, 255 }, ALIGN_DEFAULT,
								L"RECHARGES IN %.1f SEC", ( recharge )
							);
						}
						else
						{
							g_Draw.OutlinedString(
								g_Fonts[ EFonts::WEAPON ].m_dwFont,
								x + w + 3, y + nTextOffset,
								{ 255, 255, 255, 255 }, ALIGN_DEFAULT,
								L"RECHARGES IN %.1f SEC", ( recharge )
							);
						}
						nTextOffset += g_Fonts[ EFonts::WEAPON ].m_nTall;
					}
				}
				int level = pObject->m_iUpgradeLevel( );
				if ( level )
				{
					if ( Vars::ESP::Main::esp_cheaptext.var )
					{
						g_Draw.String(
							g_Fonts[ EFonts::CHEAPWEAPON ].m_dwFont,
							x + w + 3, y + nTextOffset,
							{ 255, 255, 255, 255 }, ALIGN_DEFAULT,
							L"LEVEL %i", ( level )
						);
					}
					else
					{
						g_Draw.OutlinedString(
							g_Fonts[ EFonts::WEAPON ].m_dwFont,
							x + w + 3, y + nTextOffset,
							{ 255, 255, 255, 255 }, ALIGN_DEFAULT,
							L"LEVEL %i", ( level )
						);
					}
					nTextOffset += g_Fonts[ EFonts::WEAPON ].m_nTall;
				}
				y += 2;
			}

			if ( Vars::ESP::Buildings::distance.var )
			{
				if ( pLocal && !pLocal->deadflag( ) && !pLocal->IsDormant( ) )
				{
					const Vec3 localOrigin = pLocal->GetAbsOrigin( );
					const Vec3 playerOrigin = pObject->GetAbsOrigin( );

					float distance = sqrt( powf( localOrigin.x - playerOrigin.x, 2 ) + powf( localOrigin.y - playerOrigin.y, 2 ) + powf( localOrigin.z - playerOrigin.z, 2 ) ) * 0.01905f;
					int realdistance = round( distance / 5 ) * 5;

					if ( Vars::ESP::Main::esp_cheaptext.var )
					{
						g_Draw.String(
							g_Fonts[ EFonts::CHEAPCONDS ].m_dwFont,
							x + w / 2, y + h + 6,
							{ 255, 255, 255, 255 }, ALIGN_CENTER,
							L"[ %im ]", /*real*/( int )distance
						);
					}
					else
					{
						g_Draw.OutlinedString(
							g_Fonts[ EFonts::CONDS ].m_dwFont,
							x + w / 2, y + h + 6,
							{ 255, 255, 255, 255 }, ALIGN_CENTER,
							L"[ %im ]", /*real*/( int )distance
						);
					}
				}
			}
		}
	}
}

void __ESP::RenderWorld( CTFPlayer *pLocal )
{
	if ( !Vars::ESP::World::esp_world.var )
		return;

	for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::WORLD_ALL ) )
	{
		const auto &pAnimated = pEntity->As<CBaseEntity>( );

		int x = 0, y = 0, w = 0, h = 0;
		Vec3 vTrans[ 8 ];

		if ( Utils::GetDrawBounds( pAnimated, vTrans, x, y, w, h ) )
		{
			if ( pEntity->GetClassId( ) == EClassIds::CCaptureFlag )
			{
				int nTextOffset = 0; y -= 2;
				const auto &pIntel = pAnimated->As<CCaptureFlag>( );

				if ( pIntel->m_nFlagStatus( ) == 1 )
					continue;

				if ( Vars::ESP::Main::esp_cheaptext.var )
				{
					g_Draw.String(
						g_Fonts[ EFonts::CHEAPWEAPON ].m_dwFont,
						x + w + 3, y + nTextOffset,
						{ 255, 255, 255, 255 }, ALIGN_DEFAULT,
						G::GetIntelStatus( pIntel->m_nFlagStatus( ) )
					);
				}
				else
				{
					g_Draw.OutlinedString(
						g_Fonts[ EFonts::WEAPON ].m_dwFont,
						x + w + 3, y + nTextOffset,
						{ 255, 255, 255, 255 }, ALIGN_DEFAULT,
						G::GetIntelStatus( pIntel->m_nFlagStatus( ) )
					);
				}

				nTextOffset += g_Fonts[ EFonts::DEBUG ].m_nTall;

				if ( pIntel->m_nFlagStatus( ) == 2 )
				{
					if ( Vars::ESP::Main::esp_cheaptext.var )
					{
						g_Draw.String(
							g_Fonts[ EFonts::CHEAPWEAPON ].m_dwFont,
							x + w + 3, y + nTextOffset,
							{ 255, 255, 255, 255 }, ALIGN_DEFAULT,
							L"RETURNS IN %.1f SEC", ( pIntel->m_flResetTime( ) - I::GlobalVars->curtime + 0.2f )
						);
					}
					else
					{
						g_Draw.OutlinedString(
							g_Fonts[ EFonts::WEAPON ].m_dwFont,
							x + w + 3, y + nTextOffset,
							{ 255, 255, 255, 255 }, ALIGN_DEFAULT,
							L"RETURNS IN %.1f SEC", ( pIntel->m_flResetTime( ) - I::GlobalVars->curtime + 0.2f )
						);
					}
				}
				y += 2;
			}

			//name
			if ( Vars::ESP::World::name.var )
			{
				int nTextOffset = 0;
				nTextOffset += g_Fonts[ EFonts::NAME ].m_nTall / 2;
				if ( Vars::ESP::Main::esp_cheaptext.var )
				{
					g_Draw.String( g_Fonts[ EFonts::CHEAPNAME ].m_dwFont, x + w / 2, y - nTextOffset, { 255, 255, 255, 255 }, ALIGN_CENTER, G::GetWorldName( pAnimated ).c_str( ) );
				}
				else
				{
					g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, x + w / 2, y - nTextOffset, { 255, 255, 255, 255 }, ALIGN_CENTER, G::GetWorldName( pAnimated ).c_str( ) );
				}
			}

			//box
			if ( Vars::ESP::World::box.var )
				g_Draw.BlackOutlinedRect( x, y, w, h, Colors::GetWorldColor( pAnimated, pLocal ) );

			if ( Vars::ESP::World::distance.var )
			{
				if ( pLocal && !pLocal->deadflag( ) && !pLocal->IsDormant( ) )
				{
					const Vec3 localOrigin = pLocal->GetAbsOrigin( );
					const Vec3 playerOrigin = pAnimated->GetAbsOrigin( );

					float distance = sqrt( powf( localOrigin.x - playerOrigin.x, 2 ) + powf( localOrigin.y - playerOrigin.y, 2 ) + powf( localOrigin.z - playerOrigin.z, 2 ) ) * 0.01905f;
					int realdistance = round( distance / 5 ) * 5;

					if ( Vars::ESP::Main::esp_cheaptext.var )
					{
						g_Draw.String(
							g_Fonts[ EFonts::CHEAPCONDS ].m_dwFont,
							x + w / 2, y + h + 6,
							{ 255, 255, 255, 255 }, ALIGN_CENTER,
							L"[ %im ]", /*real*/( int )distance
						);
					}
					else
					{
						g_Draw.OutlinedString(
							g_Fonts[ EFonts::CONDS ].m_dwFont,
							x + w / 2, y + h + 6,
							{ 255, 255, 255, 255 }, ALIGN_CENTER,
							L"[ %im ]", /*real*/( int )distance
						);
					}
				}
			}
		}
	}
}

void __ESP::RenderNPC( CTFPlayer *pLocal )
{
	for ( const auto &pEntity : g_EntityCache.GetGroup( EEntGroup::WORLD_NPC ) )
	{
		const auto &pNpc = pEntity->As<CBaseEntity>( );

		if ( !pNpc || !pNpc->IsAlive( ) || pNpc->IsDormant( ) )
			continue;

		int x = 0, y = 0, w = 0, h = 0;
		Vec3 vTrans[ 8 ];

		if ( Utils::GetDrawBounds( pNpc, vTrans, x, y, w, h ) )
		{
			//name
			if ( Vars::ESP::World::name.var )
			{
				std::wstring wsName = G::GetNPCName( pNpc );

				int nTextOffset = 0;
				nTextOffset += g_Fonts[ EFonts::NAME ].m_nTall / 2;
				if ( Vars::ESP::Main::esp_cheaptext.var )
				{
					g_Draw.String( g_Fonts[ EFonts::CHEAPNAME ].m_dwFont, x + w / 2, y - nTextOffset - 2, { 255, 255, 255, 255 }, ALIGN_CENTER, wsName.c_str( ) );
				}
				else
				{
					g_Draw.OutlinedString( g_Fonts[ EFonts::NAME ].m_dwFont, x + w / 2, y - nTextOffset - 2, { 255, 255, 255, 255 }, ALIGN_CENTER, wsName.c_str( ) );
				}
			}

			//box
			if ( Vars::ESP::World::box.var )
			{
				g_Draw.BlackOutlinedRect( x, y, w, h, Colors::GetColorNPC( pNpc, pLocal ) );
			}

			//distance
			if ( Vars::ESP::World::distance.var )
			{
				if ( pLocal && !pLocal->deadflag( ) && !pLocal->IsDormant( ) )
				{
					const Vec3 localOrigin = pLocal->GetAbsOrigin( );
					const Vec3 playerOrigin = pNpc->GetAbsOrigin( );

					float distance = sqrt( powf( localOrigin.x - playerOrigin.x, 2 ) + powf( localOrigin.y - playerOrigin.y, 2 ) + powf( localOrigin.z - playerOrigin.z, 2 ) ) * 0.01905f;
					int realdistance = round( distance / 5 ) * 5;

					if ( Vars::ESP::Main::esp_cheaptext.var )
					{
						g_Draw.String( g_Fonts[ EFonts::CHEAPCONDS ].m_dwFont, x + w / 2, y + h + 7, { 255, 255, 255, 255 }, ALIGN_CENTER, L"[ %im ]", ( int )/*real*/distance );
					}
					else
					{
						g_Draw.OutlinedString( g_Fonts[ EFonts::CONDS ].m_dwFont, x + w / 2, y + h + 7, { 255, 255, 255, 255 }, ALIGN_CENTER, L"[ %im ]", ( int )/*real*/distance );
					}
				}
			}
		}
	}
}

void __ESP::Render( )
{
	const auto& pLocal = g_EntityCache.GetLocal( );
	
	if ( !Vars::ESP::Main::esp_global.var || I::EngineVGui->IsGameUIVisible( ) || !I::EngineClient->IsInGame( ) )
		return;

	RenderWorld( pLocal );
	RenderBuildings( pLocal );
	RenderPlayers( pLocal );
	//RenderNPC( pLocal );
}

void __ESP::DrawBones( CTFPlayer *pPlayer, const std::vector<int> &vecBones, Color_t clr )
{
	const size_t nMax = vecBones.size( ), nLast = nMax - 1;
	for ( size_t n = 0; n < nMax; n++ )
	{
		if ( n == nLast )
		{
			continue;
		}

		const auto vBone = pPlayer->GetHitboxPos( vecBones[ n ] );
		const auto vParent = pPlayer->GetHitboxPos( vecBones[ n + 1 ] );

		Vec3 vScreenBone, vScreenParent;

		if ( Utils::W2S( vBone, vScreenBone ) && Utils::W2S( vParent, vScreenParent ) )
		{
			g_Draw.Line( vScreenBone.x, vScreenBone.y, vScreenParent.x, vScreenParent.y, clr );
		}
	}
}