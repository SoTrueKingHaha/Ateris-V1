#include "IEngineTrace.h"
#include "baseentity.h"
#include "tf_player.h"

bool CTraceFilterHitscan::ShouldHitEntity(void* pEntityHandle, int nContentsMask)
{
	CBaseEntity* pEntity = reinterpret_cast<CBaseEntity*>(pEntityHandle);
	CTFPlayer *pLocal = reinterpret_cast< CTFPlayer * >( this->pSkip );

	switch (pEntity->GetClassId( ) )
	{
		case EClassIds::CFuncAreaPortalWindow:
		case EClassIds::CFuncRespawnRoomVisualizer:
		case EClassIds::CSniperDot:
		case EClassIds::CTFMedigunShield:
		case EClassIds::CTFReviveMarker: {
			return false;
		}
	}

	return (pEntityHandle != pSkip);
}

TraceType_t CTraceFilterHitscan::GetTraceType() const
{
	return TRACE_EVERYTHING;
}

bool CTraceFilterWorldAndPropsOnly::ShouldHitEntity(void* pEntityHandle, int nContentsMask)
{
	CBaseEntity* pEntity = reinterpret_cast<CBaseEntity*>(pEntityHandle);

	switch ( pEntity->GetClassId( ) )
	{
		case EClassIds::CFuncAreaPortalWindow:
		case EClassIds::CFuncRespawnRoomVisualizer:
		case EClassIds::CSniperDot:
		case EClassIds::CTFMedigunShield:
		case EClassIds::CTFReviveMarker:
		case EClassIds::CTFAmmoPack:
		case EClassIds::CTFProjectile_Arrow:
		case EClassIds::CTFProjectile_BallOfFire:
		case EClassIds::CTFProjectile_Cleaver:
		case EClassIds::CTFProjectile_Jar:
		case EClassIds::CTFProjectile_JarMilk:
		case EClassIds::CTFProjectile_EnergyBall:
		case EClassIds::CTFProjectile_EnergyRing:
		case EClassIds::CTFProjectile_Flare:
		case EClassIds::CTFProjectile_HealingBolt:
		case EClassIds::CTFProjectile_MechanicalArmOrb:
		case EClassIds::CTFProjectile_Rocket:
		case EClassIds::CTFProjectile_SentryRocket:
		case EClassIds::CTFGrenadePipebombProjectile:
		case EClassIds::CTFPlayer:
		case EClassIds::CMerasmus:
		case EClassIds::CMerasmusDancer:
		case EClassIds::CEyeballBoss:
		case EClassIds::CHeadlessHatman:
		case EClassIds::CZombie: {
			return false;
		} break;
		default:
			break;
	}

	return !(pEntityHandle == pSkip);
}

TraceType_t CTraceFilterWorldAndPropsOnly::GetTraceType() const
{
	return TRACE_EVERYTHING;
}