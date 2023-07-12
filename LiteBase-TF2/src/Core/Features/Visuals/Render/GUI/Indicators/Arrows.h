#pragma once
#include "../../../../../SDK/SDK.h"

class CPlayerArrows
{
private:
	bool ShouldRun( CTFPlayer *pLocal );
	void DrawArrowTo( const Vec3 &vecFromPos, const Vec3 &vecToPos, Color_t color, byte alpha );

	std::vector<Vec3> m_vecPlayers = {};

public:
	void Run( );
};

inline CPlayerArrows g_Arrows;