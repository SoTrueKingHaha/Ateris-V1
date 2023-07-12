#include "EntityCache.h"
#include "../../../Utils/Hash/Hash.h"
#include "../../../Features/Vars.h"

void CEntityCache::Fill()
{
	if (m_pLocal = GET_ENT_I(LOCAL_IDX)->As<CTFPlayer>())
	{
		int nLocalTeam = m_pLocal->m_iTeamNum();

		if (nLocalTeam != TEAM_RED && nLocalTeam != TEAM_BLU) {
			m_pLocal = nullptr;
			return;
		}

		m_pWeapon = GET_ENT_H(m_pLocal->m_hActiveWeapon())->As<CTFWeaponBase>();

		for (int n = 1; n < I::ClientEntityList->GetHighestEntityIndex(); n++)
		{
			IClientEntity *pEntity = GET_ENT_I(n);
			auto pModel = pEntity->As<CBaseEntity>();

			if ( !pEntity )
			{
				continue;
			}

			auto nClassID = pEntity->GetClassId( );

			if ( nClassID == EClassIds::CTFPlayer )
			{
				if ( pEntity->IsDormant( ) && !g_GlobalInfo.PartyPlayerESP.count( pEntity->entindex( ) ) && !Vars::ESP::Main::esp_dormant.var )
				{
					continue;
				}

				if ( pEntity->IsDormant( ) && !Vars::ESP::Main::esp_dormant.var )
				{
					if ( I::EngineClient->Time( ) - g_GlobalInfo.PartyPlayerESP[ pEntity->entindex( ) ].LastUpdate <= 5.0f )
					{
						pEntity->As<CTFPlayer>( )->SetAbsOrigin( g_GlobalInfo.PartyPlayerESP[ pEntity->entindex( ) ].Location );
						pEntity->As<CTFPlayer>( )->m_vecOrigin( ) = g_GlobalInfo.PartyPlayerESP[ pEntity->entindex( ) ].Location;
					}
				}
			}
			else
			{
				if ( pEntity->IsDormant( ) )
				{
					continue;
				}
			}

			switch (pEntity->GetClassId())
			{
				case EClassIds::CTFPlayer:
				{
					int nPlayerTeam = pEntity->As<CTFPlayer>()->m_iTeamNum();

					if (nPlayerTeam != TEAM_RED && nPlayerTeam != TEAM_BLU)
						continue;

					m_Groups[EEntGroup::PLAYERS_ALL].push_back(pEntity);
					m_Groups[nLocalTeam != nPlayerTeam ? EEntGroup::PLAYERS_ENEMIES : EEntGroup::PLAYERS_TEAMMATES].push_back(pEntity);

					break;
				}

				case EClassIds::CObjectSentrygun:
				{
					int nObjectTeam = pEntity->As<CBaseObject>()->m_iTeamNum();

					m_Groups[EEntGroup::SENTRY_ALL].push_back(pEntity);
					m_Groups[EEntGroup::BUILDINGS_ALL].push_back(pEntity);
					m_Groups[nLocalTeam != nObjectTeam ? EEntGroup::SENTRY_ENEMIES : EEntGroup::SENTRY_TEAMMATES].push_back(pEntity);
					m_Groups[nLocalTeam != nObjectTeam ? EEntGroup::BUILDINGS_ENEMIES : EEntGroup::BUILDINGS_TEAMMATES].push_back(pEntity);
					break;
				}
				case EClassIds::CObjectDispenser:
				{
					int nObjectTeam = pEntity->As<CBaseObject>()->m_iTeamNum();

					m_Groups[EEntGroup::DISPENSER_ALL].push_back(pEntity);
					m_Groups[EEntGroup::BUILDINGS_ALL].push_back(pEntity);
					m_Groups[nLocalTeam != nObjectTeam ? EEntGroup::DISPENSER_ENEMIES : EEntGroup::DISPENSER_TEAMMATES].push_back(pEntity);
					m_Groups[nLocalTeam != nObjectTeam ? EEntGroup::BUILDINGS_ENEMIES : EEntGroup::BUILDINGS_TEAMMATES].push_back(pEntity);
					break;
				}
				case EClassIds::CObjectTeleporter:
				{
					int nObjectTeam = pEntity->As<CBaseObject>()->m_iTeamNum();

					m_Groups[EEntGroup::TELEPORTER_ALL].push_back(pEntity);
					m_Groups[EEntGroup::BUILDINGS_ALL].push_back(pEntity);
					m_Groups[nLocalTeam != nObjectTeam ? EEntGroup::TELEPORTER_ENEMIES : EEntGroup::TELEPORTER_TEAMMATES].push_back(pEntity);
					m_Groups[nLocalTeam != nObjectTeam ? EEntGroup::BUILDINGS_ENEMIES : EEntGroup::BUILDINGS_TEAMMATES].push_back(pEntity);
					break;
				}

				case EClassIds::CBaseAnimating:
				{
					const auto szName = pModel->GetModelName();
					if (Hash::IsAmmo(szName))
					{
						m_Groups[EEntGroup::WORLD_AMMO].push_back(pEntity);
						m_Groups[ EEntGroup::WORLD_ALL ].push_back( pEntity );
						break;
					}
					if (Hash::IsHealth(szName))
					{
						m_Groups[EEntGroup::WORLD_HEALTH].push_back(pEntity);
						m_Groups[ EEntGroup::WORLD_ALL ].push_back( pEntity );
						break;
					}
					if (Hash::IsAtomBomb(szName))
					{
						m_Groups[EEntGroup::ATOMBOMB].push_back(pEntity);
						break;
					}
					if (Hash::IsControlPointHolo(szName))
					{
						m_Groups[EEntGroup::CONTROLPOINTHOLO].push_back(pEntity);
						break;
					}
					break;
				}

				case EClassIds::CDynamicProp:
				{
					const auto szName = pModel->GetModelName();
					if (Hash::IsControlPoint(szName))
					{
						m_Groups[EEntGroup::CONTROLPOINT].push_back(pEntity);
						break;
					}
					break;
				}

				case EClassIds::CTFAmmoPack:
				{
					m_Groups[EEntGroup::WORLD_AMMO].push_back(pEntity);
					m_Groups[ EEntGroup::WORLD_ALL ].push_back( pEntity );
					break;
				}

				case EClassIds::CTFViewModel:
				{
					m_Groups[ EEntGroup::WORLD_VIEWMODEL ].push_back( pEntity );
					break;
				}
				
				case EClassIds::CTFProjectile_Rocket:
				case EClassIds::CTFGrenadePipebombProjectile:
				case EClassIds::CTFProjectile_Jar:
				case EClassIds::CTFProjectile_JarGas:
				case EClassIds::CTFProjectile_JarMilk:
				case EClassIds::CTFProjectile_Arrow:
				case EClassIds::CTFProjectile_SentryRocket:
				case EClassIds::CTFProjectile_Flare:
				case EClassIds::CTFProjectile_Cleaver:
				case EClassIds::CTFProjectile_HealingBolt:
				case EClassIds::CTFProjectile_EnergyRing:
				case EClassIds::CTFProjectile_EnergyBall:
				case EClassIds::CTFProjectile_ThrowableBreadMonster:
				case EClassIds::CTFStunBall:
				case EClassIds::CTFBall_Ornament:
				{
					m_Groups[EEntGroup::WORLD_PROJECTILES].push_back(pEntity);
					m_Groups[ EEntGroup::WORLD_ALL ].push_back( pEntity );
					break;
				}
				case EClassIds::CTFPlayerResource:
				{
					m_pPlayerResource = reinterpret_cast< CTFPlayerResource * >( pEntity );
					break;
				}
				case EClassIds::CTFPumpkinBomb:
				case EClassIds::CTFGenericBomb:
				{
					m_Groups[EEntGroup::WORLD_BOMBS].push_back(pEntity);
					m_Groups[EEntGroup::WORLD_ALL].push_back(pEntity);
					break;
				}
				case EClassIds::CCaptureFlag:
				{
					m_Groups[EEntGroup::INTELLIGENCE].push_back(pEntity);
					m_Groups[ EEntGroup::WORLD_ALL ].push_back( pEntity );
					break;
				}
				case EClassIds::CHeadlessHatman:
				case EClassIds::CTFTankBoss:
				case EClassIds::CMerasmus:
				case EClassIds::CZombie:
				case EClassIds::CEyeballBoss:
				{
					m_Groups[ EEntGroup::WORLD_NPC ].push_back( pEntity );
					break;
				}
				default: break;
			}
		}
	}
}

void CEntityCache::Clear()
{
	for (auto &Group : m_Groups)
		Group.second.clear();

	m_pLocal = nullptr;
	m_pWeapon = nullptr;
	m_pPlayerResource = nullptr;
}

const std::vector<IClientEntity *> &CEntityCache::GetGroup(EEntGroup Group) {
	return m_Groups[Group];
}

CTFPlayer *CEntityCache::GetLocal() {
	return m_pLocal;
}

CTFWeaponBase *CEntityCache::GetWeapon() {
	return m_pWeapon;
}

CTFPlayerResource *CEntityCache::GetPR( ) {
	return m_pPlayerResource;
}