#pragma once
#include "../Draw/Draw.h"
#include "../PlayerResource/PlayerResource.h"

enum class EEntGroup {
	PLAYERS_ALL, PLAYERS_ENEMIES, PLAYERS_TEAMMATES,
	BUILDINGS_ALL, BUILDINGS_ENEMIES, BUILDINGS_TEAMMATES,
	SENTRY_ALL, SENTRY_ENEMIES, SENTRY_TEAMMATES,
	DISPENSER_ALL, DISPENSER_ENEMIES, DISPENSER_TEAMMATES,
	TELEPORTER_ALL, TELEPORTER_ENEMIES, TELEPORTER_TEAMMATES,
	WORLD_HEALTH, WORLD_AMMO, WORLD_VIEWMODEL, WORLD_PROJECTILES,
	LOCAL_STICKIES, WORLD_FLARES, WORLD_STICKIES,
	WORLD_BOMBS, INTELLIGENCE, BOMBCART, CONTROLPOINT,
	CONTROLPOINTHOLO, ATOMBOMB, WORLD_ALL, WORLD_NPC
};

class CEntityCache
{
private:
	std::map<EEntGroup, std::vector<IClientEntity *>> m_Groups;
	CTFPlayer *m_pLocal = nullptr;
	CTFWeaponBase *m_pWeapon = nullptr;
	CTFPlayerResource *m_pPlayerResource = nullptr;

public:
	void Fill();
	void Clear();

public:
	const std::vector<IClientEntity *> &GetGroup(EEntGroup Group);
	CTFPlayer *GetLocal();
	CTFWeaponBase *GetWeapon();
	CTFPlayerResource *GetPR( );
};

inline CEntityCache g_EntityCache;