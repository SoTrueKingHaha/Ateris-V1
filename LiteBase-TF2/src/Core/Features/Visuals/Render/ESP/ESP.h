#pragma once
#include "../../../../SDK/SDK.h"
#include "../../../../SDK/Include/EntityCache/EntityCache.h"
#include "../../Colors.h"

class __ESP
{
public:
	void Render( );
	void RenderPlayers( CTFPlayer* pLocal );
	void RenderBuildings( CTFPlayer* pLocal );
	void RenderWorld( CTFPlayer *pLocal );
	void RenderNPC( CTFPlayer *pLocal );
	static void DrawBones( CTFPlayer *pPlayer, const std::vector<int> &vecBones, Color_t clr );
	bool reset = false;
	bool active = false;
	float yline = 0.f;
	CMoveData moveData = {};
	Vec3 absOrigin = {};
	inline static float m_Health[ 128 ] = { 0 };
	inline static float m_HealthBuildings[ 128 * 4 ] = { 0 };
};

inline __ESP ESP;