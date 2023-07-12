#pragma once
#include "baseanimating.h"

class CBaseProjectile : public CBaseAnimating
{
public:
	NETVAR(m_hOriginalLauncher, int, "CBaseProjectile", "m_hOriginalLauncher"); //?
};

class CBaseGrenade : public CBaseProjectile
{
public:
	NETVAR(m_flDamage, float, "CBaseGrenade", "m_flDamage");
	NETVAR(m_DmgRadius, float, "CBaseGrenade", "m_DmgRadius");
	NETVAR(m_bIsLive, bool, "CBaseGrenade", "m_bIsLive");
	NETVAR(m_hThrower, int, "CBaseGrenade", "m_hThrower"); //?
	NETVAR(m_vecVelocity, Vec3, "CBaseGrenade", "m_vecVelocity");
	NETVAR(m_fFlags, int, "CBaseGrenade", "m_fFlags");
};

class CTFBaseRocket : public CBaseAnimating
{
public:
	NETVAR(m_vInitialVelocity, Vec3, "CTFBaseRocket", "m_vInitialVelocity");
	NETVAR(m_vecOrigin, Vec3, "CTFBaseRocket", "m_vecOrigin");
	NETVAR(m_angRotation, Vec3, "CTFBaseRocket", "m_angRotation");
	NETVAR(m_iDeflected, int, "CTFBaseRocket", "m_iDeflected");
	NETVAR(m_hLauncher, int, "CTFBaseRocket", "m_hLauncher"); //?
};

class CTFWeaponBaseGrenadeProj : public CBaseAnimating
{
public:
	NETVAR(m_vInitialVelocity, Vec3, "CTFWeaponBaseGrenadeProj", "m_vInitialVelocity");
	NETVAR(m_bCritical, bool, "CTFWeaponBaseGrenadeProj", "m_bCritical");
	NETVAR(m_iDeflected, int, "CTFWeaponBaseGrenadeProj", "m_iDeflected");
	NETVAR(m_vecOrigin, Vec3, "CTFWeaponBaseGrenadeProj", "m_vecOrigin");
	NETVAR(m_angRotation, Vec3, "CTFWeaponBaseGrenadeProj", "m_angRotation");
	NETVAR(m_hDeflectOwner, int, "CTFWeaponBaseGrenadeProj", "m_hDeflectOwner");
};

class CTFGrenadePipebombProjectile : public CBaseAnimating
{
public:
	NETVAR(m_bTouched, bool, "CTFGrenadePipebombProjectile", "m_bTouched");
	NETVAR(m_iType, int, "CTFGrenadePipebombProjectile", "m_iType");
	NETVAR(m_hLauncher, int, "CTFGrenadePipebombProjectile", "m_hLauncher"); //?
	NETVAR(m_bDefensiveBomb, int, "CTFGrenadePipebombProjectile", "m_bDefensiveBomb");
};