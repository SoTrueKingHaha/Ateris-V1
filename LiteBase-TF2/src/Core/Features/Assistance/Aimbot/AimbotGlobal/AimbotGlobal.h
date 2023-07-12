#pragma once
#include "../../../../SDK/SDK.h"

enum struct ETargetType {
	UNKNOWN, PLAYER, BUILDING, STICKY
};

enum struct ESortMethod {
	UNKNOWN, FOV, DISTANCE
};

struct Target_t
{
	CBaseEntity *m_pEntity		= nullptr;
	ETargetType m_TargetType	= ETargetType::UNKNOWN;
	Vec3 m_vPos					= {};
	Vec3 m_vAngleTo				= {};
	float m_flFOVTo				= std::numeric_limits<float>::max();
	float m_flDistTo			= std::numeric_limits<float>::max();
	int m_nAimedHitbox			= -1;
	bool m_bHasMultiPointed		= false;
	Priority m_Priority = {};
	// Backtrack
	bool ShouldBacktrack = false;
	float SimTime = 0.f;
};

class CAimbotGlobal
{
public:
	bool IsKeyDown();
	void SortTargets(const ESortMethod &Method);
	const Target_t &GetBestTarget(const ESortMethod &Method);
	Priority GetPriority( int targetIdx );

public:
	std::vector<Target_t> m_vecTargets = {};
};

inline CAimbotGlobal g_AimbotGlobal;