#pragma once
#include "../../SDK/SDK.h"

namespace Colors
{
	inline Color_t GetEntityColor( CBaseEntity *pEntity, CTFPlayer* pLocal )
	{
		if ( pEntity == pLocal || pEntity->As<CTFPlayer>()->PlayerOnFriendsList( ) )
			return Vars::Colors::LocalPlayer;

		if ( pEntity->As<CTFPlayer>( )->InCond( TF_COND_STEALTHED ) )
			return Vars::Colors::Cloaked;

		/*switch ( pEntity->m_iTeamNum( ) ) {
			case 2: return Vars::Colors::Enemy;
			case 3: return Vars::Colors::Teammate;
			default: return Vars::Colors::Default;
		}*/

		if ( Vars::Colors::Scheme.var == 0 )
		{
			if ( pEntity->m_iTeamNum( ) != pLocal->m_iTeamNum( ) )
				return Vars::Colors::Enemy;
			else if ( pEntity->m_iTeamNum( ) == pLocal->m_iTeamNum( ) )
				return Vars::Colors::Teammate;
			else
				return Vars::Colors::Default;
		}

		if ( Vars::Colors::Scheme.var == 1 )
		{
			if ( pEntity->m_iTeamNum( ) == TEAM_RED )
				return Vars::Colors::Red;
			else if ( pEntity->m_iTeamNum( ) == TEAM_BLU )
				return Vars::Colors::Blu;
			else
				return Vars::Colors::Default;
		}

		return Vars::Colors::Default;
	}

	inline Color_t GetBuildingColor( CBaseObject *pObject, CTFPlayer *pLocal )
	{
		if ( GET_ENT_H( pObject->m_hBuilder( ) ) == pLocal )
			return Vars::Colors::LocalBuilding;

		/*switch ( pObject->m_iTeamNum( ) ) {
			case 2: return Vars::Colors::EnemyBuilding;
			case 3: return Vars::Colors::TeammateBuilding;
			default: return Vars::Colors::Default;
		}*/

		if ( Vars::Colors::Scheme.var == 0 )
		{
			if ( pObject->m_iTeamNum( ) != pLocal->m_iTeamNum( ) )
				return Vars::Colors::EnemyBuilding;
			else if ( pObject->m_iTeamNum( ) == pLocal->m_iTeamNum( ) )
				return Vars::Colors::TeammateBuilding;
			else
				return Vars::Colors::Default;
		}

		if ( Vars::Colors::Scheme.var == 1 )
		{
			if ( pObject->m_iTeamNum( ) == TEAM_RED )
				return Vars::Colors::RedBuilding;
			else if ( pObject->m_iTeamNum( ) == TEAM_BLU )
				return Vars::Colors::BluBuilding;
			else
				return Vars::Colors::Default;
		}

		return Vars::Colors::Default;
	}

	inline Color_t GetProjectileColor( CBaseGrenade *pProjectile, CTFPlayer *pLocal )
	{
		if ( GET_ENT_H( pProjectile->m_hThrower( ) ) == pLocal )
			return Vars::Colors::LocalPlayer;

		/*switch ( pProjectile->m_iTeamNum( ) ) {
			case 2: return Vars::Colors::Enemy;
			case 3: return Vars::Colors::Teammate;
			default: return Vars::Colors::Default;
		}*/

		if ( Vars::Colors::Scheme.var == 0 )
		{
			if ( pProjectile->m_iTeamNum( ) != pLocal->m_iTeamNum( ) )
				return Vars::Colors::Enemy;
			else if ( pProjectile->m_iTeamNum( ) == pLocal->m_iTeamNum( ) )
				return Vars::Colors::Teammate;
			else
				return Vars::Colors::Default;
		}

		if ( Vars::Colors::Scheme.var == 1 )
		{
			if ( pProjectile->m_iTeamNum( ) == TEAM_RED )
				return Vars::Colors::Red;
			else if ( pProjectile->m_iTeamNum( ) == TEAM_BLU )
				return Vars::Colors::Blu;
			else
				return Vars::Colors::Default;
		}

		return Vars::Colors::Default;
	}

