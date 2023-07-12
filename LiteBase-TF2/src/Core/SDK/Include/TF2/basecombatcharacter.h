#pragma once
#include "baseflex.h"
#include "studio.h"

class CBaseCombatCharacter : public CBaseFlex
{
public:
	NETVAR(m_flNextAttack, float, "CBaseCombatCharacter", "m_flNextAttack");
	NETVAR(m_hActiveWeapon, int, "CBaseCombatCharacter", "m_hActiveWeapon");
	NETVAR(m_hMyWeapons, void *, "CBaseCombatCharacter", "m_hMyWeapons");
	NETVAR(m_bGlowEnabled, bool, "CBaseCombatCharacter", "m_bGlowEnabled");
};