	inline Color_t GetWorldColor( CBaseEntity *pAnimated, CTFPlayer *pLocal )
	{
		const auto &pModel = pAnimated->As<CBaseEntity>( );

		if ( pAnimated->GetClassId( ) == EClassIds::CBaseAnimating && Hash::IsHealth( pModel->GetModelName( ) ) )
		{
			return Vars::Colors::Health;
		}

		if ( ( pAnimated->GetClassId( ) == EClassIds::CBaseAnimating && Hash::IsAmmo( pModel->GetModelName( ) ) ) || pAnimated->GetClassId( ) == EClassIds::CTFAmmoPack )
		{
			return Vars::Colors::Ammo;
		}

		if ( pAnimated->GetClassId( ) == EClassIds::CTFProjectile_Rocket || pAnimated->GetClassId( ) == EClassIds::CTFGrenadePipebombProjectile )
		{
			return GetProjectileColor( pAnimated->As<CBaseGrenade>( ), pLocal );
		}

		if ( pAnimated->GetClassId( ) == EClassIds::CTFGenericBomb || pAnimated->GetClassId( ) == EClassIds::CTFPumpkinBomb )
		{
			return Vars::Colors::Bomb;
		}

		if ( pAnimated->GetClassId( ) == EClassIds::CCaptureFlag )
		{
			/*switch ( pAnimated->m_iTeamNum( ) )
			{
				case 2: return Vars::Colors::Enemy;
				case 3: return Vars::Colors::Teammate;
				default: return Vars::Colors::Default;
			}*/
			
			if ( Vars::Colors::Scheme.var == 0 )
			{
				if ( pAnimated->m_iTeamNum( ) != pLocal->m_iTeamNum( ) )
					return Vars::Colors::Enemy;
				else if ( pAnimated->m_iTeamNum( ) == pLocal->m_iTeamNum( ) )
					return Vars::Colors::Teammate;
				else
					return Vars::Colors::Default;
			}

			if ( Vars::Colors::Scheme.var == 1 )
			{
				if ( pAnimated->m_iTeamNum( ) == TEAM_RED )
					return Vars::Colors::Red;
				else if ( pAnimated->m_iTeamNum( ) == TEAM_BLU )
					return Vars::Colors::Blu;
				else
					return Vars::Colors::Default;
			}

			return Vars::Colors::Default;
		}
		return Vars::Colors::Other;
	}

	inline Color_t GetColorNPC( CBaseEntity *pEntity, CTFPlayer* pLocal )
	{
		switch ( pEntity->GetClassId( ) )
		{
			case EClassIds::CHeadlessHatman: //purple
			{
				return Vars::Colors::HeadlessHatman;
				break;
			}
			case EClassIds::CTFTankBoss:
			{
				return Vars::Colors::Tank;
				break;
			}
			case EClassIds::CMerasmus: //green
			{
				return Vars::Colors::Merasmus;
				break;
			}
			case EClassIds::CZombie: //team
			{
				if ( Vars::Colors::Scheme.var == 0 )
				{
					if ( pEntity->m_iTeamNum( ) == pLocal->m_iTeamNum( ) )
					{
						return Vars::Colors::Teammate;
					}
					else
					{
						if ( pEntity->m_iTeamNum( ) == TEAM_RED || pEntity->m_iTeamNum( ) == TEAM_BLU )
						{
							return Vars::Colors::Enemy;
						}
						else
						{
							return Vars::Colors::Zombie;
						}
					}
				}

				if ( Vars::Colors::Scheme.var == 1 )
				{
					if ( pEntity->m_iTeamNum( ) == TEAM_RED )
					{
						return Vars::Colors::Red;
					}
					else if ( pEntity->m_iTeamNum( ) == TEAM_BLU )
					{
						return Vars::Colors::Blu;
					}
					else
					{
						return Vars::Colors::Zombie;
					}
				}
				break;
			}
			case EClassIds::CEyeballBoss: //team
			{
				if ( Vars::Colors::Scheme.var == 0 )
				{
					if ( pEntity->m_iTeamNum( ) == pLocal->m_iTeamNum( ) )
					{
						return Vars::Colors::Teammate;
					}
					else
					{
						if ( pEntity->m_iTeamNum( ) == TEAM_RED || pEntity->m_iTeamNum( ) == TEAM_BLU )
						{
							return Vars::Colors::Enemy;
						}
						else
						{
							return Vars::Colors::Monoculus;
						}
					}
				}

				if ( Vars::Colors::Scheme.var == 1 )
				{
					if ( pEntity->m_iTeamNum( ) == TEAM_RED )
					{
						return Vars::Colors::Red;
					}
					else if ( pEntity->m_iTeamNum( ) == TEAM_BLU )
					{
						return Vars::Colors::Blu;
					}
					else
					{
						return Vars::Colors::Zombie;
					}
				}
				break;
			}
			default: return Vars::Colors::Other;
		}
		return Vars::Colors::Other;
	}
}